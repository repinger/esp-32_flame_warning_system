#include <Arduino.h>
#include <sensors.h>

bool smoke_detected(int smokeLevel)
{
	if (smokeLevel >= MQ2_MIN_SMOKE_THRESHOLD
	    && smokeLevel <= MQ2_MAX_SMOKE_THRESHOLD)
		return true;

	return false;
}

bool fire_detected(int flameLevel)
{
	if (flameLevel >= FLAME_THRESHOLD)
		return true;

	return false;
}
