CC = nspire-gcc
LD = nspire-ld
OBJCOPY = arm-none-eabi-objcopy
CFLAGS = -Wall -Wextra -nostdlib -I../../include -O3
LDFLAGS =

SOURCES = link.c
OBJECTS = link.o
LIBS = ../../libSDL.a
TARGET = Link.tns

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(LD) $(LDFLAGS) $^ $(LIBS) -o $(@:.tns=.elf)
	$(OBJCOPY) -O binary $(@:.tns=.elf) $(TARGET)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET:.tns=.elf) $(TARGET)