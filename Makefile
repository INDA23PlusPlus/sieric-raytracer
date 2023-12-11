CC=gcc
LD=ld

INCS+=-Iinclude
LIBS+=-lGL -lglfw -lGLEW -lm

CPPFLAGS?=
CPPFLAGS+=$(INCS)

CFLAGS?=-O2 -g
CFLAGS+=-Wall -Wextra -MD -std=c99

LDFLAGS?=
LDFLAGS+=$(LIBS)

SRC_C=$(wildcard src/*.c src/**/*.c)
SRC_MK=$(SRC_C:.c=_c.d)
OBJ=$(SRC_C:.c=_c.o)

BIN=tayracer
BUILDFILES=$(OBJ) $(SRC_MK) $(SRC)

all: $(BIN)

clean:
	rm -f $(BUILDFILES)

%_c.o: %.c
	@echo "CC	$(shell basename $@)"
	@$(CC) -o $@ -c $< $(CPPFLAGS) $(CFLAGS)

$(BIN): $(OBJ)
	@echo "LD	$(shell basename $@)"
	@$(CC) -o $@ $(CFLAGS) $^ $(LDFLAGS)

-include $(SRC_MK)

.PHONY: all clean
