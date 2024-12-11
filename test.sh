#!/bin/bash

station=$1
consommateur=$2

ecrire_fichier_sortie() {
	echo "Station $station : capacite : consomation ($consommateur) " > nom_temporaire.csv
	echo ./main >> nom_temporaire.csv

}

#clock :
tempsDeb=$(time)

tempsDebPause=$(time)
tempsFinPause=$(time)

tempsFin=$(time)

duree=$(((tempsFin - tempsDeb) - (tempsFinPause + tempsDebPause)))

ecrire_fichier_sortie

echo $duree
