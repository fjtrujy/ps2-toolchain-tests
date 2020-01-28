#include <stdio.h>

extern const int i1, i2, i3, i4;
extern const char *formatt;


int main()
{
       printf(formatt, i1);
       printf(formatt, i2);
       printf(formatt, i3);
       printf(formatt, i4);

       return 0;
}

const char *formatt = "%d\n";
const int dum1=1;
const int dum2=2;
const int dum3=3;
const int dum4=4;
const int dum5=5;
const int i1=1111;
const int i2=2222;
const int i3=3333;
const int i4=4444;
