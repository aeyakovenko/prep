#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>

struct heap {
   int *data;
   int size;
   int elems;
};
void swap(int *a, int *b) {
   int tmp = *a;
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
   if(h->data[pos] < h->data[root]) {
      swap(&h->data[pos],  &h->data[root]);
      bubble_up(h, root);
   }
}

void heap_insert(struct heap *h, int val) { 
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
      if(h->data[ixes[i]] < h->data[min]) {
         min = ixes[i];
      }
   }
   if(min != root) {
      swap(&h->data[min],  &h->data[root]);
      bubble_down(h, min);
   }
}

int heap_remove(struct heap *h) { 
   int rv;
   assert(h->elems > 0);
   rv = h->data[0];
   h->data[0] = h->data[h->elems - 1];
   h->elems--;
   bubble_down(h, 0);
   return rv;
}

int main(void) {
   int prev, i,j;
   int data[10];
   struct heap h;
   h.size = 10;
   h.data = data;
   h.elems = 0;
   for(j = 0; j < 10; ++j) {
      printf("starting...\n");
      memset(data,0,sizeof(data));
      assert(h.elems == 0);
      for(i = 0; i < 10; ++i) {
         int v = rand();
         printf("\tadding %d\n", v);
         heap_insert(&h, v);
      }
      assert(h.elems == 10);
      prev = h.data[0];
      for(i = 0; i < 10; ++i) {
         int rv = heap_remove(&h); 
         printf("%d\n", rv);
         assert(prev <= rv);
         prev = rv;
      }
   }
   return 0;
}
