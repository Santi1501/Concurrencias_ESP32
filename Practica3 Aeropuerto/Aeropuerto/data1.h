#ifndef DATA_H
#define DATA_H

#include <Arduino.h>

enum AvionCommand {
    Avion_1, 
    Avion_2, 
    Avion_3, 
    Avion_4,
    Avion_5 
};

AvionCommand randomCommand();

#endif 