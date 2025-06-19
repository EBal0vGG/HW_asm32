CFLAGS ?= -O0 -g
CFLAGS += -std=gnu99
CFLAGS += -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self \
  -Wswitch-default -Wpointer-arith -Wtype-limits -Wempty-body \
  -Wstrict-prototypes -Wold-style-declaration -Wold-style-definition \
  -Wmissing-parameter-type -Wmissing-field-initializers -Wnested-externs \
  -Wstack-usage=4096 -Wfloat-equal -Wabsolute-value
CFLAGS += -fsanitize=undefined -fsanitize-undefined-trap-on-error

CC += -m32 -no-pie -fno-pie
LDLIBS = -lm


OBJS = BuildDir/main.o BuildDir/parsing.o BuildDir/root.o BuildDir/func.o BuildDir/test_func.o
OBJS1 = BuildDir/tests.o BuildDir/root.o BuildDir/func.o BuildDir/test_func.o
TARGET = integral

all: $(TARGET)

$(TARGET): make_dir $(OBJS)
	$(CC) -m32 -o $(TARGET) $(OBJS) $(LDLIBS)

make_dir:
	mkdir -p BuildDir

build_test: make_dir $(OBJS1)
	$(CC) -m32 -o tests $(OBJS1) $(LDLIBS)

test: build_test
	./tests

# Object files from .asm
BuildDir/%.o: %.asm
	nasm -g -f elf32 -o $@ $<
#Link all object files with gcc
BuildDir/%.o: %.c
	$(CC) -m32 -c $(CFLAGS) -o $@ $<

.PHONY: clean

clean:
	rm -rf BuildDir
	rm -f $(TARGET)
