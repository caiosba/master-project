#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <GL/glut.h>
 
void listenToUDP();

bool fullscreen = false;
bool mouseDown = false;
float xrot = 0.0f;
float yrot = 0.0f;
float xdiff = 0.0f;
float ydiff = 0.0f;
long yaw, pitch, roll;
 
void drawBox() {
  glBegin(GL_QUADS);
  glColor3f(1.0f, 0.0f, 0.0f);
  // FRONT
  glVertex3f(-0.5f, -0.5f, 0.5f);
  glVertex3f( 0.5f, -0.5f, 0.5f);
  glVertex3f( 0.5f, 0.5f, 0.5f);
  glVertex3f(-0.5f, 0.5f, 0.5f);
  // BACK
  glVertex3f(-0.5f, -0.5f, -0.5f);
  glVertex3f(-0.5f, 0.5f, -0.5f);
  glVertex3f( 0.5f, 0.5f, -0.5f);
  glVertex3f( 0.5f, -0.5f, -0.5f);
  glColor3f(0.0f, 1.0f, 0.0f);
  // LEFT
  glVertex3f(-0.5f, -0.5f, 0.5f);
  glVertex3f(-0.5f, 0.5f, 0.5f);
  glVertex3f(-0.5f, 0.5f, -0.5f);
  glVertex3f(-0.5f, -0.5f, -0.5f);
  // RIGHT
  glVertex3f( 0.5f, -0.5f, -0.5f);
  glVertex3f( 0.5f, 0.5f, -0.5f);
  glVertex3f( 0.5f, 0.5f, 0.5f);
  glVertex3f( 0.5f, -0.5f, 0.5f);
  glColor3f(0.0f, 0.0f, 1.0f);
  // TOP
  glVertex3f(-0.5f, 0.5f, 0.5f);
  glVertex3f( 0.5f, 0.5f, 0.5f);
  glVertex3f( 0.5f, 0.5f, -0.5f);
  glVertex3f(-0.5f, 0.5f, -0.5f);
  // BOTTOM
  glVertex3f(-0.5f, -0.5f, 0.5f);
  glVertex3f(-0.5f, -0.5f, -0.5f);
  glVertex3f( 0.5f, -0.5f, -0.5f);
  glVertex3f( 0.5f, -0.5f, 0.5f);
  glEnd();
}
 
bool init() {
  glClearColor(0.93f, 0.93f, 0.93f, 0.0f);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glClearDepth(1.0f);
  return true;
}
 
void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  gluLookAt(
  0.0f, 0.0f, 3.0f,
  0.0f, 0.0f, 0.0f,
  0.0f, 1.0f, 0.0f);
  glRotatef(xrot, 1.0f, 0.0f, 0.0f);
  glRotatef(yrot, 0.0f, 1.0f, 0.0f);
  drawBox();
  glFlush();
  glutSwapBuffers();
}
 
void resize(int w, int h) {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0, w, h);
  gluPerspective(45.0f, 1.0f * w / h, 1.0f, 100.0f);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}
 
void initiateCube() {
  int argc = 1;
  char *argv[1] = {(char*)"Something"};
  glutInit(&argc, argv);
  glutInitWindowPosition(50, 50);
  glutInitWindowSize(500, 500);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  glutCreateWindow("Control a Solid using the Glasses");
  glutReshapeWindow(500, 500);
  glutPositionWindow(50, 50);
  glutDisplayFunc(display);
  glutReshapeFunc(resize);
  glutTimerFunc(200, &listenToUDP, 0);
  glutMainLoop();
}

void controlCube(long y, long p, long r) {
  printf("Yaw: %d Pitch: %d Roll: %d\n", y, p, r);
  yaw = y;
  pitch = p;
  roll = r;
  xrot = pitch;
  yrot = yaw;
  glutPostRedisplay();
  glutTimerFunc(200, &listenToUDP, 0);
}
