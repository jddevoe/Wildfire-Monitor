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

Adafruit_seesaw ss;

dht DHT; //declare DHT

#define DHT11_PIN 7 //DHT input to Pin 7 on Arduino Uno


int digitalPin = 2; // KY-026 digital interface
int analogPin = A2; // KY-026 analog interface
int digitalVal; // digital readings
int analogVal; //analog readings

void setup() 
{
  Serial.begin(9600); //initialize serial monitor at 9600

  if (!ss.begin(0x36)) {
    Serial.println("ERROR! seesaw not found");
    while(1) delay(1);
  } else {
    Serial.print("seesaw started! version: ");
    Serial.println(ss.getVersion(), HEX);
  }
}

void loop() 
{
    //WIND SENSOR
    float sensorValue = analogRead(A0);
    float voltage = (sensorValue / 1023) * 5;
    float wind_speed = mapfloat(voltage, 0, 5, 0, 45);
    float speed_mph = ((wind_speed *3600)/1609.344);
    Serial.print("Wind Speed =");
    //Serial.print(wind_speed);
    //Serial.println("m/s");
    Serial.print(speed_mph);
    Serial.println("mph");
    delay (3000);

    //HUMIDITY SENSOR  
    int chk = DHT.read11(DHT11_PIN);
    Serial.print("Relative Humidity = "); //print "Humidity = " to the serial monitor
    Serial.print(DHT.humidity); //print humidity value to the serial monitor
    Serial.println(" %"); //print " %" to the serial monitor
    delay (3000);

    //TEMPERATURE SENSOR
    chk = DHT.read11(DHT11_PIN); //read from DHT (humidity and temperature sensor)
    Serial.print("Temperature = "); //print "Temperature = " to the serial monitor
    Serial.print(DHT.temperature); //print temperature value (in degrees C) to the serial monitor
    Serial.println(" degrees C"); //print "degrees C" to the serial monitor
    delay (3000);

    //CO SENSOR
    float val;
    val=analogRead(1);//Read Gas value from analog 1
    Serial.print("CO concentration: ");
    Serial.print(val/100);//Print the value to serial port
    Serial.println(" ppm");
    delay (3000);

    //SOIL SENSOR
    float tempC = ss.getTemp();
    uint16_t capread = ss.touchRead(0);

    Serial.print("Soil Temperature: "); Serial.print(tempC); Serial.println("*C");
    Serial.print("Soil Capacitive: "); Serial.println(capread);
    delay(3000);

    //FLAME IR SENSOR
    digitalVal = digitalRead(digitalPin); // Read the digital interface of FLAME sensor
    if(digitalVal == HIGH) // HIGH if flame is detected
    {Serial.println("Fire Detected!");} // print "Fire Detected!" to the serial monitor
    else
    {Serial.println("No Fire Detected");} // print "No Fire Detected" to the serial monitor
    delay (3000);

    Serial.println(" ");
    Serial.println(" ");

}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
