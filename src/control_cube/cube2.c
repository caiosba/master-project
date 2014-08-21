#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <Eigen/Core>
#include <Eigen/SVD>
#include <Eigen/Cholesky>
#include <Eigen/Geometry>
#include <Eigen/LU>

void listenToUDP();

bool fullscreen = false;
bool mouseDown = false;
float xrot = 0.0f;
float yrot = 0.0f;
float xdiff = 0.0f;
float ydiff = 0.0f;
Eigen::Matrix3f rotation;
Eigen::Vector3f translation;

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

	float x = atan2(rotation(2,1), rotation(2,2)) * 180.0 / M_PI;
	float y = atan2(-rotation(2,0), sqrt(powf(rotation(2,1),2) + powf(rotation(2,2),2))) * 180.0 / M_PI;
	float z = atan2(rotation(1,0),rotation(0,0)) * 180.0 / M_PI;

	printf("X: %f Y: %f Z: %f\n", x, y, z);

  glRotatef(-z, 0, 0, 1);
  glRotatef(-y, 0, 1, 0);
  glRotatef(-x, 1, 0, 0);
	// glTranslatef(translation(0)/100, translation(1)/100, translation(2)/100);
	
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
  glutCreateWindow("Control a Solid using KinectFusion");
  glutReshapeWindow(500, 500);
  glutPositionWindow(50, 50);
  glutDisplayFunc(display);
  glutReshapeFunc(resize);
  glutTimerFunc(200, &listenToUDP, 0);
  glutMainLoop();
}

typedef struct {
  float x;
  float y;
  float z;
} Vector;

void anglesToAxes(const Vector angles, Vector * left, Vector * up, Vector * forward) {
  const float DEG2RAD = 3.141593f / 180;
  float sx, sy, sz, cx, cy, cz, theta;

  // rotation angle about X-axis (pitch)
  theta = angles.x * DEG2RAD;
  sx = sinf(theta);
  cx = cosf(theta);

  // rotation angle about Y-axis (yaw)
  theta = angles.y * DEG2RAD;
  sy = sinf(theta);
  cy = cosf(theta);

  // rotation angle about Z-axis (roll)
  theta = angles.z * DEG2RAD;
  sz = sinf(theta);
  cz = cosf(theta);

  // determine left axis
  left->x = cy*cz;
  left->y = sx*sy*cz + cx*sz;
  left->z = -cx*sy*cz + sx*sz;

  // determine up axis
  up->x = -cy*sz;
  up->y = -sx*sy*sz + cx*cz;
  up->z = cx*sy*sz + sx*cz;

  // determine forward axis
  forward->x = sy;
  forward->y = -sx*cy;
  forward->z = cx*cy;
}

void controlCube(Eigen::Matrix3f r, Eigen::Vector3f t) {
  rotation = r;
  translation = t;
  glutPostRedisplay();
  glutTimerFunc(100, &listenToUDP, 0);
}
