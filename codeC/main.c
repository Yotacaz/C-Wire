#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "avl.h"
#include "avlUtile.h"
#include "somme.h"

int main() {
    // printf("2:30:233\n");
    // printf("2:30:20\n");
    // for (int i = 0; i<30; i++){
    //   printf("2:%d:%d\n",30-i,i);
    // }
    // return 0;

    srand(time(NULL));
    pAVL avl = NULL;
    avl = transfert_donne_ds_AVL(avl);
    affichageinfixe(avl);
    liberer_AVL(avl);
    return 0;
}
