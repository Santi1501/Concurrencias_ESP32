#include "data.h"

AvionCommand ledCommands[50] = {
    
    Avion_3, Avion_2, Avion_4, Avion_5, Avion_3,
    Avion_2, Avion_4, Avion_3, Avion_5, Avion_2,
    Avion_4, Avion_3, Avion_5, Avion_2, Avion_3,
    Avion_4, Avion_5, Avion_2, Avion_3, Avion_4,
    Avion_5, Avion_2, Avion_3, Avion_4, Avion_5,
    Avion_2, Avion_3, Avion_4, Avion_5, Avion_2,
    Avion_3, Avion_4, Avion_5, Avion_2, Avion_3,
    Avion_4, Avion_5, Avion_2, Avion_3, Avion_4,
    Avion_5, Avion_2, Avion_3, Avion_4, Avion_5,
    Avion_2, Avion_3, Avion_4, Avion_5, Avion_2
};

AvionCommand randomCommand() {
    
    return AvionCommands[random(0, 50)];
}