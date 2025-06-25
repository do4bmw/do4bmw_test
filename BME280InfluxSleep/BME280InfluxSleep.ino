#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <WiFi.h>
#include <InfluxDbClient.h>

// WiFi credentials
#define WIFI_SSID "REPLACE_WITH_YOUR_SSID"
#define WIFI_PASSWORD "REPLACE_WITH_YOUR_PASSWORD"

// InfluxDB connection info
#define INFLUXDB_URL "http://your-influxdb-host:8086"
#define INFLUXDB_TOKEN "YOUR_TOKEN"
#define INFLUXDB_ORG "YOUR_ORG"
#define INFLUXDB_BUCKET "YOUR_BUCKET"

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme;          // I2C interface
InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN);
Point sensor("environment");

void setup() {
  Serial.begin(115200);

  // Initialize BME280 sensor
  if (!bme.begin(0x76)) {  // default I2C address
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1) { delay(10); }
  }

  // Connect to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print('.');
  }
  Serial.println(" connected");

  // Read sensor data
  float temperature = bme.readTemperature();
  float humidity = bme.readHumidity();
  float pressure = bme.readPressure() / 100.0F; // hPa

  sensor.clearFields();
  sensor.addTag("device", "esp32");
  sensor.addField("temperature", temperature);
  sensor.addField("humidity", humidity);
  sensor.addField("pressure", pressure);

  // Write the data to InfluxDB
  if (!client.validateConnection()) {
    Serial.print("InfluxDB connection failed: ");
    Serial.println(client.getLastErrorMessage());
  } else {
    if (!client.writePoint(sensor)) {
      Serial.print("InfluxDB write failed: ");
      Serial.println(client.getLastErrorMessage());
    } else {
      Serial.println("Data written to InfluxDB");
    }
  }

  Serial.println("Entering deep sleep for 30 minutes...");
  // Sleep for 30 minutes (30 * 60 seconds * 1,000,000 microseconds)
  esp_sleep_enable_timer_wakeup(30ULL * 60ULL * 1000000ULL);
  esp_deep_sleep_start();
}

void loop() {
  // The ESP32 restarts from setup() after deep sleep, so loop remains empty
}

