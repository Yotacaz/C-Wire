#include <stdio.h>
#pragma once

typedef struct _donneeStation             //raccourcie qui ralonge
{
    unsigned long ID_station;
    unsigned long consomation;
    unsigned long capacite;
} Donnee_station;

void traiterStation (Donnee_station station);
