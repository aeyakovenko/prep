#include <stdio.h>
#include <assert.h>
void prefix_sum(int *buf, int *tmp, int logsize) {
   int d, k;
   int *swap;
   int size = 1<<logsize;
   for(d = 0; d < logsize; ++d) {
      for(k = 0; k < size; ++k) {
         //in parallel
         //printf("size sz:%d k:%d\n", size, k);
         if(k - (1<<d) < 0) {
            tmp[k] = buf[k];
         } else {
            tmp[k] = buf[k] + buf[k - (1<<d)];
         }
      }
      swap = buf;
      buf = tmp;
      tmp = swap;
   }
}

int main(void) {
   int i;
   int buf[64];
   int tmp[64];
   for(i = 0; i < 64; ++i) {
      buf[i] = i;
   }
   prefix_sum(buf, tmp, 6);
   for(i = 0; i < 64; ++i) {
      assert(buf[i] ==  (i * (i + 1)) / 2);
      //printf("buf:%d expecting:%d\n", buf[i], (i * (i + 1)) / 2);
   }
   return 0;
}
