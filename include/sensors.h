/* MQ-2 smoke threshold */
#define MQ2_MIN_SMOKE_THRESHOLD    120
#define MQ2_MAX_SMOKE_THRESHOLD    400

/* Flame sensor threshold */
#define FLAME_THRESHOLD            3040

/* DHT-22 temperature threshold */
#define DHT22_TEMP_THRESHOLD       50.0
#define DHT_TYPE                   DHT22

class Sensors {
	private:
		float temperatureLevel;
		unsigned int smokeLevel;
		unsigned int flameLevel;

	public:
		Sensors(float temperatureLevel, unsigned int smokeLevel,
			unsigned int flameLevel);

		/* Scenarios */
		int isSmokeDetected(void);
		int isFireDetected(void);
		int isHighTempDetected(void);
};
