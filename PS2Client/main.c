#include <stdio.h>
#include <stdlib.h>
#include <tamtypes.h>
#include <kernel.h>
#include <sifrpc.h>

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

int main(int argc, char *argv[])
{
   int i;
   
   for (i = 0; i < argc; ++i) {
        printf("Argument #%d is %s\n", i, argv[i]);
    }

    printf("Maybe crash\n");
    printf("******** %s *********\n", argv[0]);

   loop();
   
   return 0;
}
