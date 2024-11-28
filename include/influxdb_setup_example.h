#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>

#define DEVICE		"ESP32"

#define TZ_INFO		"WIB-7"

#define INFLUXDB_URL	"https://localhost"
#define INFLUXDB_TOKEN	"my_token"
#define INFLUXDB_ORG	"my_org"
#define INFLUXDB_BUCKET	"my_bucket"

const char InfluxDB_Server_CACert[] PROGMEM =  R"EOF(
-----BEGIN CERTIFICATE-----
-----END CERTIFICATE-----
)EOF";
