CC=gcc
LD=ld

PREFIX?=./pre

INCS+=-Iinclude
LIBS+=-lGL -lglfw -lGLEW -lm

CPPFLAGS?=
CPPFLAGS+=$(INCS)

CFLAGS?=-O2 -g
CFLAGS+=-Wall -Wextra -MD -std=c99 -D_POSIX_C_SOURCE=200809L -D_XOPEN_SOURCE=600

LDFLAGS?=
LDFLAGS+=$(LIBS)

SRC_C=$(wildcard src/*.c src/**/*.c)
SRC_MK=$(SRC_C:.c=_c.d)
OBJ=$(SRC_C:.c=_c.o)

SHAREDIR=share
SHARE=$(wildcard $(SHAREDIR)/tayracer/* $(SHAREDIR)/tayracer/**/*)

BIN=tayracer
BUILDFILES=$(OBJ) $(SRC_MK) $(SRC)

all: $(BIN) $(SHARE)

clean:
	rm -f $(BUILDFILES)

%_c.o: %.c
	@echo "CC	$(shell basename $@)"
	@$(CC) -o $@ -c $< $(CPPFLAGS) $(CFLAGS)

$(BIN): $(OBJ)
	@echo "LD	$(shell basename $@)"
	@$(CC) -o $@ $(CFLAGS) $^ $(LDFLAGS)

install: $(BIN) $(SHARE)
	@echo "Installing to $(PREFIX)"
	@mkdir -p $(PREFIX)/bin
	@mkdir -p $(PREFIX)/share/tayracer
	@cp $(BIN) $(PREFIX)/bin
	@cp -r $(SHAREDIR) $(PREFIX)

uninstall:
	@echo "Uninstalling from $(PREFIX)"
	@rm -f $(PREFIX)/bin/$(BIN)
	@rm -fr $(PREFIX)/share/tayracer

-include $(SRC_MK)

.PHONY: all clean install uninstall
