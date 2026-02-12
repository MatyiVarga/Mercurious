#include <SoftwareSerial.h>
#include <TinyGPS++.h>

SoftwareSerial gpsSerial(8, 9);
TinyGPSPlus gps;

unsigned long lastPrint = 0;

void setup() {
  gpsSerial.begin(9600);
  Serial.begin(9600);
}

void loop() {
  // 1. Constantly feed the GPS object
  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
  }

  // 2. Print every 1000ms (1 Hz)
  if (millis() - lastPrint >= 1000) {
    lastPrint = millis(); 

    Serial.print("Satellites: ");
    Serial.println(gps.satellites.value());

    // Check 1: Has it EVER found a location?
    if (!gps.location.isValid()) {
      Serial.println("Status: Waiting for GPS Fix...");
    }
    // Check 2: Is the data fresh? (younger than 1.5 seconds)
    else if (gps.location.age() > 1500) {
      Serial.println("Status: Signal Lost! (Data is stale)");
      Serial.print("Last valid age: ");
      Serial.print(gps.location.age());
      Serial.println(" ms ago");
    }
    // If valid and fresh, print coordinates
    else {
      Serial.print("Status: Active\n");
      Serial.print("Latitude: ");
      Serial.println(gps.location.lat(), 6);
      
      Serial.print("Longitude: ");
      Serial.println(gps.location.lng(), 6);
    }
    
    Serial.println("---------------------");
  }
}