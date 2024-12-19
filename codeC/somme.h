#pragma once
#include "avl.h"
#include "station.h"
#include <stdbool.h>
#include <stdio.h>

pAVL somme(pAVL avl, Donnee_station donne, bool existe);
pAVL transfert_donne_ds_AVL(pAVL avl);
bool recherche(pAVL arbre, Donnee_station elm);
