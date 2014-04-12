CFLAGS = -g -Wall --std=c99 -O3

OBJS = \
	2048.c.o

build: 2048

2048: $(OBJS)
	$(LINK.c) -o $@ $^

%.c.o: %.c
	$(COMPILE.c) -o $@ $^

clean:
	rm -f 2048 *.o

run: 2048
	./2048