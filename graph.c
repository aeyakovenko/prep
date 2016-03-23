#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include "heap.h"

struct node;
struct edge {
   struct node *n;
   unsigned weight;
};
struct node {
   int id;
   struct edge *edges;
   int nedges;
   int nsize;
};

struct dist {
   int visited;
   int distance;
   const struct node *prev;
};

void connect(struct node *a, unsigned weight, struct node *b) {
   if(a->nedges == a->nsize) {
      int size = (a->nsize + 1)*2;
      a->edges = realloc(a->edges, sizeof(a->edges[0])*size);
      assert(a->edges);
      memset(&a->edges[a->nsize], 0, (size - a->nsize) * sizeof(a->edges[0]));
      a->nsize = size;
   }
   a->edges[a->nedges].weight = weight;
   a->edges[a->nedges].n = b;
   a->nedges++;
}

void node_dtor(struct node *a) {
   if(a->nsize) {
      free(a->edges);
   }
   a->nsize = 0;
   a->nedges = 0;
   a->id = -1;
}

void bruteforce_distance(struct dist *ds, const struct node *s, int depth) {
   int i;
   struct dist *sd = &ds[s->id];
   if(depth == 0) {
      return;
   }
   sd->visited = 1;
   for(i = 0; i < s->nedges; ++i) {
      struct edge *e = &s->edges[i];
      struct dist *nd = &ds[e->n->id];
      if(nd->distance > sd->distance + e->weight) {
         nd->prev = s;
         nd->distance = sd->distance + e->weight;
      }
   }
   depth--;
   for(i = 0; i < s->nedges; ++i) {
      bruteforce_distance(ds, s->edges[i].n, depth);
   }
}
int compare(void *ctx, void *a, void *b) {
   struct dist *sd = (struct dist*)ctx;
   int ixa = (int)(uintptr_t)a;
   int ixb = (int)(uintptr_t)b;
   return sd[ixa].distance - sd[ixb].distance;
}
void dijkstras_distance(struct heap *h, struct dist *ds, struct node *nodes) {
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
      struct edge *e = &s->edges[i];
      struct node *n = e->n;
      struct dist *nd = &ds[n->id];
      if(!nd->visited && nd->distance > sd->distance + e->weight) {
         nd->prev = s;
         nd->distance = sd->distance + e->weight;
         heap_insert(h, (void*)(uintptr_t)n->id);
      }
   }
   dijkstras_distance(h, ds, nodes);
}

void test_dijkstras(void) {
   int i,a,b,w;
   struct node nodes[10];
   struct dist brute[10];
   struct dist dijks[10];
   struct heap h;
   void *data[10];
   h.data = data;
   h.size = 10;
   h.elems = 0;
   h.ctx = dijks;
   h.pfn = compare;
   memset(nodes, 0, sizeof(nodes)); 
   memset(brute, 0, sizeof(brute)); 
   memset(dijks, 0, sizeof(dijks)); 

   for(i = 0; i < 10; ++i) {
      nodes[i].id = i;
   }
   for(i = 0; i < 10; ++i) {
      brute[i].distance = INT_MAX;
      dijks[i].distance = INT_MAX;
   }

   for(i = 0; i < 50; ++i) {
      a = rand() % 10;
      b = rand() % 10;
      w = rand() % 10 + 1;
      connect(&nodes[a], w, &nodes[b]);
   }
   brute[0].distance = 0;
   brute[0].prev = &nodes[0];
   bruteforce_distance(brute, &nodes[0], 10);

   dijks[0].distance = 0;
   dijks[0].prev = &nodes[0];
   heap_insert(&h, (void*)(uintptr_t)0);
   dijkstras_distance(&h, dijks, nodes);
   for(i = 0; i < 10; ++i) {
      assert(brute[i].distance == dijks[i].distance);
      assert(brute[i].visited == dijks[i].visited);
   }
   for(i = 0; i < 10; ++i) {
      node_dtor(&nodes[i]);
   }
}

void prims_msp_v2(struct node *nodes, struct node *msp, int sz) {
   int i;
   struct node *minn = 0;
   struct edge *mine = 0;
   int mind = INT_MAX; 
   for(i = 0; i < sz; ++i) {
      int k;
      struct node *n = &nodes[i];
      if(msp[i].id != i) {
         //not in the msp yet
         continue;
      }
      for(k = 0; k < n->nedges; ++k) {
         struct edge *e = &n->edges[k];
         if(msp[e->n->id].id == e->n->id) {
            //already an edge to the msp
            continue;
         }
         if(!mine || e->weight < mine->weight) {
            mine = e;
            minn = n;
         }
      }
   }
   if(minn) {
      assert(mine);
      //min is in msp
      assert(msp[minn->id].id == minn->id);
      //edge is to a node not in msp
      assert(msp[mine->n->id].id != mine->n->id);

      msp[mine->n->id].id = mine->n->id;
      connect(&msp[minn->id], mine->weight, &msp[mine->n->id]);
      prims_msp_v2(nodes, msp, sz);
   }
}
int main(void) {
   int i;
   for(i = 0; i < 5; ++i) { 
      test_dijkstras();
   }
}
