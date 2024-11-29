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
                printf("Failed to connect to Wi-Fi. Restarting . . .\n");
                ESP.restart();
        }

        timeSync(TZ_INFO, "time.cloudflare.com");

        if (client.validateConnection())
                printf("Connected to InfluxDB: %s\n",
			client.getServerUrl().c_str());
        else
                printf("InfluxDB connection failed: %s\n",
			client.getLastErrorMessage().c_str());
#endif
        delay(5000);
}

void loop(void)
{
#if ENABLE_INFLUXDB
	static unsigned long lastSendTime = 0;
#endif
	float temperatureLevel = dht.readTemperature();
        unsigned int smokeLevel = map(constrain(analogRead(MQ2_PIN), 100, 4096), 100, 4096, 0, 2048);
	unsigned int flameLevel = map(constrain(analogRead(FLAME_PIN), 0, 4096), 0, 4096, 3072, 0);
	/* Init sensors class */
        Sensors node_sensors(temperatureLevel, smokeLevel, flameLevel);

	int tempStatus = node_sensors.isHighTempDetected();
	int smokeStatus = node_sensors.isSmokeDetected();
	int flameStatus = node_sensors.isFireDetected();

#if DEBUG
        printf("Smoke: %u\nFlame: %u\nTemp: %.2f\n\n",
		smokeLevel,
                flameLevel,
                temperatureLevel);
#endif

	/* Trigger buzzer */
        if (tempStatus && smokeStatus && flameStatus)
                buzz(200);
        else if (tempStatus || smokeStatus || flameStatus)
                buzz(500);

#if ENABLE_INFLUXDB
        if ((millis() - lastSendTime) >= 5000) {
		Point sensors("esp32_node-1");

                sensors.clearFields();

                /* Send sensor readings */
                sensors.addField("temperatureLevel", temperatureLevel);
                sensors.addField("smokeLevel", smokeLevel);
                sensors.addField("flameLevel", flameLevel);

                /* Send status */
                sensors.addField("isTemperatureHigh", tempStatus);
                sensors.addField("isSmokeDetected", smokeStatus);
                sensors.addField("isFlameDetected", flameStatus);

                if (!client.writePoint(sensors))
                        printf("Failed to write data to InfluxDB: %s\n",
                                client.getLastErrorMessage().c_str());

                lastSendTime = millis();
        }
#endif

        delay(200);
}
