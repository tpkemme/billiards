#ifndef __BALL_H__
#define __BALL_H__

#include "./geom2.h"
#include "./vector.h"

#define B_RADIUS 15
#define H_RADIUS 30
// Change this (TINY changes) to change friction / deceleration.
// Higher number = slicker surface.  Lower number = more like sandpaper.
#define B_DECEL  0.9965

// A handy Ball class
class Ball {
   private:
   public:
double x, y;     // Position variables
double px, py;   // Backtrack variables
float r, g, b;   // Color variables
Vector *vector;

Ball(float bx, float by, float br, float bg, float bb);
void drawBall();
void drawHole();
bool intersects(Ball *other);
bool intersectsHole(Ball *other);
};

#endif
