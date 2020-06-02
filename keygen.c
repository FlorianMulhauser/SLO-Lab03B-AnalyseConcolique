#include <string.h>
#include <stdio.h>

int main(int a1, char **a2, char **a3)
{
  __int64_t v4; // rbx@10
  signed int v5; // [sp+1Ch] [bp-14h]@4

  if ( a1 == 2 )
  {
    if ( 42 * (strlen(a2[1]) + 1) != 504 )
      goto LABEL_31;
    v5 = 1;
    if ( *a2[1] != 69 )
      v5 = 0;
    if ( 2 * a2[1][3] != 202 )
      v5 = 0;
    if ( *a2[1] + 14 != a2[1][6] - 14 )
      v5 = 0;
    v4 = a2[1][5];
    if ( v4 != 9 * strlen(a2[1]) + 6)
      v5 = 0;
    if ( a2[1][1] != a2[1][7]+6 )
      v5 = 0;
    if ( a2[1][1] != a2[1][10] + 5)
      v5 = 0;
    if ( a2[1][1] - 51 != *a2[1] )
      v5 = 0;
    if ( a2[1][3] + 16!= a2[1][9] )
      v5 = 0;
    if ( a2[1][4] != 108 )
      v5 = 0;
    if ( a2[1][2] - a2[1][1] != -8 )
      v5 = 0;
    if ( a2[1][8] - a2[1][7] != -5 )
      v5 = 0;
    if ( v5 ) {
      printf("Launching software!\n");
    }
    else
LABEL_31:
      printf("Try again...\n");
  }
  else
  {
    printf("Usage: %s <pass>\n", *a2);
  }
}

