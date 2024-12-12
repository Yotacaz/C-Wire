#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <time.h>


typedef struct _donneeStation             //raccourcie qui ralonge
{
    unsigned int ID_station;
    unsigned int conso;
    unsigned int capacite;
} Donnee_station;

typedef struct struct_nd
{
    int eq;                            //equilibre
    Donnee_station val;                //valeur
    struct struct_nd *fg;
    struct struct_nd *fd;
} Noeud;

typedef Noeud *pAVL;

//FONCTIONS 

int min2(int a, int b){
    return a < b ? a : b;                //si a < b alors retourner a sinon retourner b
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

// Affichage de l'arbre en mode horizontal
void afficherAVL(pAVL nd, int niveau)
{
    if (nd == NULL)
    {
        return;
    }

    // Affiche d'abord le sous-arbre droit (car affichage horizontal)
    afficherAVL(nd->fd, niveau + 1);

    // Affiche le noeud actuel avec indentation
    for (int i = 0; i < niveau; i++)
    {
        printf("     "); // indentationint main(){
    }
    printf("ID=%d ca=%d co=%d "
           "\x1B[0;34m"
           "%d\n"
           "\x1B[0m",
           nd->val.ID_station,nd->val.capacite,nd->val.conso, nd->eq);

    // Affiche ensuite le sous-arbre gauche
    afficherAVL(nd->fg, niveau + 1);
}

// attention ne pas utiliser si assert desactive
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

Noeud *suppMax(Noeud *nd, int *max, int *h)
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

/// @brief create a random int table where each value is unique
/// @param min minimal value for the table
/// @param max maximal value for the table
/// @param size size of table
/// @return the table
Donnee_station *unique_rand_tab(int min, int max, int size)
{
    int delta = max - min + 1;
    assert(size > 0);

    assert(delta >= size);
    Donnee_station *tab = malloc(sizeof(Donnee_station) * size);
    assert(tab);
    Donnee_station rand_val;
    rand_val.ID_station = 0;
    rand_val.conso = 0;
    rand_val.capacite = 0;
    bool in_tab = true;
    for (int i = 0; i < size; i++)
    {
        in_tab = true;
        while (in_tab) // bruteforce
        {
            rand_val.ID_station = rand() % delta + min;
            rand_val.conso = rand() % delta + min;
            rand_val.capacite = rand() % delta + min;
            in_tab = false;
            for (int j = 0; j < i; j++)
            {
                if (rand_val.ID_station == tab[j].ID_station)
                {
                    in_tab = true;
                    break;
                }
            }
        }
        tab[i] = rand_val;
    }
    return tab;
}

pAVL suppRand(pAVL avl, Donnee_station *tab, int size)
{
    assert(size > 0);
    assert(tab);
    int i = rand() % size;
    Donnee_station val = tab[i];
    tab[i] = tab[size - 1];
    tab[size - 1] = val;
    avl = suppValAVL(avl, val);
    assertAVL(avl);
    return avl;
}

/// @brief genere des avl effectue des operation aleatoire et verifie la validite de l'avl
/// @param min 
/// @param max 
/// @param size 
/// @return l'avl apres des opperations
pAVL randAVLTest(int min, int max, int size)
{
    assert(size > 0);
    assert(min + size < max + 1);
    Donnee_station *tab = unique_rand_tab(min, max, size);
    pAVL avl = creerAVL(tab[0]);
    for (int i = 1; i < size; i++)
    {
        avl = insertionAVL(avl, tab[i]);
    }
    assertAVL(avl);

    // for (int i = 0; i < size / 4; i++)
    // {
    //     avl = suppRand(avl, tab, size - i);
    //     assertAVL(avl);
    // }

    free(tab);
    return avl;
}

pAVL somme(pAVL avl,Donnee_station donne){
    // printf("PA    %u", donne.ID_station);
    // printf("  %u  ", donne.capacite);
    // printf("%u\n", donne.conso);
    if(avl==NULL){
        avl= insertionAVL(avl,donne);
    }
    else if(avl->val.ID_station == donne.ID_station){
        // printf("%u  %u  %u\n", avl->val.ID_station,avl->val.capacite,avl->val.conso);
        avl->val.conso =avl->val.conso + donne.conso;
        avl->val.capacite =avl->val.capacite + donne.capacite;
        // printf("%u  %u  %u\n", avl->val.ID_station,avl->val.capacite,avl->val.conso);
    }
    else if(avl->val.ID_station > donne.ID_station && existe_fg(avl) == true){
        somme(avl->fg,donne);
    }
    else if(avl->val.ID_station < donne.ID_station && existe_fd(avl) == true){
        somme(avl->fd,donne);
    }
    else if(avl->val.ID_station > donne.ID_station && existe_fg(avl) == false){
        avl = insertionAVL(avl,donne);
    }
    else if(avl->val.ID_station < donne.ID_station && existe_fd(avl) == false){
    	avl = insertionAVL(avl,donne);
    }
    return avl;    
}

pAVL transfert_donné_ds_AVL(pAVL avl){
    Donnee_station donnee;
    while(scanf("%u;%u;%u", &donnee.ID_station, &donnee.capacite, &donnee.conso) == 3){
        avl=somme(avl,donnee);
    }
return avl;
}

int main()
{
    srand(time(NULL));
    pAVL avl = NULL;
    avl=transfert_donné_ds_AVL(avl);
    afficherAVL(avl,0);
    return 0;
}
