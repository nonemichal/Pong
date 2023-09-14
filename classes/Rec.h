//
// Created by michal on 9/12/23.
//

#ifndef PONG_REC_H
#define PONG_REC_H

#include "Obj.h"
#include "../defines.h"

class Rec : public Obj {
public:
    int vel;

    Rec(int x = 0, int y = 0, int w = REC_WIDTH, int h = REC_HEIGHT, int v = VELOCITY, int c = 0x2596be);

    Rec operator++() {
        if (pos_y < SCREEN_HEIGHT - REC_HEIGHT) {
            pos_y += vel;
        }
    }
    Rec operator--() {
        if (pos_y > 0) {
            pos_y -= vel;
        }
    }
};


#endif //PONG_REC_H
