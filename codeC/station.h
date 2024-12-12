#pragma once

typedef struct _donneeStation             //raccourcie qui ralonge
{
    int ID_station;
    int consomation;
    int capacite;
} Donnee_station;

void traiterStation (Donnee_station station);