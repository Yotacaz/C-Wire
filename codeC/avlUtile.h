#pragma once
#include "avl.h"
#include "station.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void afficherAVL(pAVL nd, int niveau);
void affichage_infixe(pAVL noeuds);
void affichage_infixe_minmax(FILE* fichier, pAVL noeud);
