#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "heap.h"

int ints(void *ctx, void *a, void *b) {
   return (int)(uintptr_t)a - (int)(uintptr_t)b;
}
int main(void) {
   int prev, i,j;
   void *data[10];
   struct heap h;
   h.size = 10;
   h.data = data;
   h.elems = 0;
   h.pfn = ints;
   h.ctx = 0;
   for(j = 0; j < 10; ++j) {
      //printf("starting...\n");
      memset(data,0,sizeof(data));
      assert(h.elems == 0);
      for(i = 0; i < 10; ++i) {
         int v = rand();
         //printf("\tadding %d\n", v);
         heap_insert(&h, (void*)(uintptr_t)v);
      }
      assert(h.elems == 10);
      prev = (int)(uintptr_t)h.data[0];
      for(i = 0; i < 10; ++i) {
         int rv = (int)(uintptr_t)heap_remove(&h); 
         //printf("%d\n", rv);
         assert(prev <= rv);
         prev = rv;
      }
   }
   return 0;
}
