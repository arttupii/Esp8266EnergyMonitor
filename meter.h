#ifndef METER__H_
#define METER__H_
#include <ESP8266WiFi.h>

#define MEASUREMENT_TIME_S 15
#define PULSEARRAY_LENGTH_TIME_S (60*60)
#define PULSEARRAY_SIZE (PULSEARRAY_LENGTH_TIME_S/MEASUREMENT_TIME_S)
class Meter {
	public:
		Meter(int gpioPin);
		void reset();
		String getUpdate();
		String getPulseArray();
		void run();
		void begin();
	private:
		static void highInterrupt();
		static uint64_t counter0;
		static uint64_t counter1;
		uint64_t time1;
		uint64_t measurementTimer;

		unsigned int pulseLength;	
		static unsigned int counter2;

		unsigned int pulseLengthArray[PULSEARRAY_SIZE];
		int arrayIndex;
		int pin;
};

#endif
