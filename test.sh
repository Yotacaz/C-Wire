#!/bin/bash

station=$1
consommateur=$2

ecrire_fichier_sortie() {
	echo "Station $station : capacite : consomation ($consommateur) " > nom_temporaire.csv
	echo ./main >> nom_temporaire.csv
	#jsp comment main renvoie les valeurs
}

#clock :
tempsExe=$(date +%s)
#fonction1

tempsCompil=$(date +%s)
#fontion
tempsCompil=$(( $(date +%s) - tempsCompil ))

tempsExe=$(( $(date +%s) - tempsExe ))
tempsExe=$(( tempsExe - tempsCompil ))

ecrire_fichier_sortie

echo $tempsExe


tempsExe=$(date +%s)
#fonction1
tempsExe=$(( $(date +%s) - tempsExe ))
