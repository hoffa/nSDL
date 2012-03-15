#include <os.h>

static volatile unsigned *value = (unsigned *)0x900C000C;
static volatile unsigned *control = (unsigned *)0x900C0014;

int main(void) {
	int i;
	*(volatile unsigned *)0x900B0018 &= ~(1 << 11);
	*(volatile unsigned *)0x900C0080 = 0xA;
	*control = 0b10000;
	*(volatile unsigned *)0x900C0010 = 32;
	*value = 0;
	*control = 0b01111;
	unsigned start = *value;
	for(i = 0; i < 1000; ++i) {
		printf("timer: %u\n", *value);
		sleep(10);
	}
	printf("diff: %u\n", (*value - start));
	return 0;
}