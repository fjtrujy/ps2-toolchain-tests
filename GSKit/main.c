#include <tamtypes.h>
#include <kernel.h>
#include <sifrpc.h>
#include <sbv_patches.h>
#include <iopcontrol.h>
#include <stdio.h>

int main()
{
   SifInitRpc(0);
#if !defined(DEBUG)
   /* Comment this line if you don't wanna debug the output */
   while(!SifIopReset(NULL, 0)){};
#endif

   while(!SifIopSync()){};
   SifInitRpc(0);

   printf("Hello, world!\n");

   while (1)
   {
      /* code */
      printf("Hello, world!\n");
   }
   

   return 0;
}
