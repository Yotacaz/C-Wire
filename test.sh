#!/bin/bash

station=$1
consommateur=$2


chemin_sortie="test/"

ecrire_fichier_sortie() {
	local nom_consommateur=""
	case "$consommateur" in
		all)
			nom_consommateur="tous" ;;
		comp)
			nom_consommateur="entreprises" ;;
		indiv) 
			nom_consommateur="individus" ;;
	esac
	echo "Station $station : capacite : consomation ($nom_consommateur) " > "$chemin_sortie""$station"_"$consommateur".csv

	# if [ "$station" = "lv" ] && [ "$consommateur" = "all" ]; then
	# 	echo test reussi
	# fi
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
