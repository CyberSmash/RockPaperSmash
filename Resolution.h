//
// Created by Jordan Sebastian on 5/30/23.
//

#ifndef ROCKPAPERSCISSORS_RESOLUTION_H
#define ROCKPAPERSCISSORS_RESOLUTION_H

#include "UnitType.h"

class Resolution {
public:
    Resolution(int row, int col, UnitType type);
    int row;
    int col;
    UnitType type;
};


#endif //ROCKPAPERSCISSORS_RESOLUTION_H
