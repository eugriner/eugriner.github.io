/* programa teste-make.c */

#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <cmath>

void display(void);
void timer(int);
void keyboard(unsigned char key, int x, int y);
void HSItoRGB(int h, float s, float i);
float rad(float a);

/* cores do quadrado */
GLfloat r=0.0, g=0.0, b=0.0;
float s, i;
int h;

int main(int argc, char** argv){
  //srand(time(NULL));
  //r = (float)rand() / (float)RAND_MAX ;
  //g = (float)rand() / (float)RAND_MAX ;
  //b = (float)rand() / (float)RAND_MAX ;
  h = 0;
  s = 1.0;
  i = 0.5;

  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize (256, 256); 
  glutInitWindowPosition (100, 100); 
  glutCreateWindow (argv[0]);
  // inicia um temporizador. após 33ms ativa a funcao timer
  glutTimerFunc(33, timer, 1);
  glClearColor(1.0, 1.0, 1.0, 0.0);
  //glShadeModel (GL_FLAT);
  glOrtho (0, 1, 0, 1, -1 ,1);
  glutDisplayFunc(display);
  glutMainLoop();
  return 0;
}

void timer(int value){
  h = h + 1;
  printf("h=%d\n", h);
  if(h>90) h=0;
  glutPostRedisplay();
  glutTimerFunc(100, timer, 1);
}

void display(void){
  glClear(GL_COLOR_BUFFER_BIT);
  glBegin(GL_POLYGON);
  
  //v1
  //
  //.
  HSItoRGB(h, s, i);
  glColor3f (r, g, b);
  printf("(%f,%f,%f)\n",r,g,b );
  glVertex2f(0.25,0.25);
  
  //v2
  //
  //.  .
  HSItoRGB(h+180, s, i);
  glColor3f (r, g, b);
  printf("(%f,%f,%f)\n",r,g,b );
  glVertex2f(0.75,0.25);
  
  //v3
  //   .
  //.  .   
  HSItoRGB(h+90, s, i);
  glColor3f (r, g, b);
  printf("(%f,%f,%f)\n",r,g,b );
  glVertex2f(0.75,0.75);
  
  //v4
  //.  .
  //.  . 
  HSItoRGB(h+270, s, i);
  glColor3f (r, g, b);
  printf("(%f,%f,%f)\n",r,g,b );
  glVertex2f(0.25,0.75);
  glEnd();
  glFlush();
}

void HSItoRGB(int h, float s, float i) {
  if (h>= 0 && h < 120)
  {
    b = i*(1-s)/3;
    r = i*(1+(s*cos(rad(h))/cos(rad(60-h))))/3;
    g = i*(1-(r+b));
  } else if (h < 240)
  {
    h = h-120;
    r = i*(1-s)/3;
    g = i*(1+(s*cos(rad(h))/cos(rad(60-h))))/3;
    b = i*(1-(r+g));
  } else if (h <= 360)
  {
    h = h-240;
    g = i*(1-s)/3;
    b = i*(1+(s*cos(rad(h))/cos(rad(60-h))))/3;
    r = i*(1-(g+b));
  } else {
    printf("\nERROR - Invalid Color!!\n");
  }
}
float rad(float a) {
  return a*M_PI/180.0;
}
