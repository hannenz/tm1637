#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tm1637.h"

int main(int argc, char **argv) {

	int i;

	tm1637_init(3, 2, true, 1.0);
	tm1637_clear();

	for (i = 0; i < 9999; i++) {
		tm1637_show_int(i);
		gpioDelay(10000);
	}

	tm1637_cleanup();
	return 0;
}
