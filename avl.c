#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

typedef struct struct_nd
{
    int eq;
    int val;
    struct struct_nd * fg;
    struct struct_nd * fd;
}Noeud;

typedef Noeud * pAVL;

int main{
  return 0;
}
