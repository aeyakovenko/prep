#include <assert.h>
#include "heap.h"
void swap(void **a, void **b) {
   void *tmp = *a;
   *a = *b;
   *b = tmp;
}

//     0
//    1 2
//  3 4 5 6
void bubble_up(struct heap *h, int pos) {
   int root;
   root = ((pos  + 1 ) / 2) - 1;
   if(root < 0)
      return;
   if(h->pfn(h->ctx,
             h->data[pos],
             h->data[root]) < 0) {
      swap(&h->data[pos],  &h->data[root]);
      bubble_up(h, root);
   }
}

void heap_insert(struct heap *h, void *val) { 
   assert(h->size > h->elems);
   h->data[h->elems++] = val;
   bubble_up(h, h->elems-1);
}

void bubble_down(struct heap *h, int root) {
   int right = (root + 1)*2, left = right - 1;
   int ixes[] = {left, right};
   int i;
   int min = root;
   for(i = 0; i < 2 && ixes[i] < h->elems; ++i) {
      if(h->pfn(h->ctx,
                h->data[ixes[i]],
                h->data[min]) < 0) {
         min = ixes[i];
      }
   }
   if(min != root) {
      swap(&h->data[min],  &h->data[root]);
      bubble_down(h, min);
   }
}

void* heap_remove(struct heap *h) { 
   void *rv;
   assert(h->elems > 0);
   rv = h->data[0];
   h->data[0] = h->data[h->elems - 1];
   h->elems--;
   bubble_down(h, 0);
   return rv;
}


