//
// Created by michal on 9/12/23.
//

#include "Obj.h"

int Obj::get_color(Color c) {
    switch(c) {
        case RED:
            return (color >> 4) & 0xff;
        case GREEN:
            return (color >> 2) & 0xff;
        case BLUE:
            return color & 0xff;
        default:
            return 0;
    }
}