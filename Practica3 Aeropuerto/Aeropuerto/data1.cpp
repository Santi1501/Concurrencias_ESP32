#include "data1.h"

AvionCommand AvionCommands[50] = {
    
    Avion_1, Avion_2, Avion_4, Avion_5, Avion_3,
    Avion_2, Avion_4, Avion_3, Avion_5, Avion_1,
    Avion_4, Avion_1, Avion_5, Avion_2, Avion_3,
    Avion_1, Avion_5, Avion_2, Avion_3, Avion_4,
    Avion_5, Avion_2, Avion_3, Avion_4, Avion_1,
    Avion_1, Avion_3, Avion_4, Avion_5, Avion_2,
    Avion_3, Avion_4, Avion_5, Avion_2, Avion_1,
    Avion_4, Avion_5, Avion_2, Avion_3, Avion_4,
    Avion_1, Avion_2, Avion_3, Avion_4, Avion_5,
    Avion_2, Avion_3, Avion_4, Avion_5, Avion_1
};

AvionCommand randomCommand() {
    
    return AvionCommands[random(0, 50)];
}