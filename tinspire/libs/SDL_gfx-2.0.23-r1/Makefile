CC = nspire-gcc
AR = arm-none-eabi-ar
RANLIB = arm-none-eabi-ranlib
CFLAGS = -Wall -Wextra -nostdlib -Ofast

TARGET = libSDL_gfx.a
SOURCES = SDL_framerate.c SDL_gfxBlitFunc.c SDL_gfxPrimitives.c SDL_imageFilter.c SDL_rotozoom.c
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(AR) cr $@ $^
	$(RANLIB) $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)