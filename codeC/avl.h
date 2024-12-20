#pragma once
#include "station.h"
#include "utiles.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct struct_nd {
    int eq;             // equilibre
    Donnee_station val; // valeur
    struct struct_nd *fg;
    struct struct_nd *fd;
} Noeud;

typedef Noeud *pAVL;

pAVL creerAVL(Donnee_station val);
bool existe_fd(Noeud *nd);
bool existe_fg(Noeud *nd);
int assertAVL(pAVL a);
Noeud *rotationGauche(Noeud *nd);
Noeud *rotationDroite(Noeud *nd);
Noeud *doubleRotationDroite(Noeud *nd);
Noeud *doubleRotationGauche(Noeud *nd);
Noeud *equilibrageAVL(Noeud *nd);
Noeud *insertionAVLrec(Noeud *nd, Donnee_station val, int *h);
pAVL insertionAVL(Noeud *racine, Donnee_station val);
Noeud *suppMax(Noeud *nd, unsigned long *max, int *h);
Noeud *suppValAVLrec(Noeud *nd, Donnee_station val, int *h);
pAVL suppValAVL(pAVL racine, Donnee_station val);
void liberer_AVL(pAVL avl);
