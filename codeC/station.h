#pragma once
#include <stdio.h>

typedef struct _donneeStation // raccourcie qui ralonge
{
    unsigned long ID_station;
    unsigned long conso;
    unsigned long capacite;
} Donnee_station;

// fonction qui renvoie les données au shell sous la bonne forme
#define traiter_station(station) printf("%lu:%lu:%lu\n",station.ID_station, station.capacite, station.conso)

#define traiter_station_minmax(fichier, station) fprintf(fichier,"%lu:%lu:%lu:%ld\n",station.ID_station, station.capacite, station.conso, station.capacite-station.conso)
