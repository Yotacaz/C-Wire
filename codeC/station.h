#pragma once
#include <stdio.h>

typedef struct _donneeStation             //raccourcie qui ralonge
{
    unsigned long ID_station;
    unsigned long conso;
    unsigned long capacite;
} Donnee_station;

void traiterStation (Donnee_station station);
