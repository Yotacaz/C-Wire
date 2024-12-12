#include <stdio.h>

#include "station.h"

void traiter_station(Donnee_station station){
    printf("%u:%u:%u\n",station.ID_station, station.capacite, station.consomation);
}