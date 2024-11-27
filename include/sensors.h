/* MQ-2 smoke threshold */
#define MQ2_MIN_SMOKE_THRESHOLD		120
#define MQ2_MAX_SMOKE_THRESHOLD		400

/* Flame sensor threshold */
#define FLAME_THRESHOLD			3040

/* Scenarios */
bool smoke_detected(int smokeLevel);
bool fire_detected(int flameLevel);
