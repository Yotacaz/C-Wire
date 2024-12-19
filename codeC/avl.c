#include "avl.h"
#include "station.h"
#include "utiles.h"

pAVL creerAVL(Donnee_station val)
{
    pAVL avl = malloc(sizeof(Noeud));
    assert(avl);
    avl->eq = 0;
    avl->fd = NULL;
    avl->fg = NULL;
    avl->val.ID_station = val.ID_station;
    avl->val.conso = val.conso;
    avl->val.capacite = val.capacite;
    return avl;
}

bool existe_fd(Noeud *nd)
{
    assert(nd);                                 // cas pas pris en charge
    return nd->fd != NULL;
}

bool existe_fg(Noeud *nd)
{
    assert(nd);                                 // cas pas pris en charge
    return nd->fg != NULL;
}

int assertAVL(pAVL a)
{
    if (!a)
    {
        return 0;
    }
    int hg = assertAVL(a->fg);
    int hd = assertAVL(a->fd);
    int eq = hd - hg;
    assert(eq < 2 && eq > -2); // on supose que l'arbre est equilibre
    assert(a->eq == eq);
    return (hg > hd) ? hg + 1 : hd + 1; // on retourne la hauteur du parent
}

Noeud *rotationGauche(Noeud *nd)
{
    assert(nd);
    // assert(nd->eq ==2);
    Noeud *pivot = nd->fd;
    assert(pivot);
    // rotation
    nd->fd = pivot->fg;
    pivot->fg = nd;
    // mise à jour eq
    if (pivot->eq >= 0)
    {
        nd->eq = nd->eq - 1 - pivot->eq; // Ajustement de nd en fonction de l'équilibre de pivot
    }
    else
    {
        nd->eq = nd->eq - 1; // Le sous-arbre droit perd un niveau
    }

    pivot->eq = pivot->eq - 1 + ((nd->eq < 0) ? nd->eq : 0);
    return pivot;
}

Noeud *rotationDroite(Noeud *nd)
{
    assert(nd);
    // assert(nd->eq ==-2);
    Noeud *pivot = nd->fg;
    // printf("fin\n");
    // afficherAVL(nd, 0);
    assert(pivot);
    // rotation
    nd->fg = pivot->fd;

    pivot->fd = nd;

    // mise a jour eq
    nd->eq += 1 - min2(0, pivot->eq);

    pivot->eq += 1 + max2(nd->eq, 0);
    return pivot;
}

Noeud *doubleRotationDroite(Noeud *nd)
{
    // rot gauche a droite de nd puis droite sur nd
    assert(nd);
    assert(nd->eq == -2);
    assert(nd->fg);
    assert(nd->fg->eq == 1); // desequilibre sur le centre

    nd->fg = rotationGauche(nd->fg);
    return rotationDroite(nd);
}

Noeud *doubleRotationGauche(Noeud *nd)
{
    // rot droite a gauche de nd puis gauche sur nd
    assert(nd);
    assert(nd->eq == 2);
    assert(nd->fd);
    assert(nd->fd->eq == -1); // desequilibre sur le centre

    nd->fd = rotationDroite(nd->fd);
    return rotationGauche(nd);
}

Noeud *equilibrageAVL(Noeud *nd)
{
    assert(nd);
    assert(nd->eq < 3 && nd->eq > -3);
    if (nd->eq == 2)
    {
        assert(nd->fd);
        if (nd->fd->eq == -1)
        {
            return doubleRotationGauche(nd);
        }
        return rotationGauche(nd);
    }
    else if (nd->eq == -2)
    {
        assert(nd->fg);
        if (nd->fg->eq == 1)
        {
            return doubleRotationDroite(nd);
        }
        return rotationDroite(nd);
    }
    return nd; // aucun equilibrage necessaire
}


/// @brief insere un entier dans un avl
/// @param nd racine
/// @param val valeur de la station à ajouter
/// @param h pointeur variation d'equilibre (initialement *h = 0)
/// @return la racine
Noeud *insertionAVLrec(Noeud *nd, Donnee_station val, int *h)
{

    assert(h);
    if (!nd)
    {
        *h = 1;
        return creerAVL(val);
    }
    else if (nd->val.ID_station > val.ID_station)
    {
        nd->fg = insertionAVLrec(nd->fg, val, h);
        *h = -*h; //<=> *h = -abs(*h)
    }
    else if (nd->val.ID_station < val.ID_station)
    {
        nd->fd = insertionAVLrec(nd->fd, val, h);
        //(*h = h)
    }
    else
    {
        *h = 0;
        return nd;
    }
    if (*h != 0)
    {
        nd->eq += *h;
        nd = equilibrageAVL(nd);
        *h = (nd->eq == 0) ? 0 : 1; // si eq==0 : pas retiré de hauteur au parent
    }
    return nd;
}

    pAVL insertionAVL(Noeud *racine, Donnee_station val)
{
    int h = 0;
    Noeud *nd = insertionAVLrec(racine, val, &h);
    assert(nd);
    return nd;
}

Noeud *suppMax(Noeud *nd, unsigned long *max, int *h)
{
    assert(max);
    assert(nd);
    assert(h);
    if (!nd->fd)
    {
        *max = nd->val.ID_station;
        *h = -1; // on sup un elem qui se situe a droite
        Noeud *tmp = nd->fg;
        free(nd);
        return tmp;
    }

    nd->fd = suppMax(nd->fd, max, h);
    if (*h != 0)
    {
        nd->eq += *h;

        nd = equilibrageAVL(nd);
        if (nd->eq != 0)
        {
            *h = 0; // eq!=0 <=> on a pas changé la hauteur du parent (fg porte la hauteur)
        }
    }
    return nd;
}

// n'appeler que depuis la fonction suppValAVL(...)
Noeud *suppValAVLrec(Noeud *nd, Donnee_station val, int *h)
{
    assert(h);
    if (!nd)
    {
        *h = 0;
        return nd;
    }
    else if (nd->val.ID_station > val.ID_station)
    {
        nd->fg = suppValAVLrec(nd->fg, val, h);
        //(*h = h)
    }
    else if (nd->val.ID_station < val.ID_station)
    {
        nd->fd = suppValAVLrec(nd->fd, val, h);
        *h = -*h; //(*h) devient negatif
    }
    else
    {
        if (existe_fg(nd))
        {
            nd->fg = suppMax(nd->fg, &(nd->val.ID_station), h); // supprime le max de gauche
            *h = -*h;                                // suppMax modifie *h en une valeur neg/nulle
        }
        else
        {
            *h = 1;
            Noeud *temp = nd->fd;
            free(nd);
            // pas besoin de reequilibrer temp, on a pas modif ses enfants
            return temp;
        }
    }
    if (*h != 0) // equilibrage
    {
        nd->eq += *h;
        nd = equilibrageAVL(nd);
        *h = (nd->eq == 0) ? 1 : 0;
    }
    return nd;
}

pAVL suppValAVL(pAVL racine, Donnee_station val)
{
    assert(racine);
    int h = 0;
    pAVL tmp = suppValAVLrec(racine, val, &h);
    return tmp;
}

void freeAVL(pAVL avl)
{
    if (!avl)
    {
        return;
    }
    freeAVL(avl->fg);
    freeAVL(avl->fd);
    free(avl);
}
