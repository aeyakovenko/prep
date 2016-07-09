all:heap_u graph prefix
	./heap_u
	./graph
	./prefix

heap_u:heap.c
graph:heap.c

CFLAGS+=-g

.PHONY:all
