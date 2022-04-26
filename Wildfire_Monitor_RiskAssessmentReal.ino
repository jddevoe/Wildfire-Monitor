#include <Adafruit_Crickit.h>
#include <Adafruit_miniTFTWing.h>
#include <Adafruit_NeoKey_1x4.h>
#include <Adafruit_NeoTrellis.h>
#include <Adafruit_seesaw.h>
#include <Adafruit_TFTShield18.h>
#include <seesaw_motor.h>
#include <seesaw_neopixel.h>
#include <seesaw_servo.h>
#include <seesaw_spectrum.h>

#include <dht.h> //DHT Adafruit library

dht DHT; //declare DHT

#define DHT11_PIN 7 //DHT input to Pin 7 on Arduino Uno

Adafruit_seesaw ss;

int digitalPin = 2; // KY-026 digital interface
int analogPin = A2; // KY-026 analog interface
int digitalVal; // digital readings
int analogVal; //analog readings
int wind_risk;
int temp_risk;
int humid_risk;
int CO_risk;
int soiltemp_risk;
int soilcap_risk;

//threshold values
//int wind_threshold_low = 15;
//int wind_threshold_high = 25;
//int temperature_threshold_low = 20;
//int temperature_threshold_high = 35;
//int humidity_threshold_low = 15;
//int humidity_threshold_high = 40;

//threshold values for DEMO
int wind_threshold_low = 1;
int wind_threshold_high = 3;
int temperature_threshold_low = 30;
int temperature_threshold_high = 50;
int humidity_threshold_low = 15;
int humidity_threshold_high = 40;
int CO_threshold_low = 50;
int CO_threshold_high = 166;
int cap_threshold_low = 300;
int cap_threshold_high = 500;

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
    float sensorValue = analogRead(A0); //read analog wind value from A0
    float voltage = (sensorValue / 1023) * 5; //convert to voltage 0-5V
    float wind_speed = mapfloat(voltage, 0, 5, 0, 45); //scale voltage to wind speed (m/s)
    float speed_mph = ((wind_speed *3600)/1609.344); //calculate wind speed in mph
    
    if (speed_mph <= wind_threshold_low) //low wind risk
    {wind_risk = 1;}
    else if (speed_mph > wind_threshold_high) //high wind risk
    {wind_risk = 3;}
    else //medium wind risk
    {wind_risk = 2;}

    //TEMPERATURE SENSOR
    int chk = DHT.read11(DHT11_PIN); //read from DHT (humidity and temperature sensor)
    if (DHT.temperature <= temperature_threshold_low) //low temperature risk
    {temp_risk = 1;}
    else if (DHT.temperature > temperature_threshold_high) //high temperature risk
    {temp_risk = 3;}
    else //medium temperature risk
    {temp_risk = 2;}

    //HUMIDITY SENSOR 
    chk = DHT.read11(DHT11_PIN); //read from DHT (humidity and temperature sensor)
    if (DHT.humidity <= humidity_threshold_low) //low humidity risk
    {humid_risk = 1;}
    else if (DHT.humidity > humidity_threshold_high) //high humidity risk
    {humid_risk = 3;}
    else //medium humidity risk
    {humid_risk = 2;}

    //CO SENSOR
    if (analogRead(1) <= CO_threshold_low) //low CO risk
    {humid_risk = 1;}
    else if (analogRead(1) > CO_threshold_high) //high CO risk
    {humid_risk = 3;}
    else //medium CO risk
    {humid_risk = 2;}

    //SOIL SENSOR
    float tempC = ss.getTemp(); //get soil temperature
    uint16_t capread = ss.touchRead(0); //get soil capacitive 
    if (tempC <= temperature_threshold_low) //low soil temp risk
    {soiltemp_risk = 1;}
    else if (tempC > temperature_threshold_high) //high soil temp risk
    {soiltemp_risk = 3;}
    else //medium soil temp risk
    {soiltemp_risk = 2;}
    
    if (capread >= cap_threshold_high) //low soil moisture risk
    {soilcap_risk = 1;}
    else if (tempC < cap_threshold_low) //high soil moisture risk
    {soilcap_risk = 3;}
    else //medium soil moisture risk
    {soilcap_risk = 2;}
    
    //RISK ASSESSMENT
    float risk = (wind_risk *0.2)+(temp_risk*0.25)+(humid_risk*0.25)+(CO_risk*0.1)+(soiltemp_risk*0.1)+(soilcap_risk*0.1); //weighted sum for risk assessment
    if (risk <= 1.67) //low risk
    {Serial.println("LOW Risk of Wildfire");
     delay(5000);} //wait 5 seconds before sensing again
    else if (risk >= 2.33) //high risk
    {Serial.println("HIGH Risk of Wildfire"); 
    delay(500);} //wait 0.5 seconds before sensing again
    else //medium risk
    {Serial.println("MEDIUM Risk of Wildfire"); 
    delay(2000);} //wait 2 seconds before sensing again

  }
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max) //function to scale wind sensor input voltage to wind speed (m/s)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min; //voltage to wind speed (m/s)
}
