#!/bin/bash

station=$1
consommateur=$2

id_centrales="${*:4}"



CHEMIN_RESULTAT="test/"
CHEMIN_PROG_C="codeC/"
CHEMIN_FICHIER_TEMP="tmp/"
CHEMIN_GRAPH="graphs/"
CHEMIN_INPUT="input/"

NOM_EXECUTABLE="main"

error=0
#verif existance
if [ ! -d $CHEMIN_PROG_C ];then
	error=1
	echo "ERREUR: Le dossier $CHEMIN_PROG_C n'existe pas"
fi
if [ ! -d $CHEMIN_FICHIER_TEMP ];then
	mkdir $CHEMIN_FICHIER_TEMP
else
	rm -rf ${CHEMIN_FICHIER_TEMP:?}/*	#nettoyage du fichier temp
fi

#creation s'ils n'existent pas des dossiers nécéssaires
mkdir -p $CHEMIN_RESULTAT
mkdir -p $CHEMIN_GRAPH
mkdir -p $CHEMIN_INPUT

# TODO En cours
#compilation
if [ ! -f $CHEMIN_PROG_C$NOM_EXECUTABLE ]; then
	if ! make -s -C "$CHEMIN_PROG_C" ; then
		echo "ERREUR lors de la compilation de l'executable"
		exit 1
	fi
fi

#initialise nom du fichier de sortie et creer l'en-tête
init_fichier_sortie() {
	#génération du nom du fichier de sortie
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
	fichier_sortie="$CHEMIN_RESULTAT""$station"_"$consommateur""$sep_id_centrales".csv
	
	#génération de l'en tête du document
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

echo "Temps d'execution du programme : ""$tempsExe""s"

tempsExe=$(date +%s)
#fonction1
tempsExe=$(( $(date +%s) - tempsExe ))


#compilation & exec des prog c
#TODO test sur existance du Makefile
make -s -C "$CHEMIN_PROG_C" clean
make -s -C "$CHEMIN_PROG_C"

#tris des donnes de sortie croissant en fonction de la 2eme colonne (capacité), séparées par des ':' 
if ! ./"$CHEMIN_PROG_C""main" | sort -n --key=2 --field-separator=':' >> "$fichier_sortie"; then
	echo "Une erreur a été rencontrée lors de l'execution du programme c"
	exit 1
fi

#cas où on doit creer le fichier lv_all_minmax
if [ "$station" = "lv" ] && [ "$consommateur" = "all" ]; then
	fichier_minmax="$CHEMIN_RESULTAT""lv_all_minmax.csv"
	fichier_temp1="$CHEMIN_FICHIER_TEMP""tmp1"
	{
		#format de l'en tête : Station lv:capacite:consomation(tous):consomation en trop
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
	
	#tris à ajout sur le fichier minmax
	fichier_temp2="$CHEMIN_FICHIER_TEMP""tmp2"
	#tris décroissant en fonction de la 4eme colonne (conso en trop), séparées par des ':' 
	sort -r -n --key=4 --field-separator=':' "$fichier_temp1" > "$fichier_temp2"
	
	n_ligne=$(wc -l "$fichier_temp1" | cut -d ' ' -f1)
	
	#copie des résultats dans le fichier de resultat (lv_all_minmax.csv)
	if [ "$n_ligne" -lt 21  ]; then
		cat "$fichier_temp2" >> "$fichier_minmax"
	else	
		head -n 10 "$fichier_temp2" >> "$fichier_minmax"
		tail -n 10 "$fichier_temp2" >> "$fichier_minmax"
	fi
	#nettoyage
	rm "$fichier_temp1"
	rm "$fichier_temp2"
fi
