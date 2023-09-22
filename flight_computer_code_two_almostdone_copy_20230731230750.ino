// Arduino Rocket Flight Computer 
// Team Apogee - Code By Ishmael L
// CCNY STEM Institute Summer 2023 - Rocketry/Engineering Research

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"
#include <Adafruit_MPU6050.h>
#include <SD.h>

char fileName; 
float temperature;
float pressure;
float altitude;
float temperaturetwo;
float accx;
float accy;
float accz;
float rotx;
float roty;
float rotz;

#define SEALEVELPRESSURE_HPA (1014)

Adafruit_BMP3XX bmp;
Adafruit_MPU6050 mpu;

File myFile;
const int chipSelect = 4;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  if (!SD.begin(chipSelect)) {
    Serial.println("SD card initialization failed!");
    while (1);
  }
  

  Serial.println("SD card initialized.");
  
  if (!bmp.begin_I2C()) {
    Serial.println("Could not find a valid BMP3 sensor, check wiring!");
    while (1);
  }
  
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1);
  }

  Serial.println("Data logging to CSV started.");

  // Create a new file and write headers
  myFile = SD.open("testt.csv", FILE_WRITE);
  if (myFile) {
    myFile.println("time, temp, pressure, altitude, accx, accy, accz, rotx, roty, rotz");
    myFile.close();
  } else {
    Serial.println("Error opening csv file");
  }
}

void loop() {

  unsigned long timestamp = millis();

  // Convert timestamp to time in HH:MM:SS format
  unsigned long seconds = timestamp / 1000;
  unsigned long minutes = seconds / 60;
  unsigned long hours = minutes / 60;
  unsigned long secondsSinceStart = timestamp / 1000;

  // Calculate the actual time components
  seconds = seconds % 60;
  minutes = minutes % 60;
  hours = hours % 24;

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  float accx = (a.acceleration.x);
  float accy = (a.acceleration.y);
  float accz = (a.acceleration.z);

  float rotx = (g.gyro.x);
  float roty = (g.gyro.y);
  float rotz = (g.gyro.z);

  float temperaturetwo = (((temp.temperature) * 1.8) + 32);

  if (!bmp.performReading()) {
    Serial.println("Failed to perform BMP3 reading :(");
    return;
  }

  float temperature = (((bmp.temperature) * 1.8) + 32);
  float pressure = (bmp.pressure / 100.0);
  float altitude = (bmp.readAltitude(SEALEVELPRESSURE_HPA) * 3.28);

  // Open the file in append mode
  myFile = SD.open("testt.csv", FILE_WRITE);
  if (myFile) {
    // Write data to the file
    myFile.print(secondsSinceStart);
    myFile.print(",");
    myFile.print(temperature);
    myFile.print(",");
    myFile.print(pressure);
    myFile.print(",");
    myFile.print(altitude);
    myFile.print(",");
    myFile.print(accx);
    myFile.print(",");
    myFile.print(accy);
    myFile.print(",");
    myFile.print(accz);
    myFile.print(",");
    myFile.print(rotx);
    myFile.print(",");
    myFile.print(roty);
    myFile.print(",");
    myFile.println(rotz);

    // Close the file
    myFile.close();
  } else {
    Serial.println("Error writing to csv file");
  }

  // Print the data to Serial Monitor

  Serial.print("Seconds: ");
  Serial.println(secondsSinceStart);

  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.println(" Farenheit");
  
  Serial.print("Pressure = ");
  Serial.print(pressure);
  Serial.println(" hPa");
  
  Serial.print("Altitude = ");
  Serial.print(altitude);
  Serial.println(" ft");

  Serial.print("Acceleration X = ");
  Serial.print(accx);
  Serial.println(" m/s^2");
  
  Serial.print("Acceleration Y = ");
  Serial.print(accy);
  Serial.println(" m/s^2");
  
  Serial.print("Acceleration Z = ");
  Serial.print(accz);
  Serial.println(" m/s^2");

  Serial.print("Rotation X = ");
  Serial.print(rotx);
  Serial.println(" rad/s");
  
  Serial.print("Rotation Y = ");
  Serial.print(roty);
  Serial.println(" rad/s");
  
  Serial.print("Rotation Z = ");
  Serial.print(rotz);
  Serial.println(" rad/s");

  Serial.println();
  delay(2000);
}

