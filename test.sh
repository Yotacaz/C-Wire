#!/bin/bash

station=$1
consommateur=$2

id_centrales="${*:4}"

chemin_sortie="test/"
chemin_prog_c="codeC/"

init_fichier_sortie() {
	local nom_consommateur=""
	case "$consommateur" in
		all)
			nom_consommateur="tous" ;;
		comp)
			nom_consommateur="entreprises" ;;
		indiv) 
			nom_consommateur="individus" ;;
	esac
	local sep_id_centrales=""
	if [ -n "$id_centrales" ]; then
		sep_id_centrales="_""$(echo "$id_centrales" | tr ' ' '_')"
	fi
	fichier_sortie="$chemin_sortie""$station"_"$consommateur""$sep_id_centrales".csv

	echo "Station $station:capacite:consomation($nom_consommateur)" > "$fichier_sortie"


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



#compilation des prog c
#todo test sur existance du Makefile
make -C "$chemin_prog_c"
./"$chemin_prog_c""main"