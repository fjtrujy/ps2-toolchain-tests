#include <tamtypes.h>
#include <kernel.h>
#include <sifrpc.h>
#include <sbv_patches.h>
#include <iopcontrol.h>
#include <stdio.h>

char * initaddr;
int main(int argc, char *argv[])
{
   char dummy;  // Note -- first variable declared.
   initaddr = &dummy;

   printf("First address %i\n", &dummy);

      SifInitRpc(0);
#if !defined(DEBUG)
   /* Comment this line if you don't wanna debug the output */
   // while(!SifIopReset(NULL, 0)){};
#endif

   while(!SifIopSync()){};
   SifInitRpc(0);

   printf("Hello, world!\n");

   something(0);

   while (1)
   {
      /* code */
      printf("Hello, world!\n");
   }
}

int something(int value) {
   printf("Something %i\n", value);
   print_stacksize();
   return value + something(value+1);
}

int stacksize(void)
{
   char dummy2, *lastptr;
   lastptr = &dummy2;

   return(initaddr-lastptr); // This will give the stacksize at the instant of the return function.
}

void print_stacksize(void)
{
   printf("The stack size is %i\n", stacksize());
}
