#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"

// FOR RAPSBERRY PI PICO
// gp4 -> sda
// gp5 -> scl
// pin 38 (gnd) -> GND
// pin 36 (3v3 out) -> 3v3
// code written by google gemini.

// Create the sensor object
Adafruit_BME680 bme; 

void setup() {
  Serial.begin(115200);
  while (!Serial); // Wait for the Serial Monitor to open

  Serial.println("BME680 Test Starting...");

  // Initialize I2C on GP4 (SDA) and GP5 (SCL)
  Wire.begin();

  // Try to find the sensor. Most generic modules use address 0x76.
  // If this fails, the code will try 0x77 automatically.
  if (!bme.begin(0x76)) {
    if (!bme.begin(0x77)) {
      Serial.println("Could not find a BME680 sensor. Check your wiring!");
      while (1);
    }
  }

  // Set up oversampling and filter
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320°C for 150 ms
  Serial.print("temp, humidity, pressure, gas res");
}

void loop() {
  if (!bme.performReading()) {
    return; // Skip this loop if reading fails
  }

  // The Serial Plotter reads labels in the first line or uses specific formatting:
  // Format: "Label1:Value1, Label2:Value2, Label3:Value3"
  
  Serial.print("Temp_C:");
  Serial.print(bme.temperature);
  Serial.print(",");

  Serial.print("Humidity_%:");
  Serial.print(bme.humidity);
  Serial.print(",");

  Serial.print("Pressure_hPa:");
  Serial.print(bme.pressure / 100.0);
  
  // Note: I left out Gas Resistance because its value (e.g., 50,000) 
  // is so much higher than Temperature (25) that it will "squash" 
  // the other lines to the bottom of the graph.
  
  Serial.println(); // New line tells the plotter "this is one data point"
  
  delay(100); // Faster updates for a smoother graph
}