#include <os.h>

#define NSP_ADDRVAL(addr) *(volatile unsigned *)(addr)

#define BASE 0x900c0000

#define NSP_TIMER_GET_VALUE()		NSP_ADDRVAL(BASE + 0x4)
#define NSP_TIMER_SET_VALUE(value)	NSP_ADDRVAL(BASE) = value
#define NSP_TIMER_START()	NSP_TIMER_SET_VALUE(0xffffffff); \
				NSP_ADDRVAL(BASE + 0x8) |= 1 << 7
#define NSP_TIMER_STOP()	NSP_ADDRVAL(BASE + 0x8) &= ~(1 << 7)
#define NSP_TIMER_RESET()	NSP_TIMER_SET_VALUE(0); \
				NSP_ADDRVAL(BASE + 0x08) = 0x46; \
				NSP_TIMER_STOP()
#define NSP_TIMER_INIT()	NSP_ADDRVAL(0x900b0018) &= ~(1 << 11); \
				NSP_ADDRVAL(0x900c0080) = 0xa; \
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
	/* SOME ISSUES WITH DIS SHIT */
	int i;
	unsigned int start, end;

	NSP_TIMER_INIT();
	NSP_TIMER_START();
	sleep(1000);
	start = NSP_TIMER_GET_VALUE();
	for(i = 0; i < 10; ++i) {
		unsigned int timer = NSP_TIMER_GET_VALUE();
		printf("timer: %u\n", timer);
		sleep(100);
	}
	end = NSP_TIMER_GET_VALUE();
	printf("start: %x, end: %x, diff: %d\n", start, end, (start-end));
	NSP_TIMER_RESET();
	return 0;
}