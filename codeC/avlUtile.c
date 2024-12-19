#include "avl.h"
#include "station.h"
#include "avlUtile.h"

//affiche un arbre à la vertical comme si on l'écrivé sur papier
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
    printf("ID=%lu ca=%lu co=%lu "
           "\x1B[0;34m"
           "%d\n"
           "\x1B[0m",
           nd->val.ID_station,nd->val.capacite,nd->val.conso, nd->eq);

    // Affiche ensuite le sous-arbre gauche
    afficherAVL(nd->fg, niveau + 1);
}

//renvoie les données finaux au shel grâce à un parcours infixe
void affichageinfixe(pAVL noeuds)
{
	if(existe_fg(noeuds) == true){
		affichageinfixe(noeuds->fg);
	}
	traiter_station(noeuds->val);
	if(existe_fd(noeuds) == true){
		affichageinfixe(noeuds->fd);
	}
}
