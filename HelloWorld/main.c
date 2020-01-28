#include <tamtypes.h>
#include <kernel.h>
#include <sifrpc.h>
#include <sbv_patches.h>
#include <iopcontrol.h>
#include <stdio.h>

#include <stdio.h>

extern int i1, i2, i3, i4;

int main()
{
       printf("i1=%d\n", i1);
       printf("i2=%d\n", i2);
       printf("i3=%d\n", i3);
       printf("i4=%d\n", i4);

       return 0;
}

asm(
".sdata         \n"
"dummy: .word 0 \n" // <- insert 1, 2, 3 or 4 dummy words, and see what happens!
".globl i1      \n"
"i1: .word 1111 \n"
".globl i2      \n"
"i2: .word 2222 \n"
".globl i3      \n"
"i3: .word 3333 \n"
".globl i4      \n"
"i4: .word 4444 \n"
);