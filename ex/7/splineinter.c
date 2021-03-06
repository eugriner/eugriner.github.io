#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

// To work better with vectors
#define X 0
#define Y 1
#define Z 2
#define MINDISTANCE 0.5
#define INTLIMIT 32000

GLint eixox, eixoy, eixoz;

GLint nVertices = 4;
GLfloat vertices[4][4][3];

GLint largura, altura;

GLint mudaCurva = 0;
GLint verticeCorrente = 0;

enum {BEZIER, NURBS};
GLint spline;
GLUnurbsObj *nc;
GLfloat nos[8] = {0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0};
GLint nNos = 8;

GLint matrizViewport[4];
GLdouble matrizModelview[16], matrizProjecao[16];
GLint yreal;  /*  posição da coordenada y no OpenGL */
GLdouble wx, wy, wz;  /*  coordenadas no mundo real: x, y, z  */
GLdouble winx, winy, winz;  /*  coordenadas na janela: x, y, z  */
GLdouble clickPos[3];
GLdouble pmin[3]; // armazena o valor minimo dos p's
GLdouble w1[3], w2[3]; // ponto inicial e ponto final
GLint vc[2] = {0, 0}; // vertice selecionado para movimentacao

void gera_superficie(void) {
  int i, j;
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      vertices[i][j][0] = 2.0 * ((GLfloat)i - 1.5);
      vertices[i][j][1] = 2.0 * ((GLfloat)j - 1.5);

      if ( (i == 1 || i == 2) && (j == 1 || j == 2))
        vertices[i][j][2] = 7.0;
      else
        vertices[i][j][2] = -3.0;
    }
  }
}
void display(void) {
  int i, j;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix();
  glRotatef(eixox, 1 , 0 , 0);
  glRotatef(eixoy, 0 , 1 , 0);
  glRotatef(eixoz, 0 , 0 , 1);
  glScalef(0.25, 0.25, 0.25);

  glGetIntegerv(GL_VIEWPORT, matrizViewport);
  glGetDoublev(GL_MODELVIEW_MATRIX, matrizModelview);
  glGetDoublev(GL_PROJECTION_MATRIX, matrizProjecao);

  glDisable(GL_LIGHTING);
  glPushMatrix();
  glTranslatef(-5, -5, -5);
  glColor3f(1, 1, 1);
  glBegin(GL_LINES);
  glVertex3f(0, 0, 0);  glVertex3f(1, 0, 0);
  glEnd();
  glRasterPos3f(1.5, 0, 0);
  glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, 'x');

  glBegin(GL_LINES);
  glVertex3f(0, 0, 0);  glVertex3f(0, 1, 0);
  glEnd();
  glRasterPos3f(0, 1.5, 0);
  glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, 'y');

  glBegin(GL_LINES);
  glVertex3f(0, 0, 0);  glVertex3f(0, 0, 1);
  glEnd();
  glRasterPos3f(0, 0, 1.5);
  glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, 'z');

  glPopMatrix();
  glEnable(GL_LIGHTING);

  switch (spline) {
  case BEZIER:
    glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 4, 0, 1, 3 * nVertices, 4, &vertices[0][0][0]);
    glEnable(GL_AUTO_NORMAL);
    glMapGrid2f(20, 0, 1.0, 20, 0, 1.0);
    glEvalMesh2(GL_FILL, 0, 20, 0, 20);
    break;
  case NURBS:
    gluBeginSurface(nc);
    gluNurbsSurface(nc, nNos, nos, nNos, nos, 4 * 3, 3, &vertices[0][0][0], 4, 4, GL_MAP2_VERTEX_3);
    gluEndSurface(nc);
    break;
  }
  glPointSize(5.0);
  glColor3f(1.0, 0.0, 0.0);
  glDisable(GL_LIGHTING);
  glBegin(GL_POINTS);
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      glVertex3fv(&vertices[i][j][0]);
    }
  }
  glEnable(GL_LIGHTING);
  glEnd();

  glPopMatrix();
  glFlush();

  glutSwapBuffers();
}

void init(void) {
  GLfloat mat_diffuse[] = { 0.7, 0.7, 0.7, 1.0 };
  GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat mat_shininess[] = { 100.0 };

  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_AUTO_NORMAL);
  glEnable(GL_NORMALIZE);

  gera_superficie();

  nc = gluNewNurbsRenderer();
  gluNurbsProperty(nc, GLU_SAMPLING_TOLERANCE, 5.0);
  gluNurbsProperty(nc, GLU_DISPLAY_MODE, GLU_FILL);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef (0.0, 0.0, -5.0);

  spline = NURBS;
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_MAP2_VERTEX_3);
  display();
}

