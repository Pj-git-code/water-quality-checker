#include <OneWire.h>
#include <DallasTemperature.h>

// --- Sensor Pin Definitions ---
#define TURBIDITY_SENSOR A0
#define TDS_SENSOR A1
#define ONE_WIRE_BUS 2  // DS18B20 Data Pin

// --- Calibration Constants ---
#define VREF 5.0
#define ADC_RESOLUTION 1023.0
#define TDS_FACTOR 0.5  // Based on TDS sensor datasheet

// --- Turbidity Calibration ---
#define CLEAN_WATER_VOLTAGE 0.77  // Voltage in clean water (from real measurement)
#define TURBIDITY_SCALE 1000.0    // Adjust this for proper NTU scaling

// DS18B20 Setup
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(9600);
  sensors.begin();
  delay(1000); // Allow sensors to stabilize
}

void loop() {
  // --- Temperature Reading ---
  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);
  if (temperatureC == DEVICE_DISCONNECTED_C) {
    Serial.println("Error: Temperature sensor not connected!");
    return;
  }

  // --- TDS Reading with Temperature Compensation ---
  int tdsRaw = analogRead(TDS_SENSOR);
  float tdsVoltage = tdsRaw * (VREF / ADC_RESOLUTION);
  float compensationCoefficient = 1.0 + 0.02 * (temperatureC - 25.0);
  float compensatedVoltage = tdsVoltage / compensationCoefficient;
  float tdsValue = (compensatedVoltage * TDS_FACTOR) * 1000.0; // in ppm

  // --- Turbidity Reading ---
  int turbidityRaw = analogRead(TURBIDITY_SENSOR);
  float turbVoltage = turbidityRaw * (VREF / ADC_RESOLUTION);
  float turbidityNTU;

  if (turbVoltage >= CLEAN_WATER_VOLTAGE) {
    turbidityNTU = 0; // Clear water
  } else {
    turbidityNTU = (CLEAN_WATER_VOLTAGE - turbVoltage) * TURBIDITY_SCALE;
  }

  // --- Output All Readings ---
  Serial.print("Temp: ");
  Serial.print(temperatureC, 2);
  Serial.print(" °C | TDS: ");
  Serial.print(tdsValue, 2);
  Serial.print(" ppm | Turbidity: ");
  Serial.print(turbidityNTU, 2);
  Serial.print(" NTU | Voltage: ");
  Serial.println(turbVoltage, 3);

  delay(1000); // 1-second interval
}
