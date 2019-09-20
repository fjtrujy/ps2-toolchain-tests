#include <tamtypes.h>
#include <kernel.h>
#include <sifrpc.h>
#include <sbv_patches.h>
#include <iopcontrol.h>
#include <stdio.h>
#include <time.h>

int main()
{
   unsigned long long currentTime;
   int times;
   
   SifInitRpc(0);
#if !defined(DEBUG)
   /* Comment this line if you don't wanna debug the output */
   while(!SifIopReset(NULL, 0)){};
#endif


   while(!SifIopSync()){};
   SifInitRpc(0);
   sbv_patch_enable_lmb();

   printf("Hello, world!\n");

   while (1)
   {
      /* code */
      if (times == 100000000) {
         currentTime = clock();
         printf("Current time %llu\n", currentTime);
         times = 0;
      } else {
         times++;
      }
   }
   

   return 0;
}