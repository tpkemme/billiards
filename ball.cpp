#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "./ball.h"
#include "./common.h"
// Handy ball class implementation

Ball::Ball(float bx, float by, float br, float bg, float bb) {
    x = bx, px = bx;
    y = by, py = by;
    r = br;
    g = bg;
    b = bb;
    vector = new Vector(0, 0);
}

bool Ball::intersects(Ball *other) {
    double result = (x-other->x)*(x-other->x)+(y-other->y)*(y-other->y);
    double radSq = (B_RADIUS + B_RADIUS)*(B_RADIUS + B_RADIUS)+15;
    if (result <= radSq)
        return true;
    return false;
}

bool Ball::intersectsHole(Ball *other) {
    double result = (x-other->x)*(x-other->x)+(y-other->y)*(y-other->y);
    double radSq = (B_RADIUS + H_RADIUS)*(B_RADIUS + H_RADIUS);
    if (result <= radSq)
        return true;
    return false;
}

void Ball::drawBall() {
  glBegin(GL_TRIANGLE_FAN);
  glColor3f(r, g, b);
  px = x;
  py = y;
  x+= vector->x;
  y+= vector->y;
  // double ang = atan(y/x);
  // printf("Vx prev: %f\n", vector->x);
  vector->x *= B_DECEL;
  vector->y *= B_DECEL;
  // printf("Vx afte: %f\n", vector->x);
  for (int angle = 0; angle < 360; angle++) {
    glVertex2f(x + sin(angle)*B_RADIUS, y + cos(angle)*B_RADIUS);
  }
  glEnd();
}

void Ball::drawHole() {
  glBegin(GL_TRIANGLE_FAN);
  glColor3f(r, g, b);
  x+= vector->x;
  y+= vector->y;
  // double ang = atan(y/x);
  // printf("Vx prev: %f\n", vector->x);
  vector->x *= B_DECEL;
  vector->y *= B_DECEL;
  // printf("Vx afte: %f\n", vector->x);
  for (int angle = 0; angle < 360; angle++) {
    glVertex2f(x + sin(angle)*H_RADIUS, y + cos(angle)*H_RADIUS);
  }
  glEnd();
}
