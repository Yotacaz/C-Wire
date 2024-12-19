#include "station.h"

/fonction qui renvoie les données au shell sous la bonne forme
void traiter_station(Donnee_station station){
    printf("%lu:%lu:%lu\n",station.ID_station, station.capacite, station.consomation);
}
