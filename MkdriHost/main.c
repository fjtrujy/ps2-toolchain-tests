#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int main()
{
       struct stat buf, buf2, buf3, buf4;
       printf("Before 1 stat\n");
       stat("host:testDir", &buf);
       printf("Before 2 stat\n");
       stat("host:testDir/hola.txt", &buf2);
       printf("Before 3 stat\n");
       stat("host:/testDir", &buf3);
       printf("Before 24stat\n");
       stat("host:/testDir/hola.txt", &buf4);

       int is_dir = S_ISDIR(buf.st_mode);
       int is_dir2 = S_ISDIR(buf2.st_mode);
       int is_dir4 = S_ISDIR(buf3.st_mode);
       int is_dir3 = S_ISDIR(buf4.st_mode);
       while (1) {
              printf("host:testDir is a dir %i\n", is_dir);
              printf("host:testDir/hola.txt is a dir %i\n", is_dir2);
              printf("host:/testDir is a dir %i\n", is_dir3);
              printf("host:/testDir/hola.txt is a dir %i\n", is_dir4);
       }
       

       return 0;
}

