# BME280InfluxSleep

Dieses Beispiel zeigt, wie ein ESP32 über I2C einen BME280 Sensor ausliest und die
Messwerte über WLAN in eine InfluxDB schreibt. Nach dem Senden der Daten geht
der ESP32 für 30 Minuten in den Deep-Sleep-Modus, um Energie zu sparen.

## Abhängigkeiten

* [Adafruit BME280 Library](https://github.com/adafruit/Adafruit_BME280_Library)
* [InfluxDB Client Library for Arduino](https://github.com/tobiasschuerg/InfluxDB-Client-for-Arduino)

Diese Bibliotheken können über den Bibliotheksverwalter der Arduino IDE installiert
werden.

## Verwendung

1. `WIFI_SSID` und `WIFI_PASSWORD` mit den Daten des eigenen WLAN ersetzen.
2. `INFLUXDB_URL`, `INFLUXDB_TOKEN`, `INFLUXDB_ORG` und `INFLUXDB_BUCKET`
   entsprechend der InfluxDB-Konfiguration anpassen.
3. Sketch auf einen ESP32 laden. Der Sensor wird standardmäßig unter der
   I2C-Adresse `0x76` erwartet.

Beim Aufwachen misst der ESP32 einmal Temperatur, Luftfeuchtigkeit und
Luftdruck. Anschließend werden die Werte an InfluxDB gesendet. Danach startet
der Deep-Sleep-Modus für 30 Minuten.
