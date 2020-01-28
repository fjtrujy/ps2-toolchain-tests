#include <stdio.h>

void init_scr(void);
void scr_printf(const char *, ...) __attribute__((format(printf,1,2)));

extern const int i1, i2, i3, i4;

int main()
{
   init_scr();
   scr_printf("i1=%d\n", i1);
   scr_printf("i2=%d\n", i2);
   scr_printf("i3=%d\n", i3);
   scr_printf("i4=%d\n", i4);

   while(1) {}
   
   return 0;
}

const int dum1=1;
const int dum2=2;
const int dum3=3;
const int dum4=4;
const int dum5=5;
const int i1=1111;
const int i2=2222;
const int i3=3333;
const int i4=4444;
