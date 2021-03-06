#include <tamtypes.h>
#include <kernel.h>
#include <sifrpc.h>
#include <sbv_patches.h>
#include <iopcontrol.h>
#include <stdio.h>
#include <fileXio_rpc.h>
#include <loadfile.h>

extern unsigned char fileXio_irx;
extern unsigned int size_fileXio_irx;

extern unsigned char freesio2_irx;
extern unsigned int size_freesio2_irx;

extern unsigned char iomanX_irx;
extern unsigned int size_iomanX_irx;

#define O_RDONLY        0x0000          /* open for reading only */

int main()
{
   int times;
   int fd, size;
	char *FileBuffer;

   SifInitRpc(0);
   /* Comment this line if you don't wanna debug the output */
   // while(!SifIopReset(NULL, 0)){};

   while(!SifIopSync()){};
   SifInitRpc(0);
   sbv_patch_enable_lmb();

   printf("Hello, start the program!\n");


   /* I/O Files, Load Driver */
   int rett;
   rett = SifExecModuleBuffer(&iomanX_irx, size_iomanX_irx, 0, NULL, NULL);
   printf("SifExecModuleBuffer: iomanX_irx %i\n", rett);

   rett = SifExecModuleBuffer(&fileXio_irx, size_fileXio_irx, 0, NULL, NULL);
   printf("SifExecModuleBuffer: fileXio_irx %i\n", rett);

   rett = SifExecModuleBuffer(&freesio2_irx, size_freesio2_irx, 0, NULL, NULL);
   printf("SifExecModuleBuffer: freesio2_irx %i\n", rett);

   printf("Before init fileXio\n");
   if (fileXioInit() < 0) {
      printf("Error: fileXioInit library not initalizated\n");
   }

   printf("After init fileXio\n");


	if((fd=fileXioOpen("host:hello.txt", O_RDONLY))>=0){
		size=fileXioLseek(fd, 0, SEEK_END);
		fileXioLseek(fd, 0, SEEK_SET);
		if((FileBuffer=malloc(size))!=NULL){
			if(fileXioRead(fd, FileBuffer, size)==size){
            printf("OK!!!!\n");
         } else {
            printf("ERROR: fileXioRead\n");
         }
      } else {
         printf("ERROR DURING ALLOC\n");
      }
   } else {
      printf("ERROR opening File\n");
   }

   fileXioClose(fd);

   printf("Hello, world!\n");

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
