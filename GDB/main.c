#include <tamtypes.h>
#include <kernel.h>
#include <sifrpc.h>
#include <sbv_patches.h>
#include <iopcontrol.h>
#include <stdio.h>

int gdb_stub_main( int argc, char *argv[] );

int main( int argc, char *argv[] )
{   
    int i=0;
    int j;

    printf("Hello, world! 1\n");
    j = gdb_stub_main(argc, argv);
    printf("Hello, world! 2\n");

    while(1)
    {
       i++;
       printf("Still alife!\n");
     }
     return 0;
}