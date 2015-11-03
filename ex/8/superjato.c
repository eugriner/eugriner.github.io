#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include "png_texture.h"

#define PI 3.1415

#define COORD_TEXTURA_PLANO 1.0
#define COORD_TEXTURA_AVIAO 1.0
#define COR_DO_PLANO 0.52,0.52,0.78,1.0
#define COR_DO_AVIAO 0.3,0.52,0.18,1.0
#define COR_DA_LOGO 0.7,0.52,0.58,1.0
#define COR_DA_TURBINA 0.24,0.24,0.24,1.0 
#define TEXTURA_DO_PLANO "montanhas.png"
#define TEXTURA_DO_AVIAO "camuflagem.png"
#define LOGO_DO_AVIAO "logo.png"
#define TEXTURA_DA_CHAMA "flame2.png"


GLint WIDTH =800;
GLint HEIGHT=600;

GLfloat obs[3]={0.0,7.0,0.0};
GLfloat look[3]={0.0,3.0,0.0};
GLuint  textura_plano;
GLuint  textura_aviao;
GLuint  logo_aviao;
GLuint  textura_chama;

GLshort texturas=1;
GLshort flame_is_on=1;
GLfloat tetaxz=0;
GLfloat raioxz=6;
GLuint  jato;

GLfloat ctp[4][2]={
  {-COORD_TEXTURA_PLANO,-COORD_TEXTURA_PLANO},
  {+COORD_TEXTURA_PLANO,-COORD_TEXTURA_PLANO},
  {+COORD_TEXTURA_PLANO,+COORD_TEXTURA_PLANO},
  {-COORD_TEXTURA_PLANO,+COORD_TEXTURA_PLANO}
};

GLfloat cta[4][2]={
  {-COORD_TEXTURA_AVIAO,-COORD_TEXTURA_AVIAO},
  {+COORD_TEXTURA_AVIAO,-COORD_TEXTURA_AVIAO},
  {+COORD_TEXTURA_AVIAO,+COORD_TEXTURA_AVIAO},
  {-COORD_TEXTURA_AVIAO,+COORD_TEXTURA_AVIAO}
};

GLfloat ctl[4][2]={
  {0,0},
  {1,0},
  {1,1},
  {0,1}
};

GLfloat asa[][3]={
  {-4.0,0.0,0.0},
  {+4.0,0.0,0.0},
  {0.0,0.0,3.0}
};

GLfloat cauda[][3]={
  {0.0,0.0,0.0},
  {0.0,2.0,-1.0},
  {0.0,2.0,0.0},
  {0.0,0.0,2.0}
};

GLfloat logo[][3]={
  {0.0,1.0,-0.2},
  {0.0,1.5,-0.2},
  {0.0,1.5,0.3},
  {0.0,1.0,0.3}
};

