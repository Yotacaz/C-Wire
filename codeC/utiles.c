#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#include "utiles.h"

int min2(int a, int b)
{
    return a < b ? a : b;
}

int max2(int a, int b)
{
    return a < b ? b : a;
}

int max3(int a, int b, int c)
{
    if (a < b)
    {
        return max2(b, c);
    }
    return max2(a, c);
}

int min3(int a, int b, int c)
{
    if (a < b)
    {
        return min2(a, c);
    }
    return min2(b, c);
}

//verifier si la condition est vrai, sinon arrete le programe avec un message d'erreur
void verifier(bool condition, char * msg_err){
    assert(msg_err);
    if (!condition){
        printf(BOLD_RED"%s"RESET, msg_err);
        exit(EXIT_FAILURE);
    }
}