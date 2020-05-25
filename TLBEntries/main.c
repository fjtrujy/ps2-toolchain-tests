#include <stdio.h>
#include <kernel.h>

void testTLBEntries()
{
  int index;
  int tlbEntryValue;
  unsigned int pageMask, entryHi, entryLo0, entryLo1;

  for (index = 0x00; index < 0x30; index++ ) {
    tlbEntryValue = GetTLBEntry(index, &pageMask, &entryHi, &entryLo0, &entryLo1);

    printf("GetTLBEntry for index = %X, \treturns = %X, \t\tpageMask = %X, \t\tentryHi = %X, \t\tentryLo0 = %X, \t\tentryLo1 = %X\n", 
            index, tlbEntryValue, pageMask, entryHi, entryLo0, entryLo1);
  }
}

int main()
{
  int index = 0;
  testTLBEntries();

  while(1)
  {
    if (index % 100000 == 0) {
      printf("Still alive! \n");
    }

    index++;
  }

  return 0;
}