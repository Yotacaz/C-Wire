#include "somme.h"
#include "station.h"
#include "avl.h"

//fonction qui adtitionne les consomation au station si elle existe et les crée si elle n'existe pas
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

//récupére les donnés du shel et vérifier qu'elle soit entière puis les sommes avec la fonction ci-dessus.
pAVL transfert_donne_ds_AVL(pAVL avl)
{
    Donnee_station donnee;
    bool existence = false;
    short nb_scanne = 1;
    do{
        nb_scanne = scanf("%lu;%lu;%lu", &donnee.ID_station, &donnee.capacite, &donnee.conso);
        // printf("\n\n %d \n \n",nb_scanne);
        if(nb_scanne == 3){
            existence = recherche(avl,donnee);
            avl=somme(avl,donnee,existence);
        }
        else if(nb_scanne == -1) {
            return avl;
        }
        else {
            exit(EXIT_FAILURE);
        }
    }while(nb_scanne == 3);
}

//recherche si la station existe déja dans l'AVL
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
