# GNU/Linux + X11, for now. Other platforms: patches welcome.
CC       := gcc
CFLAGS   := -Wall -Wextra -Wpedantic -Werror -std=c99 -g -O0
INCLUDES := -Iext/raylib/src/ -Isrc/
LDFLAGS  := -Lext/raylib/src/
LIBS     := -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

TARGET   := vinora
DUMP     := vnrs_parser
SRC_DIR  := src

SOURCES  := $(shell find $(SRC_DIR) -type f -name '*.c' | sort)
HEADERS  := $(shell find $(SRC_DIR) -type f -name '*.h' | sort)
OBJECTS  := $(SOURCES:.c=.o)

RAYLIB_DIR := ext/raylib/src

# Extra words after `run' are vinora's argv. Make steals flags that
# look like its own (`-h'), so put `--' in front of those:
#   make run
#   make run assets/hamlet.vnrs
#   make -- run -h
#   make -- run --help
ifeq ($(firstword $(MAKECMDGOALS)),run)
    RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
    $(foreach a,$(RUN_ARGS),$(eval $(a):;@:))
endif

.PHONY: all raylib run clean nuke rebuild style-check help test

all: raylib $(TARGET) $(DUMP)

help:
	@echo "make              build raylib, $(TARGET), and $(DUMP)"
	@echo "make run          build and run (extra words -> argv)"
	@echo "make vnrs_parser  build the scene dump on its own"
	@echo "make test         run tests/vnrs"
	@echo "make clean        remove engine objects and binaries"
	@echo "make nuke         also clean the bundled raylib"
	@echo "make rebuild      clean, then build"
	@echo "make style-check  clang-format dry run"

raylib:
	@$(MAKE) -C $(RAYLIB_DIR) PLATFORM=PLATFORM_DESKTOP -j2

$(TARGET): $(OBJECTS) | raylib
	$(CC) $^ $(LDFLAGS) $(LIBS) -o $@

$(DUMP): vnrs_parser.o src/vnrs/parser.o
	$(CC) $^ -o $@

# A header change can resize a struct. Rebuild every object, or a
# stale .o writes past the caller's stack.
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

run: $(TARGET)
	./$(TARGET) $(RUN_ARGS)

test: $(DUMP)
	./tests/vnrs/run.sh

clean:
	rm -f $(OBJECTS) $(TARGET) $(DUMP) vnrs_parser.o

nuke: clean
	@$(MAKE) -C $(RAYLIB_DIR) clean

rebuild: clean all

style-check:
	clang-format --dry-run --Werror $(SOURCES) $(HEADERS) vnrs_parser.c
