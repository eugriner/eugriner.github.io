#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

GLint nVertices=6;
GLfloat vertices[6][3] = {
  {-4.0,  0.0, 0.0}, 
  {-4.0, +4.0, 0.0}, 
  {+4.0, -4.0, 0.0}, 
  {-4.0, -4.0, 0.0}, 
  {+4.0, +4.0, 0.0}, 
  {+4.0,  0.0, 0.0}
};

GLint largura, altura;
GLint mudaCurva=0;
GLint verticeCorrente=0;

GLfloat esquerda=-5;
GLfloat direita =+5;
GLfloat fundo   =-5;
GLfloat topo    =+5;
GLfloat longe   =+5;
GLfloat perto   =-5;

enum {BEZIER, NURBS, N_UNIFORM, N_OPEN, N_NOT_UNIFORM};
GLint spline;
GLUnurbsObj *nc;

//ORDEM

GLint ORDEM=3;
GLint nNos=nVertices+ORDEM;
const int nknots = 9;
GLfloat uniformKnots[nknots]   ={0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0};
GLfloat openKnots[nknots]      ={0.0, 0.0, 0.0, 1.0, 2.0, 3.0, 4.0, 4.0, 4.0};
GLfloat notUniformKnots[nknots]={0.0, 1.0, 5.0, 10.0, 30.0, 32.0, 33.0, 43.0, 73.0};

/*GLint ORDEM=4;
GLint nNos=nVertices+ORDEM;
const int nknots = 10;
GLfloat uniformKnots[nknots]   ={0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0};
GLfloat openKnots[nknots]      ={0.0, 0.0, 0.0, 0.0, 1.0, 2.0, 3.0, 3.0, 3.0, 3.0};
GLfloat notUniformKnots[nknots]={0.0, 1.0, 5.0, 10.0, 30.0, 32.0, 33.0, 43.0, 73.0, 103.0};

GLint ORDEM=5;
GLint nNos=nVertices+ORDEM;
const int nknots = 11;
GLfloat uniformKnots[nknots]   ={0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0};
GLfloat openKnots[nknots]      ={0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 2.0, 2.0, 2.0, 2.0, 2.0};
GLfloat notUniformKnots[nknots]={0.0, 1.0, 2.0, 5.0, 10.0, 30.0, 32.0, 33.0, 43.0, 73.0, 103.0};*/

GLint matrizViewport[4];
GLdouble matrizModelview[16], matrizProjecao[16];
GLint yreal;  /*  posicao da coordenada y no OpenGL */
GLdouble wx, wy, wz;  /*  coordenadas no mundo real: x, y, z  */

/*
void gluNurbsCurve( GLUnurbsObj *nobj, GLint nknots, GLfloat *knot, GLint stride, 
                    GLfloat *ctlarray, GLint order, GLenum type )
gluNurbsCurve(nc, nNos, uniformKnots, 3, &vertices[0][0], ORDEM, GL_MAP1_VERTEX_3);

-PARAMETERS
nobj    
  Specifies the NURBS object (created with gluNewNurbsRenderer).

nknots    
  Specifies the number of knots in knot. nknots equals the number 
  of control points plus the order.

knot    
  Specifies an array of nknots nondecreasing knot values.

stride    
  Specifies the offset (as a number of single-precision floating-point 
  values) between successive curve control points.

ctlarray    
  Specifies a pointer to an array of control points. The coordinates 
  must agree with type, specified below.

order   
  Specifies the order of the NURBS curve. order equals degree + 1, 
  hence a cubic curve has an order of 4.

type    
  Specifies the type of the curve. If this curve is defined within 
  a gluBeginCurve/gluEndCurve pair, then the type can be any of the
  valid one-dimensional evaluator types (such as GL_MAP1_VERTEX_3 or 
  GL_MAP1_COLOR_4). Between a gluBeginTrim/gluEndTrim pair, the only 
  valid types are GLU_MAP1_TRIM_2 and GLU_MAP1_TRIM_3. 
*/
void display(void){
  int i;
  glClear(GL_COLOR_BUFFER_BIT);
  switch(spline){
  case BEZIER:
    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, nVertices, &vertices[0][0]);
    glBegin(GL_LINE_STRIP);
    for (i = 0; i <= 30; i++){
      glEvalCoord1f((GLfloat) i/30.0);
    }
    glEnd();
    break;
  case N_UNIFORM:
    gluBeginCurve(nc);
    gluNurbsCurve(nc, nNos, uniformKnots, 3, &vertices[0][0], ORDEM, GL_MAP1_VERTEX_3);
    gluEndCurve(nc);
    break;
  case N_OPEN:
    gluBeginCurve(nc);
    gluNurbsCurve(nc, nNos, openKnots, 3, &vertices[0][0], ORDEM, GL_MAP1_VERTEX_3);
    gluEndCurve(nc);
    break;
  case N_NOT_UNIFORM:
    gluBeginCurve(nc);
    gluNurbsCurve(nc, nNos, notUniformKnots, 3, &vertices[0][0], ORDEM, GL_MAP1_VERTEX_3);
    gluEndCurve(nc);
    break;
  }
  glPointSize(5.0);
  glColor3f(1.0, 1.0, 0.0);
  glBegin(GL_LINE_STRIP);
  for (i = 0; i < nVertices; i++) 
    glVertex3fv(&vertices[i][0]);
  glEnd();
  glColor3f(1.0, 0.0, 0.0);
  glBegin(GL_POINTS);
  for (i = 0; i < nVertices; i++) 
    glVertex3fv(&vertices[i][0]);
  glEnd();
  glColor3f(1.0, 1.0, 1.0);
  glFlush();
  glutSwapBuffers();
}

