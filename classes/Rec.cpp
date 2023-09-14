//
// Created by michal on 9/12/23.
//

#include "Rec.h"

Rec::Rec(int x, int y, int w, int h, int v, int c) {
    pos_x = x;
    pos_y = y;
    rec_w = w;
    rec_h = h;
    vel = v;
    color = c & 0x00ffffff;
}