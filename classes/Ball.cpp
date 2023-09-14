//
// Created by michal on 9/12/23.
//

#include "Ball.h"

Ball::Ball(int x, int y, int w, int h, int c, int vx, int vy) {
    pos_x = x;
    pos_y = y;
    rec_w = w;
    rec_h = h;
    color = c & 0x00ffffff;
    vel_x = (rand() % 2) ? vx : -vx;
    vel_y = (rand() % 2) ? vy : -vy;
}

Collision Ball::detect_collision(Rec &rec1, Rec &rec2) {
    if (pos_y + rec_w == SCREEN_HEIGHT) {
        vel_y *= -1;
    } else if (pos_y == 0) {
        vel_y *= -1;
    }

    if ((pos_x + rec_w  == rec2.get_x()) &&
            (pos_y + rec_w  > rec2.get_y()) &&
            (pos_y < rec2.get_y() + rec2.get_h())) {
        vel_x *= -1;
        return Collision::rec_collision;
    } else if ((pos_x == rec1.get_x() + rec1.get_w()) &&
            (pos_y + rec_w > rec1.get_y()) &&
            (pos_y < rec1.get_y() + rec1.get_h())) {
        vel_x *= -1;
        return Collision::rec_collision;
    } else if (pos_x == SCREEN_WIDTH - rec_w ) {
        return Collision::player1;
    } else if (pos_x == 0) {
        return Collision::player2;
    }

    return Collision::no_collision;
}