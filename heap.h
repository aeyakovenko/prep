#ifndef HEAP_H
#define HEAP_H
struct heap {
   void *data;
   int size;
   int elems;
   void *ctx;
   int (*pfn)(void *ctx, void *a, void *b); 
};

void heap_insert(struct heap *h, void *val);

void* heap_remove(struct heap *h);

#endif //HEAP_H
