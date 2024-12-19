#include "somme.h"
#include "station.h"
#include "avl.h"

pAVL somme(pAVL avl,Donnee_station donne,bool existe)
{
    if(existe == true){
        if(avl->val.ID_station > donne.ID_station && existe_fg(avl) == true){
            somme(avl->fg,donne,existe);
        }
        else if(avl->val.ID_station < donne.ID_station && existe_fd(avl) == true){
            somme(avl->fd,donne,existe);
        }
        else if(avl->val.ID_station == donne.ID_station){
            avl->val.conso =avl->val.conso + donne.conso;
            avl->val.capacite =avl->val.capacite + donne.capacite;
        }
    }
    else if(existe == false){
    	avl=insertionAVL(avl,donne);
    }
    return avl;    
}

pAVL transfert_donné_ds_AVL(pAVL avl)
{
    Donnee_station donnee;
    bool existence = false;
    short nb_scanné = 1;
    do{
        nb_scanné = scanf("%lu;%lu;%lu", &donnee.ID_station, &donnee.capacite, &donnee.conso);
        // printf("\n\n %d \n \n",nb_scanné);
        if(nb_scanné == 3){
            existence = recherche(avl,donnee);
            avl=somme(avl,donnee,existence);
        }
        else if(nb_scanné == -1) {
            return avl;
        }
        else {
            exit(EXIT_FAILURE);
        }
    }while(nb_scanné == 3);
}

bool recherche(pAVL arbre,Donnee_station elm)
{
    if(arbre == NULL){
        return false;
    }
	else if(arbre->val.ID_station > elm.ID_station){
		 return recherche(arbre->fg,elm);
	}
	else if(arbre->val.ID_station < elm.ID_station){
		return recherche(arbre->fd,elm);
	}
    else if(arbre->val.ID_station == elm.ID_station){
		return true;
	}
    else if(arbre->val.ID_station != elm.ID_station){
		return false;
	}
}
