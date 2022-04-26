#include <dht.h> //DHT Adafruit library

dht DHT; //declare DHT

#define DHT11_PIN 7 //DHT input to Pin 7 on Arduino Uno


int digitalPin = 2; // KY-026 digital interface
int analogPin = A2; // KY-026 analog interface
int digitalVal; // digital readings
int analogVal; //analog readings
int wind_risk;
int temp_risk;
int humid_risk;
int CO_risk;

//int wind_threshold_low = 15;
//int wind_threshold_high = 25;
//int temperature_threshold_low = 20;
//int temperature_threshold_high = 35;
//int humidity_threshold_low = 15;
//int humidity_threshold_high = 40;

int wind_threshold_low = 1;
int wind_threshold_high = 3;
int temperature_threshold_low = 30;
int temperature_threshold_high = 50;
int humidity_threshold_low = 15;
int humidity_threshold_high = 40;
int CO_threshold_low = 50;
int CO_threshold_high = 166;

void setup() 
{
  Serial.begin(9600); //initialize serial monitor at 9600
}

void loop() 
{
    //FLAME IR SENSOR
    digitalVal = digitalRead(digitalPin); // Read the digital interface of FLAME sensor
    if(digitalVal == HIGH) // HIGH if flame is detected
    {Serial.println("Fire Detected!"); // print "Fire Detected!" to the serial monitor
    delay(500);}
    
    else
    {
    //WIND SENSOR
    float sensorValue = analogRead(A0);
    float voltage = (sensorValue / 1023) * 5;
    float wind_speed = mapfloat(voltage, 0, 5, 0, 45);
    float speed_mph = ((wind_speed *3600)/1609.344);
    
    if (speed_mph <= wind_threshold_low)
    {wind_risk = 1;}
    else if (speed_mph > wind_threshold_high)
    {wind_risk = 3;}
    else 
    {wind_risk = 2;}

    //TEMPERATURE SENSOR
    int chk = DHT.read11(DHT11_PIN); //read from DHT (humidity and temperature sensor)
    if (DHT.temperature <= temperature_threshold_low)
    {temp_risk = 1;}
    else if (DHT.temperature > temperature_threshold_high)
    {temp_risk = 3;}
    else 
    {temp_risk = 2;}

    //HUMIDITY SENSOR 
    chk = DHT.read11(DHT11_PIN); //read from DHT (humidity and temperature sensor)
    if (DHT.humidity <= humidity_threshold_low)
    {humid_risk = 1;}
    else if (DHT.humidity > humidity_threshold_high)
    {humid_risk = 3;}
    else 
    {humid_risk = 2;}

    //CO SENSOR
    if (analogRead(1) <= CO_threshold_low)
    {humid_risk = 1;}
    else if (analogRead(1) > CO_threshold_high)
    {humid_risk = 3;}
    else 
    {humid_risk = 2;}
      
    //RISK ASSESSMENT
    float risk = (wind_risk *0.3)+(temp_risk*0.4)+(humid_risk*0.3);
    if (risk <= 1.67)
    {Serial.println("LOW Risk of Wildfire");
     delay(5000);}
    else if (risk >= 2.33)
    {Serial.println("HIGH Risk of Wildfire");
    delay(500);}
    else 
    {Serial.println("MEDIUM Risk of Wildfire");
    delay(2000);}

  }
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
