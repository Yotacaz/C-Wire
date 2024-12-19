#!/bin/bash

#constantes
CHEMIN_RESULTAT="test/"
CHEMIN_PROG_C="codeC/"
CHEMIN_FICHIER_TEMP="tmp/"
CHEMIN_GRAPH="graphs/"
CHEMIN_INPUT="input/"

NOM_EXECUTABLE="main"
NOM_MAKEFILE="Makefile"

aide() {
  echo "Utilisation: $0 <chemin> <station> <consommateur> [<centrales>]"
}

in_array() {
  local e
  for e in "${@:2}"; do
    if [[ "$e" == "$1" ]]; then
      return 0
    fi
  done
  return 1
}



if in_array "-h" "$@"; then
  aide
  exit 0
fi

if [ $# -lt 3 ]; then
  echo "ERREUR: Pas assez d'arguments"
  aide
  exit 1
fi

chemin="$1"
station="$2"
consommateur="$3"
id_centrales="${*:4}"

error=0
if [ ! -f "$chemin" ]; then
  echo "ERREUR: Le fichier \"$chemin\" n'existe pas"
  error=1
fi

if ! in_array "$station" hvb hva lv; then
  echo "ERREUR: La station doit être \"hvb\", \"hva\" ou \"lv\". Vous avez saisi \"$station\""
  error=1
fi

if ! in_array "$consommateur" comp indiv all; then
  echo "ERREUR: Le consommateur doit être \"comp\", \"indiv\" ou \"all\". Vous avez saisi \"$consommateur\""
  error=1
fi

if in_array "$station-$consommateur" hvb-all hvb-indiv hva-all hva-indiv; then
  echo "ERREUR: Vous ne pouvez pas avoir un consommateur \"$consommateur\" avec une station \"$station\""
  error=1
fi

if [ $error -eq 1 ]; then
  aide
  exit 1
fi

#verif existance
if [ ! -d $CHEMIN_PROG_C ];then
	echo "ERREUR: Le dossier $CHEMIN_PROG_C n'existe pas"
  exit 1
fi

if [ ! -d $CHEMIN_PROG_C$NOM_MAKEFILE ];then
	echo "ERREUR: Le fichier $CHEMIN_PROG_C$NOM_MAKEFILE n'existe pas"
  exit 1
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

#compilation
if [ ! -f $CHEMIN_PROG_C$NOM_EXECUTABLE ]; then
	if ! make -s -C "$CHEMIN_PROG_C" ; then
		echo "ERREUR lors de la compilation de l'executable"
		exit 1
	fi
fi


# Format des données en entrée :
# |  1  |  2  |  3  |  4  |  5  |  6  |  7  |   8   |
# |-----+-----+-----+-----+-----+-----+-----+-------|
# | idc |  -  |  -  |  -  |  -  |  -  | cap |   -   | Centrale (id et production maximale / capacite)
# | idc | idb |  -  |  -  |  -  |  -  | cap |   -   | Station HV-B (id centrale, id hvb et capacite)
# | idc | idb |  -  |  -  | ide |  -  |  -  | conso | Entreprise sur HV-B (id centrale, id hvb, id entp et consommation)
# | idc | idb | ida |  -  |  -  |  -  | cap |   -   | Station HV-A (id centrale, id hvb, id hva et capacite)
# | idc |  -  | ida |  -  | ide |  -  |  -  | conso | Entreprise sur HV-A (id centrale, id hva, id entp et consommation)
# | idc |  -  | ida | idv |  -  |  -  | cap |   -   | Poste LV (id centrale, id hva, id lv et capacite)
# | idc |  -  |  -  | idv | ide |  -  |  -  | conso | Entreprise sur LV (id centrale, id lv, id entp et consommation)
# | idc |  -  |  -  | idv |  -  | idp |  -  | conso | Particulier sur LV (id centrale, id lv, id particulier et consommation)

id_station=""
case "$station" in
  hvb) id_station=2 ;;
  hva) id_station=3 ;;
  lv) id_station=4 ;;
esac

filtre=""
if [ -n "$id_centrales" ]; then
  filtre="^($(echo "$id_centrales" | tr ' ' '|'));"
else
  filtre="^[^;]+;"
fi

case "$station" in
  hvb) filtre="$filtre[0-9]+;-;" ;;
  hva) filtre="$filtre[^;]+;[0-9]+;-;" ;;
  lv) ;;
esac

cat "$chemin" | tail -n+2 | grep -E "$filtre" | cut -d ";" -f "$id_station,7,8" | grep -v "^-" | tr '-' '0'


#SORTIE DU FICHIER C

#génération du nom du fichier de sortie
nom_consommateur=""
case "$consommateur" in
  all)
    nom_consommateur="tous" ;;
  comp)
    nom_consommateur="entreprises" ;;
  indiv) 
    nom_consommateur="individus" ;;
esac
sep_id_centrales=""
if [ -n "$id_centrales" ]; then
  sep_id_centrales="_""$(echo "$id_centrales" | tr ' ' '_')"
fi
fichier_sortie="$CHEMIN_RESULTAT""$station"_"$consommateur""$sep_id_centrales".csv

#génération de l'en tête du fichier csv
echo "Station $station:capacite:consomation($nom_consommateur)" > "$fichier_sortie"


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
