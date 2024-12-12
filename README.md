# **C-Wire**
**Un projet de groupe d'école pour gérer la distribution sur des réseaux électriques de plusieurs centrales.**

Pour commencer, ouvrez ce repertoire sur une console, puis executez le fichier shell **c-wire.sh** avec les options voulues :
`bash c-wire.sh <chemin> <station> <consommateur> [<centrales>]`
- `chemin` du fichier .csv à traiter (par défaut): *input* 
- `station` à traiter parmis: *hva,hvb,lv*
- `consomateurs` : *all, indiv, comp*
- `centrales` (optionnel) : *n° de la centrale* pour laquel le traitement est effectué


Notes:
Le fichier de données d’entrée est situé dans le dossier *‘input’*
Le programme C et tous les fichiers qui s’y rapportent (makefile,exécutable, …) sont situés dans le dossier *‘codeC’*
Les graphiques, sont stockés dans des images sur le disque dur dans un dossier *‘graphs’*
Aucun fichiers temporaire n'est généré par le programe.
Les résultats d’exécutions précédentes sont dans le dossier *‘tests’*.
