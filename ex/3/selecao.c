#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>

GLfloat rb[4], gb[4], bb[4], rf[4], gf[4], bf[4];
int qx[4], qy[4];
int posX, posY;
bool flagKey;
void init(void);
void display(void);
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void square(int qn);

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize (256, 256);
  glutInitWindowPosition (100, 100);
  glutCreateWindow ("Preenchendo regioes");
  init();
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutMainLoop();
  return 0;
}

void init(void) {
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glOrtho (0, 256, 0, 256, -1 , 1);
  for (int i = 0; i < 4; ++i)
  {
    rb[i] = 0; gb[i] = 0; bb[i] = 0;
    rf[i] = 1; gf[i] = 1; bf[i] = 0;
  }
  
  flagKey = 1;
  qx[0] = 30;
  qy[0] = 226;
  qx[1] = 143;
  qy[1] = 226;
  qx[2] = 30;
  qy[2] = 113;
  qx[3] = 143;
  qy[3] = 113;
  posX = 0; posY = 0;
}

void display(void) {
  int i;
  glClear(GL_COLOR_BUFFER_BIT);
  glDisable(GL_POLYGON_STIPPLE);

  square(0);
  square(1);
  square(2);
  square(3);

  glFlush();
  glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
  switch (key) {
  case 'b':
    flagKey = 0;
    break;
  case 'f':
    flagKey = 1;
    break;
  case 27:
    exit(0);
    break;
  }
}

void mouse(int button, int state, int x, int y) {
  posX = x;
  //O eixo y do rastreamento do mouse é o inverso do de desenho do openGL, why!!!???
  posY = (y-256)*-1;
  //posY = y;
  switch (button) {
  case GLUT_LEFT_BUTTON:
    if (state == GLUT_DOWN) { 
      //printf("(%d,%d)\n", posX, posY);
      glutPostRedisplay();
    }
    break;
  }
}

void square(int qn) {
  int x = qx[qn];
  int y = qy[qn];
  //printf("Qn=%d, X=%d, Y=%d\n",qn, x, y);
  //printf("(X=%d)\n", (x <= posX && (x+83) >= posX));
  //printf("(Y=%d)\n", (y <= posY && (y-83) >= posY));
  if ((x <= posX && (x+83) >= posX) && (y >= posY && (y-83) <= posY)) 
  {
    if (flagKey)
    {
      rf[qn] = (GLfloat)rand() / (RAND_MAX + 1.0);
      gf[qn] = (GLfloat)rand() / (RAND_MAX + 1.0);
      bf[qn] = (GLfloat)rand() / (RAND_MAX + 1.0);
    } else {
      rb[qn] = (GLfloat)rand() / (RAND_MAX + 1.0);
      gb[qn] = (GLfloat)rand() / (RAND_MAX + 1.0);
      bb[qn] = (GLfloat)rand() / (RAND_MAX + 1.0);
    }
  }
  glPolygonMode(GL_BACK, GL_FILL);
  glColor3f(rf[qn], gf[qn], bf[qn]);
  glBegin(GL_POLYGON);
  glVertex2i(x, y); glVertex2i(x + 83, y);
  glVertex2i(x + 83, y - 83); glVertex2i(x, y - 83);
  glEnd();

  glPolygonMode(GL_BACK, GL_LINE);
  glColor3f(rb[qn], gb[qn], bb[qn]);
  glBegin(GL_POLYGON);
  glVertex2i(x, y); glVertex2i(x + 83, y);
  glVertex2i(x + 83, y - 83); glVertex2i(x, y - 83);
  glEnd();
}
