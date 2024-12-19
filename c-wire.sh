#!/bin/bash

#constantes
CHEMIN_RESULTAT="test/"
CHEMIN_PROG_C="codeC/"
CHEMIN_FICHIER_TEMP="tmp/"
CHEMIN_GRAPH="graphs/"
CHEMIN_INPUT="input/"

NOM_EXECUTABLE="main"

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

chemin_sortie="test/"  #TODO : Tests pour savoir si rep existe
chemin_prog_c="codeC/"

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

filtre_centrales=""
if [ -n "$id_centrales" ]; then
  filtre_centrales="^($(echo "$id_centrales" | tr ' ' '|'));"
fi

cat "$chemin" | tail -n+2 | grep -E "$filtre_centrales" | cut -d ";" -f "$id_station,7,8" | grep -v "^-" | tr '-' '0'
