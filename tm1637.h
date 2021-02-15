#ifndef __TM1637_H__
#define __TM1637_H__

#include <pigpio.h>
#include <stdbool.h>

#define ADDR_AUTO 0x40
#define ADDR_FIXED 0x44
#define STARTADDR 0xc0

typedef struct {
	int clock_pin;
	int data_pin;
	bool colon;
	double brightness;
} tm1637_settings;

int tm1637_init(int clock_pin, int dio, bool colon, double brightness);
void tm1637_cleanup();
void tm1637_clear();
void tm1637_show_int(unsigned int n);
void tm1637_show(const char *data);
void tm1637_show_1(int pos, int digit);
void tm1637_start();
void tm1637_stop();
void tm1637_br();
void tm1637_write_byte(const char data);
const char tm1637_encode(const char data);
void tm1637_set_brightness(double percent);
#endif
