#include <tamtypes.h>
#include <kernel.h>
#include <sifrpc.h>
#include <sbv_patches.h>
#include <iopcontrol.h>
#include <stdio.h>
#include <libjpg.h>

void loop(void) {
   int times = 0;

   while (1)
   {
      /* code */
      if (times == 100000000) {
         printf("Hello, world!\n");
         times = 0;
      } else {
         times++;
      }
   }
}

int loadJPG(const char *filePath) {
   jpgData *jpg = NULL;
   int result = -199;
   FILE *file = fopen(filePath, "rb");
    if (file) {
        jpg = jpgOpenFILE(file, JPG_NORMAL);
   }
   if (jpg)
      jpgClose(jpg);

   if (file)
      fclose(file);

   return result;
}

int main(int argc, char *argv[])
{
   printf("Before anything!!\n");
   int res = loadJPG("host:SLUS_201.97_COV.jpg");
   
   printf("Res loadJPG %i\n", res);

   loop();
   

   return 0;
}
