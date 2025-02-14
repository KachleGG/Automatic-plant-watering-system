#include <Wire.h>

#define RELAY_PIN 6
#define SOIL_SENSOR_PIN A0
#define DEFAULT_MOISTURE 50  // Preset moisture level if no input is received

float moisture = 0;
float wantedMoisture = 0;  // Moisture threshold in percentage

void setup() {
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW);  // Ensure pump is off
    Serial.begin(9600);
    Serial.println("Setup complete");

    // Wait for user input to set the moisture threshold (in percentage)
    Serial.println("Input your wanted moisture level (in %) within 10 seconds:");
    
    unsigned long startTime = millis();
    while (millis() - startTime < 10000) {  // 10-second timeout
        if (Serial.available() > 0) {
            wantedMoisture = Serial.parseInt(); // Read input number
            break;
        }
    }

    // If no input is received, set to default
    if (wantedMoisture == 0) {
        wantedMoisture = DEFAULT_MOISTURE;
        Serial.println("No input received. Using default moisture level.");
    }

    Serial.print("Wanted Moisture Set To: ");
    Serial.println(wantedMoisture);
}

void loop() {
    moisture = readSoilMoisture(); // Get sensor input
    float moisturePercentage = convertToPercentage(moisture); // Convert to percentage

    Serial.print("Current Moisture: ");
    Serial.print(moisture);
    Serial.print(" (");
    Serial.print(moisturePercentage);
    Serial.println("%)");

    Watering(moisturePercentage); // Pass the moisture percentage to Watering
    delay(1000); // Delay for a second
}

void Watering(float moisturePercentage) {
    if (moisturePercentage < wantedMoisture) {  
        turnPumpOn();  
    } else {
        turnPumpOff(); 
    }
}

int readSoilMoisture() {
    return analogRead(SOIL_SENSOR_PIN); // Reads from soil moisture sensor
}

float convertToPercentage(int rawValue) {
    return (rawValue / 1023.0) * 100.0;  // Convert to percentage based on 0-1023 range
}

void turnPumpOn() { 
    digitalWrite(RELAY_PIN, HIGH);  
}

void turnPumpOff() { 
    digitalWrite(RELAY_PIN, LOW);   
}
