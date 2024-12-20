#!/bin/bash

#constantes
CHEMIN_RESULTAT="test/"
CHEMIN_PROG_C="codeC/"
CHEMIN_FICHIER_TEMP="tmp/"
CHEMIN_GRAPH="graphs/"
CHEMIN_INPUT="input/"

FICHIER_MINMAX=$CHEMIN_RESULTAT"lv_all_minmax.csv"

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

#cas où on doit creer le fichier lv_all_minmax
est_lv_all() { [ "$station" = "lv" ] && [ "$consommateur" = "all" ]; }

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
if [ ! -d $CHEMIN_PROG_C ]; then
    echo "ERREUR: Le dossier $CHEMIN_PROG_C n'existe pas"
    exit 1
fi

if [ ! -f $CHEMIN_PROG_C$NOM_MAKEFILE ]; then
    echo "ERREUR: Le fichier $CHEMIN_PROG_C$NOM_MAKEFILE n'existe pas"
    exit 1
fi

if [ ! -d $CHEMIN_FICHIER_TEMP ]; then
    mkdir $CHEMIN_FICHIER_TEMP
else
    rm -rf ${CHEMIN_FICHIER_TEMP:?}/* #nettoyage du fichier temp
fi

#creation s'ils n'existent pas des dossiers nécéssaires
mkdir -p $CHEMIN_RESULTAT
mkdir -p $CHEMIN_GRAPH
mkdir -p $CHEMIN_INPUT


#Si on doit creer le fichier lv_all_minmax
#alors son chemin absolu est non vide (norme prise ici)
chemin_absolut_minmax=""
if est_lv_all; then
    rm -f "$FICHIER_MINMAX"
    chemin_absolut_minmax=$(pwd)"/$FICHIER_MINMAX"
fi

#compilation
#TODO A SUPPRIMER
make -s -C "$CHEMIN_PROG_C" clean
if [ ! -f $CHEMIN_PROG_C$NOM_EXECUTABLE ]; then
    if ! make -s -C "$CHEMIN_PROG_C"; then
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
    filtre="^[^;]+"
fi

case "$station" in
    hvb) filtre="$filtre;[0-9]+;-;" ;;
    hva) filtre="$filtre;[^;]+;[0-9]+;-;" ;;
    lv)
        fc=""
        case "$consommateur" in
            indiv) fc="-;[^;]+;" ;;
            comp) fc="[^;]+;-;" ;;
            all) fc="" ;;
        esac
        filtre="$filtre;[^;]+;[^;]+;[0-9]+;$fc"
        ;;
esac

#génération du nom du fichier de sortie
nom_consommateur=""
case "$consommateur" in
    all)
        nom_consommateur="tous"
        ;;
    comp)
        nom_consommateur="entreprises"
        ;;
    indiv)
        nom_consommateur="individus"
        ;;
esac
sep_id_centrales=""
if [ -n "$id_centrales" ]; then
    sep_id_centrales="_""$(echo "$id_centrales" | tr ' ' '_')"
fi
fichier_sortie="$CHEMIN_RESULTAT""$station"_"$consommateur""$sep_id_centrales".csv

#génération de l'en tête du fichier csv

temps_dep=$(date +%s)

echo "Station $station:capacite:consommation($nom_consommateur)" >"$fichier_sortie"

cat "$chemin" |
    tail -n+2 |
    grep -E "$filtre" |
    cut -d ";" -f "$id_station,7,8" |
    grep -v "^-" |
    tr '-' '0' |
    ./"$CHEMIN_PROG_C""main" "$chemin_absolut_minmax" |
    sort -n --key=2 --field-separator=':' \
        >>"$fichier_sortie"
#tris des donnes de sortie croissant en fonction de la 2eme colonne (capacité), séparées par des ':'

if in_array 1 "${PIPESTATUS[@]}"; then
    echo "Une erreur a été rencontrée lors de l'execution du programme c"
    exit 1
fi
#SORTIE DU FICHIER C

temps_minmax=$(date +%s)


#TODO : A changer

if est_lv_all; then
    if [ ! -f "$FICHIER_MINMAX" ]; then
        echo "ERREUR lors de la creation du fichier $FICHIER_MINMAX (fichier inexistant)"
    fi

    #tris à ajout sur le fichier minmax
    #tris décroissant en fonction de la 4eme colonne (conso en trop), séparées par des ':'

    min_max=$(sort -n --key=4 --field-separator=':' "$FICHIER_MINMAX")
    n_ligne=$(wc -l <<<"$min_max")

    #creation de l'en-tête du fichier minmax
    echo "Station lv:capacite:consommation(tous):consommation en trop" >"$FICHIER_MINMAX"
    #copie des résultats dans le fichier de resultat (lv_all_minmax.csv)
    if [ "$n_ligne" -lt 21 ]; then
        cat <<<"$min_max" >>"$FICHIER_MINMAX"
    else
        head -n 10 <<<"$min_max" >>"$FICHIER_MINMAX"
        tail -n 10 <<<"$min_max" >>"$FICHIER_MINMAX"
    fi
fi

temps_tot=$(date +%s)

echo "temps d'execution total : $(("$temps_tot" - "$temps_dep"))"

if est_lv_all; then
    echo "temps de creation du fichier $fichier_sortie : $(("$temps_minmax" - "$temps_dep"))"
    echo "temps de creation du fichier $FICHIER_MINMAX : $(("$temps_tot" - "$temps_minmax"))"
fi
