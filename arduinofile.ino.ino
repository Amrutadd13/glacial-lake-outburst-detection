#include <DHT.h>

/* ==============================
   SENSOR PIN CONFIGURATION
   ============================== */

// DHT11 Temperature Sensor
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Ultrasonic Sensor (HC-SR04)
#define TRIG 9
#define ECHO 10

/* ==============================
   THRESHOLDS (FOR DEMO & LOGIC)
   ============================== */

// Temperature threshold (°C)
// Higher temperature → more ice melting
const int TEMP_THRESHOLD = 26;

// Ultrasonic threshold (cm)
// Lower distance → higher water level
const int LEVEL_THRESHOLD = 25;

void setup() {
  Serial.begin(9600);
  dht.begin();

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  Serial.println("=== ICE TEMPERATURE & GLOF MONITORING SYSTEM ===");
}

long getWaterLevel() {
  // Ultrasonic sensor measures distance to water surface
  // Lower distance = higher water level
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH);
  long distance = duration * 0.034 / 2; // cm
  return distance;
}

void loop() {

  float temperature = dht.readTemperature();
  long waterLevel = getWaterLevel();

  /* ==============================
     DATA OUTPUT (DIGITAL TWIN)
     ============================== */

  // Format suitable for Serial Plotter
  Serial.print("Temperature:");
  Serial.print(temperature);
  Serial.print(",WaterLevel:");
  Serial.println(waterLevel);

  /* ==============================
     RISK ASSESSMENT LOGIC
     ============================== */

  if (temperature > TEMP_THRESHOLD && waterLevel < LEVEL_THRESHOLD) {
    Serial.println("RISK STATUS: HIGH");
    Serial.println("CAUSE: Ice melting + high meltwater level");
  }
  else if (temperature > TEMP_THRESHOLD || waterLevel < LEVEL_THRESHOLD) {
    Serial.println("RISK STATUS: WARNING");
    Serial.println("CAUSE: One parameter exceeded safe limit");
  }
  else {
    Serial.println("RISK STATUS: SAFE");
  }

  Serial.println("-----------------------------------");
  delay(2000);
}
