//
// Created by michal on 9/12/23.
//

#ifndef PONG_OBJ_H
#define PONG_OBJ_H

enum Color {
    RED,
    GREEN,
    BLUE,
};

class Obj {
protected:
    int pos_x, pos_y, rec_w, rec_h, color;
public:
    int get_color(Color c);

    int get_x() {
        return pos_x;
    }
    int get_y() {
        return pos_y;
    }
    int get_w() {
        return rec_w;
    }
    int get_h() {
        return rec_h;
    }
};


#endif //PONG_OBJ_H
