#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int main()
{
       
       mkdir("host:testDir", 777);

       while (1) {
              printf("Alive!!\n");
       }

       return 0;
}
