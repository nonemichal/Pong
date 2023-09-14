//
// Created by michal on 9/12/23.
//

#ifndef PONG_BALL_H
#define PONG_BALL_H

#include <cstdlib>
#include <ctime>
#include <cstdlib>
#include "Obj.h"
#include "Rec.h"
#include "../defines.h"

enum class Collision {
    no_collision,
    rec_collision,
    player1,
    player2
};

class Ball : public Obj {
public:
    int vel_x, vel_y;

    Ball(int x = (SCREEN_WIDTH - BALL_SIZE) / 2, int y = (SCREEN_HEIGHT - BALL_SIZE) / 2, int w = BALL_SIZE, int h = BALL_SIZE, int c = 0xffcc80, int vx = 1, int vy = 1);
    Collision detect_collision(Rec &rec1, Rec &rec2);

    Ball operator++() {
        pos_x += vel_x;
        pos_y += vel_y;
    }
};


#endif //PONG_BALL_H
