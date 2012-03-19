CC = nspire-gcc
LD = nspire-ld
OBJCOPY = arm-none-eabi-objcopy
CFLAGS = -Wall -Wextra -I../include
LDFLAGS =

SOURCES = tinspire_test.c
OBJECTS = tinspire_test.o
LIBS = ../libSDL.a
TARGET = sdl_test.tns

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(LD) $(LDFLAGS) $^ $(LIBS) -o $(@:.tns=.elf)
	$(OBJCOPY) -O binary $(@:.tns=.elf) $(TARGET)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) *.elf