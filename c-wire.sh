#!/bin/bash

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

chemin=$1
station=$2
consommateur=$3
error=0
if [ ! -f "$chemin" ]; then
  echo "ERREUR: Le fichier \"$chemin\" n'existe pas"
  error=1
fi

id_station=""
case $station in
  hvb) id_station=2 ;;
  hva) id_station=3 ;;
  lv) id_station=4 ;;
  *) echo "ERREUR: La station doit être \"hvb\", \"hva\" ou \"lv\". Vous avez saisi \"$station\"" ;;
esac

id_consommateur=""
case $consommateur in
  comp) id_consommateur=4 ;;
  indiv) id_consommateur=5 ;;
  all) id_consommateur="both" ;;
  *) echo "ERREUR: Le consommateur doit être \"comp\", \"indiv\" ou \"all\". Vous avez saisi \"$consommateur\"" ;;
esac

# combinaisons_invalides=("hvb-all" "hvb-indiv" "hva-all" "hva-indiv")
# if in_array "$station-$consommateur" "${combinaisons_invalides[@]}"; then
if in_array "$station-$consommateur" hvb-all hvb-indiv hva-all hva-indiv; then
  echo "ERREUR: Vous ne pouvez pas avoir un consommateur \"$consommateur\" avec une station \"$station\""
  error=1
fi

if [ $error -eq 1 ]; then
  aide
  exit 1
fi
