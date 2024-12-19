#pragma once
#include <stdio.h>

#include "station.h"

pAVL somme(pAVL avl,Donnee_station donne,bool existe);
pAVL transfert_donné_ds_AVL(pAVL avl);
bool recherche(pAVL arbre,Donnee_station elm);