void init(void){
  glClearColor(0.0, 0.0, 0.0, 0.0);
  spline=BEZIER;
  nc= gluNewNurbsRenderer();
  // gluNurbsProperty(nc, GLU_SAMPLING_TOLERANCE, 5.0);
  glEnable(GL_MAP1_VERTEX_3);
  display();
}

void reshape(int w, int h)
{
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  largura=w;
  altura=h;
  glLoadIdentity();
  glOrtho(esquerda,direita, fundo, topo, perto, longe);
  glMatrixMode(GL_MODELVIEW);
  glGetIntegerv(GL_VIEWPORT, matrizViewport);
  glGetDoublev(GL_MODELVIEW_MATRIX, matrizModelview);
  glGetDoublev(GL_PROJECTION_MATRIX, matrizProjecao);
  glLoadIdentity();
  glutSwapBuffers();
}

/* ARGSUSED1 */
void keyboard(unsigned char key, int x, int y)
{
  switch (key) {
  case 'b':
    spline = BEZIER;
    break;
  case 'u':
    spline = N_UNIFORM;
    break;
  case 'o':
    spline = N_OPEN;
    break;
  case 'n':
    spline = N_NOT_UNIFORM;
    break;
  case 27:
    exit(0);
    break;
  }
  glutPostRedisplay();
}

void proximidade(){
  int i;
  double tam=0, tamin=32000;
  verticeCorrente=0;
  for(i=0; i<nVertices; i++){
    tam = (wx-vertices[i][0])*(wx-vertices[i][0])+
      (wy-vertices[i][1])*(wy-vertices[i][1]);
    if(tam < tamin){
      tamin=tam;
      verticeCorrente=i;
    }
  }
  tamin=sqrt(tamin);
  if(tamin > 0.5){
    mudaCurva=0;
  }
}

void mouse(int button, int state, int x, int y){
  switch (button) {
  case GLUT_LEFT_BUTTON:
    if (state == GLUT_DOWN) {
      yreal = matrizViewport[3] - (GLint) y - 1;
      gluUnProject ((GLdouble) x, (GLdouble) yreal, 0.0, 
        matrizModelview, matrizProjecao, matrizViewport, 
        &wx, &wy, &wz); 
      mudaCurva=1;
      proximidade();
    }
    if (state == GLUT_UP) {
      mudaCurva=0;
    }
    break;
  }
}

void motion(int x, int y){
  if(mudaCurva){
    yreal = matrizViewport[3] - (GLint) y - 1;
    gluUnProject ((GLdouble) x, (GLdouble) yreal, 0.0, 
      matrizModelview, matrizProjecao, matrizViewport, 
      &wx, &wy, &wz); 
    vertices[verticeCorrente][0]=wx;
    vertices[verticeCorrente][1]=wy;
    glutPostRedisplay();
  }
}

int main(int argc, char** argv){
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
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