CC = nspire-gcc
LD = nspire-ld
OBJCOPY = arm-none-eabi-objcopy
CFLAGS = -Wall -Wextra -nostdlib -I../../include -O3
LDFLAGS =
ifneq ($(findstring cx8,$(MAKECMDGOALS)),)
	CFLAGS += -D NSP_CX_8BIT
else
	ifneq ($(findstring cx,$(MAKECMDGOALS)),)
		CFLAGS += -D NSP_CX_16BIT
	endif
endif

SOURCES = tinspire_test.c
OBJECTS = tinspire_test.o
LIBS = ../../libSDL.a libm.a
TARGET = sdl_test.tns

cx: all
cx8: all
tc: all
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(LD) $(LDFLAGS) $^ $(LIBS) -o $(@:.tns=.elf)
	$(OBJCOPY) -O binary $(@:.tns=.elf) $(TARGET)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) *.elf $(TARGET)