#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "avl.h"
#include "avlUtile.h"
#include "somme.h"

int main(int argc, char const *argv[]) {
    assert(argc == 2);
    const char* nom_fichier = argv[1];
    assert(nom_fichier);
    pAVL avl = NULL;
    avl = transfert_donne_ds_AVL(avl);
    affichage_infixe(avl);

    //Si un fichier est passé en argument, on écrit l'arbre avec
    //l'option minmaxdans le fichier
    //si chaine non vide on suppose que c'est un fichier, et que le chemin est correct
    if(nom_fichier[0] != '\0'){
        FILE* fichier = fopen(nom_fichier, "w");
        if(fichier == NULL){
            fprintf(stderr,"Erreur lors de l'ouverture du fichier %s\n", nom_fichier);
            exit(1);
        }
        affichage_infixe_minmax(fichier, avl);
        fclose(fichier);
    }

    liberer_AVL(avl);   //free avl
    return 0;
}
