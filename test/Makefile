CC=gcc
CFLAGS= -std=c99 -Wall -Wextra -Werror -g -no-pie
OBJS= examine test

all: $(OBJS)

$(OBJS): %: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	$(RM) $(OBJS)

.PHONY: all clean
