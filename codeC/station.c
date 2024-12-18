#include <stdio.h>

#include "station.h"

void traiter_station(Donnee_station station){
    printf("%lu:%lu:%lu\n",station.ID_station, station.capacite, station.consomation);
}