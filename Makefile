OUT = nanotime
SRCDIR = src
OBJDIR = out
SRC = $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))

CFLAGS = -Wall -Wextra -Wshadow -Wundef -Wconversion -Wpedantic -Werror=return-type -std=c11 -march=native
LFLAGS =

.PHONY: all debug install clean

all: $(OUT)

$(OUT): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c -o $@ $< -O3

$(OBJDIR):
	mkdir -p $@

# Always rebuild everything when debugging
debug: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LFLAGS) -ggdb

install: $(OUT)
	cp $(OUT) /usr/local/bin/$(OUT)

clean:
	rm -rf $(OUT) $(OBJS)
