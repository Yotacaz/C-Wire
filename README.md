# **C-Wire**

**Un projet de groupe d'école pour gérer la distribution sur des réseaux électriques de plusieurs centrales.** (groupe MI3-K)
Ce projet permet de visualiser la capacité en kWh d'un type de station de distribution d'un réseau électrique et de la consommation des utilisateurs directements branché à celle-ci.

## Utilisation

Pour commencer, ouvrez ce repertoire sur une console, puis executez le fichier shell **c-wire.sh** avec les options voulues :

```shell
bash c-wire.sh <chemin> <station> <consommateur> [<centrales>]
```

- `chemin` du fichier .csv à traiter (par défaut): *input/nom_du_fichier.csv*
- `station` à traiter parmis: *hva,hvb,lv*
- `consomateurs` : *all, indiv, comp*
- `centrales` (optionnel) : *n° des centrale* pour lesquelles le traitement est effectué (séparés par des espaces)

En cas d'utilisation en `lv all`, un fichier supplémentaire sera généré, avec les 10 postes lv consommant le plus d'énergie comparé à leur capacité, et les 10 postes lv consommant le moins d'énergie par rapport à leur capacité.


## Exemples d'utilisation

- *bash c-wire.sh input/data.csv lv all*
- *bash c-wire.sh input/data.csv hva comp 1 2*


## Notes

**Le programme c n'est pas recompilé à chaque fois** (en accord avec les consignes données), en cas de problème lié à cela, supprimez le fichier executable *'main'* (dans le dossier codeC) ou utilisez la commande

```shell
make -s -C codeC clean
```

Le **fichier de données d’entrée** est situé dans le dossier *‘input’*
Le programme C et tous les fichiers qui s’y rapportent (makefile,exécutable, …) sont situés dans le dossier *‘codeC’*
Aucun graphique n'est généré.
Aucun fichier temporaire n'est générés par le programe.
Les résultats des exécutions précédentes sont dans le dossier *‘tests’* (des résultats ont étés pré-générés avec le fichier c-wire_v00.dat dans le dossier *'input'*).
