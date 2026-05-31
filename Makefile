CC       := gcc
CFLAGS   := -Wall -Wextra -Wpedantic -Werror -std=c99
INCLUDES := -Iext/raylib/src/ -Isrc/
LDFLAGS  := -Lext/raylib/src/
LIBS     := -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

TARGET   := vinora
SRC_DIR  := src
SOURCES := $(shell find $(SRC_DIR) -type f -name "*.c")
OBJECTS  := $(SOURCES:.c=.o)

RAYLIB_DIR := ext/raylib/src

all: raylib $(TARGET)

raylib:
	@$(MAKE) -C $(RAYLIB_DIR) PLATFORM=PLATFORM_DESKTOP -j2

$(TARGET): $(OBJECTS) | raylib
	$(CC) $^ $(LDFLAGS) $(LIBS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)

nuke: clean
	@$(MAKE) -C $(RAYLIB_DIR) clean

rebuild: clean all

style-check:
	clang-format -n $(SOURCES)

.PHONY: all raylib run clean clean-all rebuild
