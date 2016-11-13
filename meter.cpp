#include"meter.h"
#include <ESP8266WiFi.h>

String uint64ToString(uint64_t v) {
	String ret = "";
	if(v==0) return "0";
	while(v!=0) {
		ret = (char)((v%10)+'0') + ret;
		v/=10;
	}
	return ret;
}

uint64_t millis64() {
    static uint64_t low32=0, high32=0;
    uint32_t new_low32 = millis();
    if (new_low32 < low32) high32++;
    low32 = new_low32;
    return (uint64_t) high32 << 32 | low32;
}

Meter::Meter(int gpioPin){
	counter1 = 0;
	counter2 = 0;
    counter0 = 0;
	arrayIndex = 0;
	pulseLength = 0;
	pin = gpioPin;
	memset(pulseLengthArray, 0, sizeof(pulseLengthArray));
}


void Meter::begin() {
	pinMode(pin, INPUT_PULLUP);

	cli();
    attachInterrupt(pin, highInterrupt, RISING);	
	sei();

	measurementTimer = millis64();

	reset();	
}


void Meter::highInterrupt() {
	counter0++;
	counter1++;
	counter2++;
}

uint64_t Meter::counter0;
uint64_t Meter::counter1;
unsigned int Meter::counter2;


String Meter::getUpdate() {
	uint64_t m = millis64();
	String ret = "[";
	ret+=uint64ToString(counter0) + ",";
	ret+=uint64ToString(counter1) + ",";
	ret+=uint64ToString(m/1000) + ",";
	ret+=uint64ToString((m-time1)/1000) + ",";
	ret+=String(pulseLength) + ",";
	ret+=String(arrayIndex+1) + "]";
	return ret;
}

String Meter::getPulseArray() {
	String ret = "[";
	if(arrayIndex==0) return "[]";

	char buf[12];
	for(int i=arrayIndex-1; i>=0;i--) {
		ret += String(pulseLengthArray[i]);
		ret += ",";
	}
	ret[ret.length()-1] = ']';
	return ret;
}

void Meter::run() {
	uint64_t m = millis64();
	if(m>=(measurementTimer+MEASUREMENT_TIME_S*1000)) {
		cli();
			if(counter2!=0) {
				pulseLength = (m-measurementTimer)/counter2;
				counter2=0;
			} else {
				pulseLength=0;
			}
		sei();

		measurementTimer = m;

		for(int i=PULSEARRAY_SIZE-1; i>0;i--) {
			pulseLengthArray[i] = pulseLengthArray[i-1];
		}
		pulseLengthArray[0] = pulseLength;

		if(arrayIndex<PULSEARRAY_SIZE) {
			arrayIndex++;
		}
	}
}

void Meter::reset() {
	cli();
	counter1=0;
	sei();
	time1 = millis64();
	arrayIndex = 0;
}
