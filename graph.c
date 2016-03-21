#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include "heap.h"

struct node {
   int id;
   struct node **edges;
   int nedges;
   int nsize;
};

struct dist {
   int visited;
   int distance;
   const struct node *prev;
};

void connect(struct node *a, struct node *b) {
   if(a->nedges == a->nsize) {
      int size = (a->nsize + 1)*2;
      a->edges = realloc(a->edges, sizeof(struct node*)*size);
      assert(a->edges);
      memset(&a->edges[a->nsize], 0, (size - a->nsize) * sizeof(struct node*));
      a->nsize = size;
   }
   a->edges[a->nedges++] = b;
}

void bruteforce(struct dist *ds, const struct node *s, int depth) {
   int i;
   struct dist *sd = &ds[s->id];
   if(depth == 0) {
      return;
   }
   sd->visited = 1;
   for(i = 0; i < s->nedges; ++i) {
      struct node *n = s->edges[i];
      struct dist *nd = &ds[n->id];
      if(nd->distance > sd->distance + 1) {
         nd->prev = s;
         nd->distance = sd->distance + 1;
      }
   }
   depth--;
   for(i = 0; i < s->nedges; ++i) {
      bruteforce(ds, s->edges[i], depth);
   }
}
int compare(void *ctx, void *a, void *b) {
   struct dist *sd = (struct dist*)ctx;
   int ixa = (int)(uintptr_t)a;
   int ixb = (int)(uintptr_t)b;
   return sd[ixa].distance - sd[ixb].distance;
}
void dijkstras(struct heap *h, struct dist *ds, struct node *nodes) {
   int i;
   const struct node *s;
   struct dist *sd;
   if(h->elems == 0) {
      return;
   }
   i = (int)(uintptr_t)heap_remove(h);
   s = &nodes[i];
   sd = &ds[s->id];
   sd->visited = 1;
   for(i = 0; i < s->nedges; ++i) {
      struct node *n = s->edges[i];
      struct dist *nd = &ds[n->id];
      if(!nd->visited && nd->distance > sd->distance + 1) {
         nd->prev = s;
         nd->distance = sd->distance + 1;
         heap_insert(h, (void*)(uintptr_t)n->id);
      }
   }
   dijkstras(h, ds, nodes);
}

int main(void) {
   int i,a,b;
   struct node nodes[10];
   struct dist brute[10];
   struct dist dikjs[10];
   struct heap h;
   void *data[10];
   h.data = data;
   h.size = 10;
   h.elems = 0;
   h.ctx = dikjs;
   h.pfn = compare;

   memset(nodes, 0, sizeof(nodes)); 
   memset(brute, 0, sizeof(brute)); 
   memset(dikjs, 0, sizeof(dikjs)); 

   for(i = 0; i < 10; ++i) {
      nodes[i].id = i;
   }
   for(i = 0; i < 10; ++i) {
      brute[i].distance = INT_MAX;
      dikjs[i].distance = INT_MAX;
   }

   for(i = 0; i < 50; ++i) {
      a = rand() % 10;
      b = rand() % 10;
      connect(&nodes[a], &nodes[b]);
   }
   brute[0].distance = 0;
   brute[0].prev = &nodes[0];
   bruteforce(brute, &nodes[0], 10);

   dikjs[0].distance = 0;
   dikjs[0].prev = &nodes[0];
   heap_insert(&h, (void*)(uintptr_t)0);
   dijkstras(&h, dikjs, nodes);
   assert(0 == memcmp(brute, dikjs, sizeof(brute)));
   for(i = 0; i < 10; ++i) {
      printf("brute[%d] prev:%d dist:%d\n", i, brute[i].prev->id, brute[i].distance);
   }
   for(i = 0; i < 10; ++i) {
      printf("dikjs[%d] prev:%d dist:%d\n", i, dikjs[i].prev->id, dikjs[i].distance);
   }
}
