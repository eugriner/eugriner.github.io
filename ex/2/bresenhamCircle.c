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
void bresenDraw(int x, int y, int raio);
void pontosDaCircunferencia(int x, int y, int xc, int yc);

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

  bresenDraw(128, 128, 50);

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

void bresenDraw(int xc, int yc, int raio) {
  int x = 0;
  int y = raio;
  int d = 1 - raio;
  pontosDaCircunferencia(x, y, xc, yc);
  while (y > x) {
    if (d < 0) {
      d = d + 2 * x + 3;
      x = x + 1;
    }
    else {
      d = d + 2 * (x - y) + 5;
      x = x + 1;
      y = y - 1;
    }
    pontosDaCircunferencia(x, y, xc, yc);
  }
}

void pontosDaCircunferencia(int x, int y, int xc, int yc) {
                    //Octantes
  glVertex2i(x+xc,y+yc);  //1
  glVertex2i(y+xc,x+yc);  //2
  glVertex2i(y+xc,-x+yc); //3
  glVertex2i(x+xc,-y+yc); //4
  glVertex2i(-x+xc,-y+yc);//5
  glVertex2i(-y+xc,-x+yc);//6
  glVertex2i(-y+xc,x+yc); //7
  glVertex2i(-x+xc,y+yc); //8
}