#include <os.h>

#define NSP_ADDRVAL(addr) *(volatile unsigned *)(addr)

/* Clock is 32768/16 = 2048 Hz, we want 1024 Hz */
#define NSP_TIMER_GET_VALUE()		(NSP_ADDRVAL(0x900C0004) >> 1)
#define NSP_TIMER_SET_VALUE(value)	(NSP_ADDRVAL(0x900C0000) = value)
#define NSP_TIMER_START()	NSP_TIMER_SET_VALUE(0xFFFFFFFF); \
				NSP_ADDRVAL(0x900C0008) |= 1 << 7
#define NSP_TIMER_STOP()	NSP_ADDRVAL(0x900C0008) &= ~(1 << 7)
#define NSP_TIMER_RESET()	NSP_TIMER_SET_VALUE(0); \
				NSP_ADDRVAL(0x900C0008) = 0x66; \
				NSP_TIMER_STOP()
#define NSP_TIMER_INIT()	NSP_ADDRVAL(0x900B0018) &= ~(1 << 11); \
				NSP_TIMER_RESET()

int main(void) {
#if 0
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
#endif
	*(volatile unsigned *)0x900B0018 &= ~(1 << 11);
	volatile unsigned *load = (unsigned *)0x900C0000;
	volatile unsigned *value = (unsigned *)0x900C0004;
	volatile unsigned *control = (unsigned *)0x900C0008;
	*load = 0;
	*control = 0b01100010;
	*load = 0xFFFFFFFF;
	*control |= 1 << 7;
	printf("%u\n", *value);
	sleep(1234);
	printf("%u\n", *value);
#if 0
	printf("%u\n", NSP_TIMER_GET_VALUE());
	start = NSP_TIMER_GET_VALUE();
	for(i = 0; i < 10; ++i) {
		printf("timer: %u\n", NSP_TIMER_GET_VALUE());
		sleep(100);
	}
	sleep(3234);
	end = NSP_TIMER_GET_VALUE();
	printf("start: %x, end: %x, diff: %d\n", start, end, (start-end));
#endif
	//NSP_TIMER_RESET();
	return 0;
}