void reshape(int width, int height){
  WIDTH=width;
  HEIGHT=height;
  glViewport(0,0,(GLint)width,(GLint)height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(70.0,width/(float)height,0.1,30.0);
  glMatrixMode(GL_MODELVIEW);
}

void compoe_jato(void){
  GLUquadricObj *quadric;

  /* inicia a composicao do jato */
  jato = glGenLists(1);
  glNewList(jato, GL_COMPILE);

  /* asas */
  glBegin(GL_TRIANGLES); 
  glTexCoord2fv(cta[0]); glVertex3fv(asa[0]);
  glTexCoord2fv(cta[1]); glVertex3fv(asa[1]);
  glTexCoord2fv(cta[3]); glVertex3fv(asa[2]);
  glEnd();

  /*missil1*/
  glPushMatrix();
  //posiciona
  glTranslatef(-2, -0.11, 1);
  quadric = gluNewQuadric();
  gluQuadricTexture(quadric, GL_TRUE);
  //bico
  gluCylinder(quadric, 0.1, 0.0, 0.4, 8, 1);
  //corpo
  glTranslatef(0, 0, -0.8);
  gluCylinder(quadric, 0.1, 0.1, 0.8, 8, 1);
  //cauda
  glTranslatef(0, 0, -0.05);
  gluCylinder(quadric, 0.0, 0.1, 0.05, 8, 1);
  glPopMatrix();

  /*missil2*/
  glPushMatrix();
  //posiciona
  glTranslatef(2, -0.11, 1);
  quadric = gluNewQuadric();
  gluQuadricTexture(quadric, GL_TRUE);
  //bico
  gluCylinder(quadric, 0.1, 0.0, 0.4, 8, 1);
  //corpo
  glTranslatef(0, 0, -0.8);
  gluCylinder(quadric, 0.1, 0.1, 0.8, 8, 1);
  //cauda
  glTranslatef(0, 0, -0.05);
  gluCylinder(quadric, 0.0, 0.1, 0.05, 8, 1);
  glPopMatrix();
  
  /* corpo */
  quadric = gluNewQuadric();
  gluQuadricTexture(quadric, GL_TRUE);
  gluCylinder(quadric, 0.5, 0.5, 4, 12, 3);

  //guarda para turbina
  glPushMatrix();
  
  /* nariz */
  quadric = gluNewQuadric();
  gluQuadricTexture(quadric, GL_TRUE);
  glPushMatrix();
  glTranslatef(0,0,4);
  gluCylinder(quadric, 0.5, 0.0, 1.5, 12, 3);
  glPopMatrix();

  /* cauda */
  glBegin(GL_POLYGON); 
  glTexCoord2fv(cta[0]); glVertex3fv(cauda[0]);
  glTexCoord2fv(cta[1]); glVertex3fv(cauda[1]);
  glTexCoord2fv(cta[2]); glVertex3fv(cauda[2]);
  glTexCoord2fv(cta[3]); glVertex3fv(cauda[3]);
  glEnd();

  /* turbina */
  //retorna mat para turbina  
  glPopMatrix();
  glPushMatrix();
  glTranslatef(0,0,-1);
  
  quadric = gluNewQuadric();
  if (flame_is_on) {
    printf("%s\n", "isON");
    gluQuadricTexture(quadric, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D,textura_chama);
    glColor4f(1.0,0.0,0.0,0.5);
    gluCylinder(quadric, 0.1, 0.497, 1, 12, 1);  
  }

  glTranslatef(0,0,0.4);
  glDisable(GL_TEXTURE_2D);
  if (flame_is_on) {
    glColor4f(1.0,0.0,0.0,1.0);
  } else {
    glColor4f(0.34,0.34,0.34,1.0);
  }
  gluCylinder(quadric, 0, 0.495, 0.6, 12, 1);
  glTranslatef(0,0,0.35);
  
  glColor4f(COR_DA_TURBINA);
  gluCylinder(quadric, 0.4, 0.5, 0.25, 12, 1);
  glPopMatrix();
  /* end - turbina*/

  if(texturas){
    glEnable(GL_TEXTURE_2D);  
  }

  /* logo na cauda*/
  glBindTexture(GL_TEXTURE_2D,logo_aviao);
  glColor4f(COR_DA_LOGO);
  
  glPushMatrix();
  glTranslatef(0.01,0,0);
  glBegin(GL_QUADS);
  glTexCoord2fv(ctl[0]); glVertex3fv(logo[3]);
  glTexCoord2fv(ctl[1]); glVertex3fv(logo[0]);
  glTexCoord2fv(ctl[2]); glVertex3fv(logo[1]);
  glTexCoord2fv(ctl[3]); glVertex3fv(logo[2]);
  glEnd();
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-0.01,0,0);
  glBegin(GL_QUADS);
  glTexCoord2fv(ctl[0]); glVertex3fv(logo[3]);
  glTexCoord2fv(ctl[1]); glVertex3fv(logo[0]);
  glTexCoord2fv(ctl[2]); glVertex3fv(logo[1]);
  glTexCoord2fv(ctl[3]); glVertex3fv(logo[2]);
  glEnd();
  glPopMatrix();
  /*end - logo na cauda*/


  /* cabine do piloto */
  glDisable(GL_TEXTURE_2D);
  glTranslatef(0,0.3,3.5);
  glPushMatrix();
  glScalef(0.7,0.7,2.0);
  quadric=gluNewQuadric();
  glColor4f(0.3,0.5,1,0.5);
  gluSphere(quadric,0.5,12,12);
  glPopMatrix();


  /* termina a composicao do jato*/
  glEndList();
}

