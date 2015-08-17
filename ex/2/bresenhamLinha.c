/*Implemente o algoritmo de Bresenham para traçado de
linhas, utilizando GL_POINTS como parâmetro da função
glBegin(). Este parâmetro indica que cada vértice deve
ser tratado como um ponto simples. Utilizando o
algoritmo implementado, desenhe uma reta verde do ponto
(x, y)=(40, 200) ao ponto (x, y)=(200, 10).*/
#include <GL/glut.h>
#include <stdlib.h>
#include <cmath>
#include <stdio.h>

void init(void);
void display(void);
void keyboard(unsigned char key, int x, int y);
void bresenDraw(int x1, int y1, int x2, int y2);
void trash(void);

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize (256, 256);
  glutInitWindowPosition (100, 100);
  glutCreateWindow ("Desenhando uma linha");
  init();
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutMainLoop();
  return 0;
}

void init(void) {
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glOrtho (0, 256, 0, 256, -1 , 1);
}

void display(void) {
  int i;
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f (0.0, 0.0, 0.0);
  glBegin(GL_POINTS);
  int x0 = 40;
  int x1 = 200;
  int y0 = 200;
  int y1 = 10;

  //bresenDraw(x0, y0, x1, y1);
  //bresenDraw(40, 40, 200, 150);
  bresenDraw(40, 200, 200, 10);

  glEnd();
  glFlush();
}

void keyboard(unsigned char key, int x, int y) {
  switch (key) {
  case 27:
    exit(0);
    break;
  }
}

void bresenDraw(int x1, int y1, int x2, int y2) {
  int x = x1;
  int y = y1;
  int Delta_x = abs(x2 - x1);
  int Delta_y = abs(y2 - y1);
  int s1 = (x2 > x1) ? 1 : -1;
  int s2 = (y2 > y1) ? 1 : -1;
  int Troca, i;
  if (Delta_y > Delta_x) {
    int Temp = Delta_x;
    Delta_x = Delta_y;
    Delta_y = Temp;
    Troca = 1;
  }
  else {
    Troca = 0;
  }
  int new_e = 2 * Delta_y - Delta_x;
  for (i = 1; i <= Delta_x; i++) {
    glVertex2i(x, y);
    while (new_e >= 0) {
      if (Troca == 1) {
        //Muda para a proxima linha de rasterização
        x = x + s1;
      }
      else {
        y = y + s2;
      }
      new_e = new_e - 2 * Delta_x;
    }

    //Permanece nesta linha de rasterização
    if (Troca == 1) {
      y = y + s2;
    }
    else {
      x = x + s1;
    }
    new_e = new_e + 2 * Delta_y;
  }
}