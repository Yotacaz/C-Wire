#pragma once
#include <stdio.h>
#include <stdbool.h>

#include "station.h"

pAVL somme(pAVL avl,Donnee_station donne,bool existe);
pAVL transfert_donne_ds_AVL(pAVL avl);
bool recherche(pAVL arbre,Donnee_station elm);
