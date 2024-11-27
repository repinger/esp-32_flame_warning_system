#include <Adafruit_Sensor.h>
#include <Arduino.h>
#include <DHT.h>
#include <WiFiManager.h>

#include <setup.h>
#include <sensors.h>
#if ENABLE_INFLUXDB
#include <influxdb_setup.h>

InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDB_Server_CACert);
#endif

DHT dht(DHT22_PIN, DHT_TYPE);

static void buzz(int duration)
{
        tone(BUZZER_PIN, 1000);
        delay(duration);
        noTone(BUZZER_PIN);
}

void setup(void)
{
        Serial.begin(115200);

        pinMode(MQ2_PIN, INPUT);
        pinMode(FLAME_PIN, INPUT);
        pinMode(BUZZER_PIN, OUTPUT);
        dht.begin();

#if ENABLE_INFLUXDB
        WiFi.mode(WIFI_STA);
        WiFiManager wifiManager;

        wifiManager.setConnectTimeout(5);

        if (!wifiManager.autoConnect("Flame Warning System")) {
                Serial.println("Failed to connect to Wi-Fi. Restarting . . .");
                ESP.restart();
        }

        timeSync(TZ_INFO, "time.cloudflare.com");

        if (client.validateConnection()) {
                Serial.print("Connected to InfluxDB: ");
                Serial.println(client.getServerUrl());
        } else {
                Serial.print("InfluxDB connection failed: ");
                Serial.println(client.getLastErrorMessage());
        }
#endif
        delay(5000);
}

void loop(void)
{
#if ENABLE_INFLUXDB
        static unsigned long lastSendTime = 0;
        Point sensors("esp32_node-1");
#endif
        float temperatureLevel = dht.readTemperature();
        unsigned int smokeLevel = analogRead(MQ2_PIN);
        unsigned int flameLevel = analogRead(FLAME_PIN);

        smokeLevel = map(smokeLevel, 100, 4096, 0, 2048);
        flameLevel = map(flameLevel, 0, 4096, 3072, 0);

#if DEBUG
        Serial.print("Smoke: ");
        Serial.println(smokeLevel);
        Serial.print("Flame: ");
        Serial.println(flameLevel);
	Serial.print("Temp: ");
        Serial.println(temperatureLevel);
        Serial.println();
#endif

        if (smoke_detected(smokeLevel) && fire_detected(flameLevel))
                buzz(200);
        else if (smoke_detected(smokeLevel) || fire_detected(flameLevel))
                buzz(500);

#if ENABLE_INFLUXDB
        if ((millis() - lastSendTime) >= 5000) {
                sensors.clearFields();

                sensors.addField("temperatureLevel", temperatureLevel);
                sensors.addField("smokeLevel", smokeLevel);
                sensors.addField("flameLevel", flameLevel);

                if (!client.writePoint(sensors)) {
                        Serial.print("Failed to write data to InfluxDB: ");
                        Serial.println(client.getLastErrorMessage());
                }

                lastSendTime = millis();
        }
#endif

        delay(200);
}
