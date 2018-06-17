#ifndef BALL_H
#define BALL_H

#include "texture.h"

#include <set>

struct MovementData {
    double speed_x;
    double speed_y;
    double accel_x;
    double accel_y;
};

struct PositionData {
    double pos_x;
    double pos_y;
    double radius;
};

class Ball  {
    static std::set<Ball*> pool;

    Texture texture;
public:
    PositionData posData;
    MovementData movData;

    Ball(double px, double py, double sx, double sy);
    ~Ball() noexcept;

    void move();
    void render();

    // Static Methods to wwork on the pool of balls
    static void move_all();
    static void render_all();
    static void check_collisions();

private:
    void insert_this() {pool.insert(this);}
    void remove_this() {pool.erase(this);}
};

#endif // BALL_H
