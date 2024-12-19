#pragma once
#include <stdio.h>

typedef struct _donneeStation             //raccourcie qui ralonge
{
    unsigned long ID_station;
    unsigned long conso;
    unsigned long capacite;
} Donnee_station;

void traiter_station (Donnee_station station);
