# **C-Wire**

**Un projet de groupe d'école pour gérer la distribution sur des réseaux électriques de plusieurs centrales.** (groupe MI3-K)
Ce projet permet de visualiser la capacité en kWh d'un type de station de distribution d'un réseau électrique et de la consommation des utilisateurs directements branché à celle-ci.

Pour commencer, ouvrez ce repertoire sur une console, puis executez le fichier shell **c-wire.sh** avec les options voulues :

```shell
bash c-wire.sh <chemin> <station> <consommateur> [<centrales>]
```

- `chemin` du fichier .csv à traiter (par défaut): *input/nom_du_fichier.csv*
- `station` à traiter parmis: *hva,hvb,lv*
- `consomateurs` : *all, indiv, comp*
- `centrales` (optionnel) : *n° de la centrale* pour laquel le traitement est effectué

En cas d'utilisation en `lv all`, un fichier supplémentaire sera généré, avec les 10 postes lv consommant le plus d'énergie comparé à leur capacité, et les 10 postes lv consommant le moins d'énergie par rapport à leur capacité.

Exemples d'utilisation :

- *bash c-wire.sh input/data.csv lv all*
- *bash c-wire.sh input/data.csv hva comp 1*

Notes:

Le fichier de données d’entrée est situé dans le dossier *‘input’*
Le programme C et tous les fichiers qui s’y rapportent (makefile,exécutable, …) sont situés dans le dossier *‘codeC’*
Les graphiques, sont stockés dans des images sur le disque dur dans un dossier *‘graphs’*
Les fichiers temporaire générés par le programe sont crées *'dans'* tmp avant d'être supprimés.
Les résultats d’exécutions précédentes sont dans le dossier *‘tests’*.
