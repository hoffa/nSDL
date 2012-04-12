CC = nspire-gcc
LD = nspire-ld
OBJCOPY = arm-none-eabi-objcopy
CFLAGS = -Wall -Wextra -nostdlib -I../../include -O3
LDFLAGS =

ifneq ($(findstring cx8s,$(MAKECMDGOALS)),)
	CFLAGS += -D NSP_BPP_SW8_HW16 -D NSP_CX
else
	ifneq ($(findstring cx8,$(MAKECMDGOALS)),)
		CFLAGS += -D NSP_BPP_SW8_HW8 -D NSP_CX
	else
		ifneq ($(findstring cx,$(MAKECMDGOALS)),)
			CFLAGS += -D NSP_BPP_SW16_HW16 -D NSP_CX
		endif
	endif
endif

ifneq ($(findstring tc8,$(MAKECMDGOALS)),)
	CFLAGS += -D NSP_BPP_SW8_HW8 -D NSP_TC
else
	ifneq ($(findstring tc,$(MAKECMDGOALS)),)
		CFLAGS += -D NSP_BPP_SW8_HW4 -D NSP_TC
	endif
endif

SOURCES = tinspire_test.c
OBJECTS = tinspire_test.o
LIBS = ../../libSDL.a
TARGET = sdl_test.tns

cx: all
cx8: all
cx8s: all
tc: all
tc4: all
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(LD) $(LDFLAGS) $^ $(LIBS) -o $(@:.tns=.elf)
	$(OBJCOPY) -O binary $(@:.tns=.elf) $(TARGET)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET:.tns=.elf) $(TARGET)