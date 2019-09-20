#include <tamtypes.h>
#include <kernel.h>
#include <sifrpc.h>
#include <sbv_patches.h>
#include <iopcontrol.h>
#include <stdio.h>

int main()
{
   int times;
   volatile float hello;
   SifInitRpc(0);
#if !defined(DEBUG)
   /* Comment this line if you don't wanna debug the output */
   while(!SifIopReset(NULL, 0)){};
#endif

   while(!SifIopSync()){};
   SifInitRpc(0);
   sbv_patch_enable_lmb();

   printf("Hello, world!\n");
   hello = 105.03f;

   if (hello == 105.03f) {
      printf("Float numbers are working fine\n");
   }

   printf("Hello, world! %f\n", hello);

   times = 0;
   while (1)
   {
      /* code */
      if (times == 100000000) {
         printf("Still alive\n");
         times = 0;
      } else {
         times++;
      }
   }
   

   return 0;
}
