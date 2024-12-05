CC = gcc
CFLAGS = -Wall -Wextra -O2
LDFLAGS = 

SRCDIR = src
BUILDDIR = build
BINDIR = bin


TARGET = $(BINDIR)/tetris

C_SOURCES = $(wildcard $(SRCDIR)/*.c)
C_OBJECTS = $(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(C_SOURCES))

all: $(TARGET)

$(TARGET): $(C_OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) $(LDFLAGS) -o $@ $^

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILDDIR) $(BINDIR)

rebuild: clean all

debug: CFLAGS += -g
debug: LDFLAGS += -g
debug: rebuild
