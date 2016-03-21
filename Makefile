all:heap_u graph
	./heap_u
	./graph

heap_u:heap.c
graph:heap.c

CFLAGS+=-g

.PHONY:all
