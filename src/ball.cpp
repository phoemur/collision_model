#include "ball.h"
#include "mainwindow.h"

#include <algorithm>
#include <cmath>
#include <iterator>

std::set<Ball*> Ball::pool {};

Ball::Ball(double px, double py, double sx, double sy)
    : texture{"./ball_assets/ball.png"},
      posData{px, py, texture.get_width() / 2},
      movData{sx, sy, 0.0, 0.0}
{
    insert_this();
}

Ball::~Ball() noexcept
{
    remove_this();
}

void Ball::move()
{
    // Update position
    posData.pos_x += movData.speed_x;
    posData.pos_y += movData.speed_y;

    // Update Speed
    movData.speed_x += movData.accel_x;
    movData.speed_y += movData.accel_y;
}

void Ball::render()
{
    texture.render(static_cast<int>(posData.pos_x - posData.radius),
                   static_cast<int>(posData.pos_y - posData.radius));
}

void Ball::move_all()
{
    for (Ball* b: pool) {
        b->move();
    }
}

void Ball::render_all()
{
    for (Ball* b: pool) {
        b->render();
    }
}

inline bool checkUpper(Ball* b)
{
    if (b->posData.pos_y > b->posData.radius) {
        return false;
    }

    // We have a collision
    b->posData.pos_y = b->posData.radius;
    b->movData.speed_y *= -1;
    b->movData.accel_y *= -1;

    return true;
}

inline bool checkLower(Ball* b)
{
    auto& win = MainWindow::instance();
    auto top = win.getHeight() - b->posData.radius;

    if (b->posData.pos_y < top) {
        return false;
    }

    // Collision
    b->posData.pos_y = top;
    b->movData.speed_y *= -1;
    b->movData.accel_y *= -1;

    return true;
}

inline bool checkRight(Ball* b)
{
    auto& win = MainWindow::instance();
    auto right = win.getWidth() - b->posData.radius;

    if (b->posData.pos_x < right) {
        return false;
    }

    // Collision
    b->posData.pos_x = right;
    b->movData.speed_x *= -1;
    b->movData.accel_x *= -1;

    return true;
}

inline bool checkLeft(Ball* b)
{
    if (b->posData.pos_x > b->posData.radius) {
        return false;
    }

    // We have a collision
    b->posData.pos_x = b->posData.radius;
    b->movData.speed_x *= -1;
    b->movData.accel_x *= -1;

    return true;
}

inline void checkBounds(Ball* b)
{
    if (!checkUpper(b)) {checkLower(b);}
    if (!checkLeft(b)) {checkRight(b);}
}

inline bool collided(Ball* a, Ball* b)
{
    return std::hypot(a->posData.pos_x - b->posData.pos_x,
                      a->posData.pos_y - b->posData.pos_y) < a->posData.radius + b->posData.radius;
}

// https://www.plasmaphysics.org.uk/programs/coll2d_cpp.htm
void collision2Ds(double m1, double m2, double R,
                  double x1, double y1, double x2, double y2,
                  double& vx1, double& vy1, double& vx2, double& vy2)
{
    double  m21,dvx2,a,x21,y21,vx21,vy21,fy21,sign,vx_cm,vy_cm;

    m21=m2/m1;
    x21=x2-x1;
    y21=y2-y1;
    vx21=vx2-vx1;
    vy21=vy2-vy1;

    vx_cm = (m1*vx1+m2*vx2)/(m1+m2) ;
    vy_cm = (m1*vy1+m2*vy2)/(m1+m2) ;


    //     *** return old velocities if balls are not approaching ***
    if ( (vx21*x21 + vy21*y21) >= 0) return;


    //     *** I have inserted the following statements to avoid a zero divide;
    //         (for single precision calculations,
    //          1.0E-12 should be replaced by a larger value). **************

    fy21=1.0E-12*std::fabs(y21);
    if ( std::fabs(x21)<fy21 ) {
        if (x21<0) { sign=-1; }
        else { sign=1;}
        x21=fy21*sign;
    }

    //     ***  update velocities ***
    a=y21/x21;
    dvx2= -2*(vx21 +a*vy21)/((1+a*a)*(1+m21)) ;
    vx2=vx2+dvx2;
    vy2=vy2+a*dvx2;
    vx1=vx1-m21*dvx2;
    vy1=vy1-a*m21*dvx2;

    //     ***  velocity correction for inelastic collisions ***
    vx1=(vx1-vx_cm)*R + vx_cm;
    vy1=(vy1-vy_cm)*R + vy_cm;
    vx2=(vx2-vx_cm)*R + vx_cm;
    vy2=(vy2-vy_cm)*R + vy_cm;

    return;
}

void Ball::check_collisions()
{
    for (auto it = pool.begin(); it != pool.end(); std::advance(it, 1)) {
        checkBounds(*it);
        for (auto it2 = std::next(it); it2 != pool.end(); std::advance(it2, 1)) {
            if (collided(*it, *it2)) {
                // Solve resulting collision
                collision2Ds(10, 10, 1.0, (*it)->posData.pos_x, (*it)->posData.pos_y, (*it2)->posData.pos_x, (*it2)->posData.pos_y,
                             (*it)->movData.speed_x, (*it)->movData.speed_y, (*it2)->movData.speed_x, (*it2)->movData.speed_y);
            }
        }
    }
}
