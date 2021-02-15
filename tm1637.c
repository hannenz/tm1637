#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "tm1637.h"


tm1637_settings settings;

char current_data[] = { 0, 0, 0, 0 };

char hex_digits[] = {
	0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d,
	0x07, 0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71
};



int tm1637_init(int clock_pin, int data_pin, bool colon, double brightness) {
	if (gpioInitialise() < 0) {
		return -1;
	}

	settings.clock_pin = clock_pin;
	settings.data_pin = data_pin;
	settings.colon = colon;
	settings.brightness = brightness;

	gpioSetMode(settings.clock_pin, PI_OUTPUT);
	gpioSetMode(settings.data_pin, PI_OUTPUT);

	return 0;
}



void tm1637_clear() {
	double b;
	int c;
	b = settings.brightness;
	c = settings.colon;
	const char data[] = { 0x7f, 0x7f, 0x7f, 0x7f };
	tm1637_show(data);
	settings.brightness = b;
	settings.colon = c;
}



void tm1637_show_int(unsigned int n) {
	if (n > 9999) {
		return;
	}

	int digit, pos = 3;

	do {
		digit = n % 10;
		tm1637_show_1(pos, digit);
		pos--;
		n /= 10;
	} while (n  > 0 && pos >= 0); 
}



void tm1637_show(const char *data) {
	int i;
	for (i = 0; i < 4; i++) {
		current_data[i] = data[i];
	}

	tm1637_start();
	tm1637_write_byte(ADDR_AUTO);
	tm1637_br();
	tm1637_write_byte(STARTADDR);

	for (i = 0; i < 4; i++) {
		tm1637_write_byte(tm1637_encode(data[i]));
	}

	tm1637_br();
	tm1637_write_byte(0x88 + (int)settings.brightness);
	tm1637_stop();
}



void tm1637_show_1(int pos, int digit) {
	if (pos < 0 || pos > 3) {
		return;
	}

	current_data[pos] = digit;
	tm1637_start();
	tm1637_write_byte(ADDR_FIXED);
	tm1637_br();
	tm1637_write_byte(STARTADDR | pos);
	tm1637_write_byte(tm1637_encode(digit));
	tm1637_br();
	tm1637_write_byte(0x88 + (int)settings.brightness);
	tm1637_stop();

}



const char tm1637_encode(const char data) {

	char point_data;

	point_data = settings.colon ? 0x80 : 0x00;

	return (data == 0x7f)
		? 0
		: hex_digits[(unsigned int)data] + point_data;
}



void tm1637_set_brightness(double percent) {
	double max_brightness = 7.0;
	double brightness = ceil(max_brightness * percent);

	if (brightness < 0) {
		brightness = 0;
	}

	if (settings.brightness != brightness) {
		settings.brightness = brightness;
		tm1637_show(current_data);
	}
}



void tm1637_write_byte(const char data) {
	int i;
	char byte = data;
	for (i = 0; i < 8; i++) {
		gpioWrite(settings.clock_pin, 0);
		if (byte & 0x01) {
			gpioWrite(settings.data_pin, 1);
		}
		else {
			gpioWrite(settings.data_pin, 0);
		}
		byte >>= 1;
		gpioWrite(settings.clock_pin, 1);
	}

	// Wait for ACK
	gpioWrite(settings.clock_pin, 0);
	gpioWrite(settings.data_pin, 1);
	gpioWrite(settings.clock_pin, 1);
	gpioSetMode(settings.data_pin, PI_INPUT);

	while (gpioRead(settings.data_pin)) {
		gpioDelay(1000);
		if (gpioRead(settings.data_pin)) {
			gpioSetMode(settings.data_pin, PI_OUTPUT);
			gpioWrite(settings.data_pin, 0);
			gpioSetMode(settings.data_pin, PI_INPUT);
		}
	}
	gpioSetMode(settings.data_pin, PI_OUTPUT);
}



void tm1637_start() {
	gpioWrite(settings.clock_pin, 1);
	gpioWrite(settings.data_pin, 1);
	gpioWrite(settings.data_pin, 0);
	gpioWrite(settings.clock_pin, 0);
}



void tm1637_stop() {
	gpioWrite(settings.clock_pin, 0);
	gpioWrite(settings.data_pin, 0);
	gpioWrite(settings.clock_pin, 1);
	gpioWrite(settings.data_pin, 1);
}



void tm1637_br() {
	tm1637_stop();
	tm1637_start();
}



void tm1637_cleanup() {
	gpioTerminate();
}


