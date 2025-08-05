const int moisturePin = A0;
const int pumpRelayPin = 7;

// === CONFIGURABLE BY USER ===
float potVolume = 1000.0; // mL (or cm³)
const float soilWaterCapacityPerCm3 = 0.4; // mL/cm³ (tweak based on soil)
const float pumpRate = 10.0; // mL/sec (calibrate by testing pump)
const int targetMoisture = 75; // The desired moisture level (%)

// === SENSOR CALIBRATION 
const int dryValue = 800; 
const int wetValue = 300; 

void setup() {
  pinMode(pumpRelayPin, OUTPUT);
  digitalWrite(pumpRelayPin, HIGH); 
  Serial.begin(9600);
}

void loop() {
  int moistureReading = analogRead(moisturePin);

  // Corrected: Map the higher 'dryValue' to 0% and the lower 'wetValue' to 100%
  float moisturePercent = map(moistureReading, dryValue, wetValue, 0, 100);
  moisturePercent = constrain(moisturePercent, 0, 100);

  Serial.print("Moisture: ");
  Serial.print(moisturePercent);
  Serial.println("%");

  if (moisturePercent < targetMoisture) {
    // Calculate the water needed based on the difference between target and current moisture
    float waterCapacity = potVolume * soilWaterCapacityPerCm3; // mL
    float missingWaterPercent = targetMoisture - moisturePercent;
    float missingWater = waterCapacity * (missingWaterPercent / 100.0);

    // Calculate pump duration in milliseconds
    int pumpDuration = (int)((missingWater / pumpRate) * 1000); // ms

    Serial.print("Water needed: ");
    Serial.print(missingWater);
    Serial.print(" mL, Pumping for ");
    Serial.print(pumpDuration);
    Serial.println(" ms");

    digitalWrite(pumpRelayPin, LOW); // Turn the pump ON
    delay(pumpDuration); // Wait for the specified duration
    digitalWrite(pumpRelayPin, HIGH); // Turn the pump OFF
  }

  delay(30000); // Check every 30 seconds
}
