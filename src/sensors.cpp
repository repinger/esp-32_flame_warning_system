#include <Arduino.h>
#include <sensors.h>

Sensors::Sensors(float temperatureLevel, unsigned int smokeLevel,
                 unsigned int flameLevel) :
	temperatureLevel(temperatureLevel),
	smokeLevel(smokeLevel),
	flameLevel(flameLevel) {}

int Sensors::isSmokeDetected(void)
{
	return (smokeLevel >= MQ2_MIN_SMOKE_THRESHOLD &&
	        smokeLevel <= MQ2_MAX_SMOKE_THRESHOLD);
}

int Sensors::isFireDetected(void)
{
	return (flameLevel >= FLAME_THRESHOLD);
}

int Sensors::isHighTempDetected(void)
{
	return (temperatureLevel >= DHT22_TEMP_THRESHOLD);
}
