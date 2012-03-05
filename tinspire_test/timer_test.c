#include <os.h>

int main(void) {
	int i;
	/* enable timer access */
	*(volatile unsigned int *)0x900b0018 &= ~(1 << 11);
	/* set timer divider */
	*(volatile unsigned int *)0x900c0010 = 3276; /* set timer to ~10Hz */
	printf("divider: %d\n", *(volatile unsigned int *)0x900c0010);
	/* start timer */
	*(volatile unsigned int *)0x900c0014 = 0xf;
	for(i = 0; i < 10; ++i) {
		int timer = *(volatile unsigned int *)0x900c000c;
		int ms = 100 * timer;
		printf("%d\t%d\n", timer, ms);
		sleep(100);
	}
	return 0;
}