void reshape(int w, int h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective (45.0, (GLdouble)w / (GLdouble)h, 3.0, 8.0);
  glMatrixMode(GL_MODELVIEW);
}

/* ARGSUSED1 */
void keyboard(unsigned char key, int x, int y) {
  switch (key) {
  case 'x':
    eixox = (eixox + 5) % 360;
    glutPostRedisplay();
    break;
  case 'y':
    eixoy = (eixoy + 5) % 360;
    glutPostRedisplay();
    break;
  case 'z':
    eixoz = (eixoz + 5) % 360;
    glutPostRedisplay();
    break;
  case 'X':
    eixox = (eixox - 5) % 360;
    glutPostRedisplay();
    break;
  case 'Y':
    eixoy = (eixoy - 5) % 360;
    glutPostRedisplay();
    break;
  case 'Z':
    eixoz = (eixoz - 5) % 360;
    glutPostRedisplay();
    break;
  case 'b':
    spline = BEZIER;
    glutPostRedisplay();
    break;
  case 'n':
    spline = NURBS;
    glutPostRedisplay();
    break;
  case 27:
    exit(0);
    break;
  }
}

void proximidade() {
  int i, j;
  double tam = 0, tamin = INTLIMIT;
  double dx, dy, dz, px, py, pz;
  double piv, pir;

  // Y = V.V
  piv = clickPos[X] * clickPos[X] + clickPos[Y] * clickPos[Y] + clickPos[Z] * clickPos[Z];

  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      
      // Z = (R - Po)
      dx = vertices[i][j][0] - w1[X];
      dy = vertices[i][j][1] - w1[Y];
      dz = vertices[i][j][2] - w1[Z];
      
      // X = Z.V
      pir = dx * clickPos[X] + dy * clickPos[Y] + dz * clickPos[Z];

      // Po + (X/Y).V
      px = w1[X] + pir / piv * clickPos[X];
      py = w1[Y] + pir / piv * clickPos[Y];
      pz = w1[Z] + pir / piv * clickPos[Z];
      
      // T = (xf-xi)^2 + (yf-yi)^2
      tam = (px - vertices[i][j][0]) * (px - vertices[i][j][0]) +
            (py - vertices[i][j][1]) * (py - vertices[i][j][1]) +
            (pz - vertices[i][j][2]) * (pz - vertices[i][j][2]);

      if ((tam < tamin) && (tam < MINDISTANCE)) {
        tamin = tam;
        vc[X] = i;
        vc[Y] = j;
        pmin[0] = px;
        pmin[1] = py;
        pmin[2] = pz;
      }
    }
  }
  if (tamin < MINDISTANCE) {
    gluProject(pmin[0], pmin[1], pmin[2],
               matrizModelview, matrizProjecao, matrizViewport,
               &winx, &winy, &winz);
    mudaCurva = 1;
  }
  else {
    mudaCurva = 0;
  }
}

void mouse(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

    yreal = matrizViewport[3] - (GLint) y - 1;

    gluUnProject ((GLdouble) x, (GLdouble) yreal, -1.0,
                  matrizModelview, matrizProjecao, matrizViewport,
                  &w1[X], &w1[Y], &w1[Z]);

    gluUnProject ((GLdouble) x, (GLdouble) yreal, 1.0,
                  matrizModelview, matrizProjecao, matrizViewport,
                  &w2[X], &w2[Y], &w2[Z]);

    clickPos[X] = w2[X] - w1[X];
    clickPos[Y] = w2[Y] - w1[Y];
    clickPos[Z] = w2[Z] - w1[Z];
    proximidade();

    glutPostRedisplay();
  } else {
    mudaCurva = 0;
  }
}

void motion(int x, int y) {
  // quando tiver clicado em ponto de controle
  if (mudaCurva) {
    // convert y para coordenadas de mundo
    yreal = matrizViewport[3] - (GLint) y - 1;
    // recupera a posicao do x, y, z do mouse
    gluUnProject((GLdouble) x, (GLdouble) yreal, winz,
                 matrizModelview, matrizProjecao, matrizViewport,
                 &wx, &wy, &wz);
    // altera a posicao do vertice de controle
    vertices[vc[X]][vc[Y]][0] = wx;
    vertices[vc[X]][vc[Y]][1] = wy;
    vertices[vc[X]][vc[Y]][2] = wz;
    //redesenha
    glutPostRedisplay();
  }
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(500, 500);
  glutInitWindowPosition(100, 100);
  glutCreateWindow(argv[0]);
  init();
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutMotionFunc(motion);
  glutMouseFunc(mouse);
  glutMainLoop();
  return 0;
}
