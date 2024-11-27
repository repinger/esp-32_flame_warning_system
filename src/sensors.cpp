#include <Arduino.h>
#include <sensors.h>

int smoke_detected(int smokeLevel)
{
	if (smokeLevel >= MQ2_MIN_SMOKE_THRESHOLD
	    && smokeLevel <= MQ2_MAX_SMOKE_THRESHOLD)
		return true;

	return false;
}

int fire_detected(int flameLevel)
{
	if (flameLevel >= FLAME_THRESHOLD)
		return true;

	return false;
}

int high_temp_detected(float temp)
{
	if (temp >= DHT22_TEMP_THRESHOLD)
		return true;

	return false;
}
