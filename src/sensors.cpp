#include <Arduino.h>
#include <sensors.h>

Sensors::Sensors(float temperatureLevel, unsigned int smokeLevel,
                 unsigned int flameLevel, unsigned int gasLevel) :
	temperatureLevel(temperatureLevel),
	smokeLevel(smokeLevel),
	flameLevel(flameLevel),
	gasLevel(gasLevel) {}

int Sensors::isSmokeDetected(void)
{
	return (smokeLevel >= MQ2_MIN_SMOKE_THRESHOLD &&
	        smokeLevel <= MQ2_MAX_SMOKE_THRESHOLD);
}

int Sensors::isFireDetected(void)
{
	return (flameLevel >= FLAME_THRESHOLD);
}

int Sensors::isGasLeakDetected(void)
{
	return (gasLevel == HIGH);
}

int Sensors::isHighTempDetected(void)
{
	return (temperatureLevel >= DHT22_TEMP_THRESHOLD);
}