void display(void){
  glEnable(GL_DEPTH_TEST);

  //glDepthMask(GL_TRUE);
  glClearColor(1.0,1.0,1.0,1.0);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  glPushMatrix();

  /* calcula a posicao do observador */
  obs[0]=raioxz*cos(2*PI*tetaxz/360);
  obs[2]=raioxz*sin(2*PI*tetaxz/360);
  gluLookAt(obs[0],obs[1],obs[2],look[0],look[1],look[2],0.0,1.0,0.0);

  /* habilita/desabilita uso de texturas*/
  if(texturas){
    glEnable(GL_TEXTURE_2D);  
  }
  else{
    glDisable(GL_TEXTURE_2D);
  }
  glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);

  // grava a transformacao atual
  glPushMatrix();
  //plano texturizado
  glColor4f(COR_DO_PLANO);
  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
  glBindTexture(GL_TEXTURE_2D,textura_plano);
  glBegin(GL_QUADS);
  glTexCoord2fv(ctp[0]);  glVertex3f(-10,0,10);
  glTexCoord2fv(ctp[1]);  glVertex3f(10,0,10);
  glTexCoord2fv(ctp[2]);  glVertex3f(10,0,-10);
  glTexCoord2fv(ctp[3]);  glVertex3f(-10,0,-10);
  glEnd();

  //aviao texturizado
  glTranslatef(0.0,2.0,-3.0);
  glColor4f(COR_DO_AVIAO);
  glBindTexture(GL_TEXTURE_2D,textura_aviao);
  glCallList(jato);
  glPopMatrix();
  // volta para a ultima transformacao

  // grava a transformacao atual
  glPushMatrix();
  glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
  glColor4f(COR_DO_PLANO);
  glBindTexture(GL_TEXTURE_2D,textura_plano);
   
  glBegin(GL_QUADS);
  glTexCoord2fv(ctp[0]);  glVertex3f(-10,0,10);
  glTexCoord2fv(ctp[1]);  glVertex3f(10,0,10);
  glTexCoord2fv(ctp[2]);  glVertex3f(10,0,-10);
  glTexCoord2fv(ctp[3]);  glVertex3f(-10,0,-10);
  glEnd();
  glTranslatef(0.0,2.0,-3.0);
  glColor4f(COR_DO_AVIAO);
  glBindTexture(GL_TEXTURE_2D,textura_aviao);
  glCallList(jato);
  glPopMatrix();
  // volta para a ultima transformacao
  
  glPopMatrix();
  glutSwapBuffers();
}


void special(int key, int x, int y){
  switch (key) {
  case GLUT_KEY_UP:
    obs[1]=obs[1]+1;
    glutPostRedisplay();
    break;
  case GLUT_KEY_DOWN:
    obs[1] =obs[1]-1;
    glutPostRedisplay();
    break;
  case GLUT_KEY_LEFT:
    tetaxz=tetaxz+2;
    glutPostRedisplay();
    break;
  case GLUT_KEY_RIGHT:
    tetaxz=tetaxz-2;
    glutPostRedisplay();
    break;
  }
}

void keyboard(unsigned char key, int x, int y){
  switch (key) {
  case 27:
    exit(0);
    break;
  case 't':
    texturas = !texturas;
    glutPostRedisplay();
    break;
  case 'r':
    raioxz=raioxz+1;
    glutPostRedisplay();
    break;
  case 'R':
    raioxz=raioxz-1;
    if(raioxz==0){
      raioxz=1;
    }
    glutPostRedisplay();
    break;
  case 'o':
    flame_is_on = !flame_is_on;
    compoe_jato();
    //printf("%i\n", flame_is_on);
    glutPostRedisplay();
    break;
  }
}

void carregar_texturas(void){
  textura_plano = png_texture_load(TEXTURA_DO_PLANO, NULL, NULL);
  textura_aviao = png_texture_load(TEXTURA_DO_AVIAO, NULL, NULL);
  logo_aviao = png_texture_load(LOGO_DO_AVIAO, NULL, NULL);
  textura_chama = png_texture_load(TEXTURA_DA_CHAMA, NULL, NULL);
}

void init(){
  glEnable(GL_DEPTH_TEST);
  
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  carregar_texturas();
  compoe_jato();
  //  glShadeModel(GL_FLAT);
  glEnable(GL_TEXTURE_2D);
}

int main(int argc,char **argv){
  glutInitWindowPosition(0,0);
  glutInitWindowSize(WIDTH,HEIGHT);
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE|GLUT_ALPHA);

  if(!glutCreateWindow("Avião a jato")) {
    fprintf(stderr,"Error opening a window.\n");
    exit(-1);
  }

  init();
  
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(special);
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutMainLoop();

  return(0);
}
