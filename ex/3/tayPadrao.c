#include <GL/glut.h>
#include <stdlib.h>

GLubyte tux[] = {
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x80, 0x08, 0x00,
  0x00, 0x80, 0x08, 0x00,
  0x00, 0x80, 0x08, 0x00,
  0x00, 0x80, 0x08, 0x00,
  0x00, 0x80, 0x08, 0x00,
  0x00, 0x80, 0x08, 0x00,
  0x00, 0x80, 0x08, 0x00,
  0x00, 0x80, 0x08, 0x00,
  0x00, 0x80, 0x08, 0x00,
  0x00, 0x80, 0x08, 0x00,
  0x00, 0x80, 0x0F, 0xF8,
  0x00, 0x80, 0x08, 0x08,
  0x00, 0x80, 0x08, 0x08,
  0x00, 0x80, 0x08, 0x08,
  0x00, 0x80, 0x08, 0x08,
  0x3F, 0xFE, 0x0F, 0xF8,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00
};

GLfloat r, g, b;

void init(void);
void display(void);
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize (300, 300);
  glutInitWindowPosition (100, 100);
  glutCreateWindow ("Preenchendo regiões");
  init();
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutMainLoop();
  return 0;
}

void init(void) {
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glOrtho (0, 300, 0, 300, -1 , 1);
  r = 0; g = 0; b = 1;
}

void display(void) {
  int i;
  glClear(GL_COLOR_BUFFER_BIT);
  glDisable(GL_POLYGON_STIPPLE);

  glEnable(GL_POLYGON_STIPPLE);
  glColor3f(r, g, 1.0);
  glPolygonStipple(tux);
  glBegin(GL_POLYGON);
  glVertex2i(100, 63); glVertex2i(50, 150); glVertex2i(100, 237);
  glVertex2i(200, 237); glVertex2i(250, 150); glVertex2i(200, 63);
  glEnd();
  glFlush();
  glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
  switch (key) {
  case 27:
    exit(0);
    break;
  }
}

void mouse(int button, int state, int x, int y) {
  switch (button) {
  case GLUT_LEFT_BUTTON:
    if (state == GLUT_DOWN) {
      r = (GLfloat)rand() / (RAND_MAX + 1.0);
      g = (GLfloat)rand() / (RAND_MAX + 1.0);
      b = (GLfloat)rand() / (RAND_MAX + 1.0);
      glutPostRedisplay();
    }
    break;
  }
}
