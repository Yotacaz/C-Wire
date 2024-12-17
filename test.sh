#!/bin/bash

station=$1
consommateur=$2

id_centrales="${*:4}"

#TODO verif existance
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



}

#clock :
tempsExe=$(date +%s)
#fonction1

tempsCompil=$(date +%s)
#fontion
tempsCompil=$(( $(date +%s) - tempsCompil ))

tempsExe=$(( $(date +%s) - tempsExe ))
tempsExe=$(( tempsExe - tempsCompil ))

init_fichier_sortie

echo $tempsExe

tempsExe=$(date +%s)
#fonction1
tempsExe=$(( $(date +%s) - tempsExe ))


#compilation & exec des prog c
#TODO test sur existance du Makefile
make -s -C "$chemin_prog_c" clean
make -s -C "$chemin_prog_c"

if ! ./"$chemin_prog_c""main" >> "$fichier_sortie"; then
	echo "Une erreur a été rencontrée lors de l'execution du programme c"
fi

if [ "$station" = "lv" ] && [ "$consommateur" = "all" ]; then
	fichier_minmax="lv_all_minmax.csv"
	fichier_temp1=$(mktemp)
	{
		read -r tete
		echo "$tete:consomation en trop" > "$fichier_minmax"
		while IFS=':' read -r n_station capa conso; do
			#on s'assure que capa et conso sont des nombres
			if [[ "$conso" =~ ^[0-9]+$ && "$capa" =~ ^[0-9]+$ ]]; then
				conso_en_trop=$((conso - capa))
			else
				conso_en_trop="NA"
			fi
			echo "$n_station:$capa:$conso:$conso_en_trop" >> "$fichier_temp1"
		done
	} < "$fichier_sortie"
	fichier_temp2=$(mktemp)
	sort -r -n --key=4 --field-separator=':' "$fichier_temp1" > "$fichier_temp2"
	
	n_ligne=$(wc -l "$fichier_temp1" | cut -d ' ' -f1)
	
	if [ "$n_ligne" -lt 21  ]; then
		cat "$fichier_temp2" >> "$fichier_minmax"
	else	
		head -n 10 "$fichier_temp2" >> "$fichier_minmax"
		tail -n 10 "$fichier_temp2" >> "$fichier_minmax"
	fi
	rm "$fichier_temp1"
	rm "$fichier_temp2"
fi

# Station lv:capacite:consomation(tous):consomation en trop