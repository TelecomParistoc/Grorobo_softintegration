#include "include/sensor_thread.hpp"

#include <wiringPi.h>


void init()
{
	wiringPiSetup();

	pinMode(BLACK_SENSORS_SIDE, INPUT);
	pinMode(BLACK_SENSOR_MIDDLE, INPUT);
	pinMode(YELLOW_SENSORS_SIDE, INPUT);
	pinMode(YELLOW_SENSOR_MIDDLE, INPUT);
	pullUpDnControl(BLACK_SENSORS_SIDE, PUD_DOWN);
	pullUpDnControl(BLACK_SENSOR_MIDDLE, PUD_DOWN);
	pullUpDnControl(YELLOW_SENSORS_SIDE, PUD_UP);
	pullUpDnControl(YELLOW_SENSOR_MIDDLE, PUD_UP);
}
