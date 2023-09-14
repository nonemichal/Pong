//
// Created by michal on 9/12/23.
//

#ifndef PONG_PLAYER_H
#define PONG_PLAYER_H

#include "Rec.h"

class Player {
    int points;
public:
    Rec rec;

    Player(Rec rectangle);

    int get_points() {
        return points;
    }

    Rec operator++() {
        ++points;
    }
};

#endif //PONG_PLAYER_H
