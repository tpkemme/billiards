#include <sys/time.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <vector>
#include "./ball.h"
#include "./common.h"

#define SIZEOF_ARRAY(a)(sizeof(a)/sizeof(a[0]))

using namespace std;

Vector *cueVec;
Vector *clickPt;
Ball* holes[4];
Ball* balls[3];
Ball *cueBall;
Ball* allballs[4];

double dx;
int factor;
bool prevState;  // False = first time down, True = first time up

void animate(double speed);
// Timer function courtesy of http://stackoverflow.com/
static double ftime(void) {
    struct timeval t;
    gettimeofday(&t, NULL);
    return 1.0*t.tv_sec + 1e-6*t.tv_usec;
}

static double last_T;

static void idle(void) {
  const double now_T = ftime();
  const double delta_T = now_T - last_T;
  last_T = now_T;

  const double speed = delta_T * 60;

  animate(speed);

  glutPostRedisplay();
}

void animate(double speed) {
  if (dx > 1.5) {
    dx = -1.5;
  } else {
    dx += speed/factor;
  }
  glutPostRedisplay();
}

void Init() {
  dx = 0;
  factor = 100;
  prevState = false;
  cueVec = new Vector(0, 0);
  clickPt = new Vector(0, 0);
  glClearColor(0.0, .55, 0.0, 1.0);
  glColor3f(1.0, 0.0, 0.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(-200, 200, -150, 150);
  glMatrixMode(GL_MODELVIEW);

  holes[0] = new Ball(-175, 131.25, 0, 0, 0);
  holes[1] = new Ball(175, 131.25, 0, 0, 0);
  holes[2] = new Ball(-175, -131.25, 0, 0, 0);
  holes[3] = new Ball(175, -131.25, 0, 0, 0);

  cueBall = new Ball(0, 50, 1, 1, 1);

  balls[0] = new Ball(-50, 0, 1, 0, 0);
  balls[1] = new Ball(0, 0, 0, 1, 0);
  balls[2] = new Ball(50, 0, 0, 0, 1);

  allballs[0] = balls[0];
  allballs[1] = balls[1];
  allballs[2] = balls[2];
  allballs[3] = cueBall;

  printf("\nINITIALIZE IS DONE\n");
}

void Mouse(int button, int state, int x, int y) {
    // printf("HA\n");
    if (button ==  GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            if (!prevState) {  // First time down, set initial point
                prevState = true;
                clickPt->x = x;
                clickPt->y = y;
            }
            cueVec->x = 0;
            cueVec->y = 0;
        }
        if (state == GLUT_UP) {
            if (prevState) {  // First time up, set initial push
                prevState = false;
                cueBall->vector->x = -cueVec->x/300;
                cueBall->vector->y = -cueVec->y/300;
                cueVec->x = 0;
                cueVec->y = 0;
                clickPt->x = 0;
                clickPt->y = 0;
            }
        }
    }
}

void MouseMove(int x, int y) {
    // printf("LOL\n");
    if (prevState) {  // Mouse is down
        cueVec->x = x - clickPt->x;
        cueVec->y = - (y - clickPt->y);
    }
}

void Display() {
  glClear(GL_COLOR_BUFFER_BIT);
  // Builds table
  glColor3f(.4, .19, .0);
  glRectf(-200, 150, 200, 120);
  glRectf(-200, 150, -170, -200);
  glRectf(-200, -150, 200, -120);
  glRectf(200, 150, 170, -200);
  int i;
  // printf("I AM DISPLAY\n");
  glBegin(GL_LINES);
  glColor3f(1, 1, 1);
  glVertex2f(cueBall->x, cueBall->y);
  glVertex2f(cueBall->x+(cueVec->x/3.5), cueBall->y + (cueVec->y/3.5));
  glEnd();
  cueBall->drawBall();
  for (i = 0; i < SIZEOF_ARRAY(holes); i++) {
      holes[i]->drawHole();
  }
// ONLY DRAWING ONE BALL RIGHT NOW
  for (i = 0; i < SIZEOF_ARRAY(balls); i++) {
      balls[i]->drawBall();
  }

  for (i = 0; i < SIZEOF_ARRAY(allballs); i++) {
      if (allballs[i]->intersectsHole(holes[0])) {
        allballs[i]->x = -175;
        allballs[i]->y = 131.25;
      }
      if (allballs[i]->intersectsHole(holes[1])) {
        allballs[i]->x = 175;
        allballs[i]->y = 131.25;
      }
      if (allballs[i]->intersectsHole(holes[2])) {
        allballs[i]->x = -175;
        allballs[i]->y = -131.25;
      }
      if (allballs[i]->intersectsHole(holes[3])) {
        allballs[i]->x = 175;
        allballs[i]->y = -131.25;
      }
      if ((allballs[i]->x + 15) >= 170) {
        allballs[i]->vector->x *= -1;
      }
      if ((allballs[i]->x - 15) <= -170) {
        allballs[i]->vector->x *= -1;
      }
      if ((allballs[i]->y + 15) >= 120) {
        allballs[i]->vector->y *= -1;
      }
      if ((allballs[i]->y - 15) <= -120) {
        allballs[i]->vector->y *= -1;
      }
      int j;
      for (j = 0; j < SIZEOF_ARRAY(allballs); j++) {
        if (j == i)
          continue;
        if (allballs[i]->intersects(allballs[j])) {
            allballs[i]->x = allballs[i]->px;
            allballs[i]->y = allballs[i]->py;
            allballs[j]->x = allballs[j]->px;
            allballs[j]->y = allballs[j]->py;
            float deltX = (allballs[i]->x > allballs[j]->x)?
                (allballs[i]->x - allballs[j]->x):
                (allballs[j]->x - allballs[i]->x);
            float deltY = (allballs[i]->y > allballs[j]->y)?
                (allballs[j]->y - allballs[i]->y):
                (allballs[i]->y - allballs[j]->y);
            printf("deltX: %f     deltY: %f\n", deltX, deltY);
            printf("ivx: %f       jvx: %f\n", allballs[i]->vector->x,
                    allballs[j]->vector->x);
            printf("ivy: %f       jvy: %f\n", allballs[i]->vector->y,
                    allballs[j]->vector->y);
            float newmagX = (allballs[i]->vector->x + allballs[j]->vector->x)
              * .8;
            float newmagY = (allballs[i]->vector->y + allballs[j]->vector->y)
              * .8;
            printf("magX: %f     magY: %f\n", newmagX, newmagY);
            float angleInDegrees = atan2(deltY, deltX) * 180 / PI;
            printf("Angle: %f\n", angleInDegrees);
            allballs[j]->vector->x = newmagX*cos(angleInDegrees);
            allballs[j]->vector->y = newmagY*sin(angleInDegrees);
            allballs[i]->vector->x = newmagX*cos(angleInDegrees + 90);
            allballs[i]->vector->y = newmagY*sin(angleInDegrees - 90);
           // }
        }
      }
  }
  glutSwapBuffers();
  glFlush();
}

void Keyboard(unsigned char key, int x, int y) {
  switch (key) {
    case 'q':
      exit(EXIT_SUCCESS);
      break;
  }
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(800, 600);
  glutInitWindowPosition(0, 0);
  // DO NOT change the window title.  It is necessary for the screen capture.
  glutCreateWindow("Billiards");
  glutDisplayFunc(Display);
  glutIdleFunc(idle);
  glutPostRedisplay();
  glutMouseFunc(Mouse);
  glutMotionFunc(MouseMove);
  glutKeyboardFunc(Keyboard);
  Init();


  glutMainLoop();
}
