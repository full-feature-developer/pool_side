#include <DallasTemperature.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <OneWire.h>

// Replace with your network details
const char* ssid = "thermo_server";
const char* password = "";
const char* serverAddress = "";

#define ONE_WIRE_BUS 15 // DS18B20 on pin 15 (a GPIO pin)

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);


void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  sensors.begin();
}

void loop() {
  sensors.requestTemperatures();
  float temperature = sensors.getTempFByIndex(0);

  if (temperature == DEVICE_DISCONNECTED_C) {
    Serial.println("Error: Could not read temperature data");
    return;
  }

  sendTemperature(temperature);
  delay(5000);  // Delay between each transmission
  Serial.println(temperature);
}

void sendTemperature(float temperature) {
  HTTPClient http;

  // Construct the URL with temperature as a query parameter
  String url = String(serverAddress) + "/temperature?value=" + String(temperature);

  http.begin(url);
  int httpResponseCode = http.PUT("");  // Send an empty POST body
  
  if (httpResponseCode > 0) {
    Serial.printf("HTTP PUT Request sent. Response code: %d\n", httpResponseCode);
  } else {
    Serial.println("Error sending HTTP PUT request");
  }

  http.end();
}
