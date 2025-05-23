#ifndef DATA_H
#define DATA_H

#include <Arduino.h>

enum AvionCommand {
    Avion_1 = 1, 
    Avion_2 = 2, 
    Avion_3 = 3, 
    Avion_4 = 4,
    Avion_5 = 5 
};

AvionCommand randomCommand();

#endif 