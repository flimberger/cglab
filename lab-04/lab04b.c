#include <stdlib.h>
#include <math.h> /* define _XOPEN_SOURCE for M_PI */

#include <GL/glut.h>

typedef unsigned char uchar;

double wheelrad = 1.2; /* radius of the wheel */
double wheeldep = 0.4; /* depth of the wheel */
size_t wheelres = 16;  /* wheel circle resolution */

double goepellen = 4.5; /* length of the goepel arm */
double goepelrad = 0.1; /* radius of the goepelarm */

double sockrad = 0.25; /* radius of the socket */
size_t sockres = 8;    /* socket circle resolution */

double rotspeed = (2.0 * M_PI) / 60.0; /* rotation speed */

double camangle = 45.0; /* camera angle */
double camdist = 15;    /* camera distance */

/* angle of cycle rotation for a 1 degree rotation of the goegel arm */
double wheelrot;

/* constant to translate radians to degree and vice versa */
const double RAD2DEG = 180.0 / M_PI;
// static const double DEG2RAD = M_PI / 180.0;

static double time;
static int timer = 10;
static char *wintitle = "Computer Graphics - Exercise 04a";

void renderinit(void);
void handlekey(uchar key, int x, int y);
void handleresize(int w, int h);
void update(int val);
void drawscene(void);

int
main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400);
	glutCreateWindow(wintitle);

	renderinit();

	glutKeyboardFunc(handlekey);
	glutReshapeFunc(handleresize);
	glutDisplayFunc(drawscene);
	glutTimerFunc(timer, update, timer);

	glutMainLoop();

	/* should not reach this point */
	return 0;
}

void
renderinit(void)
{
	time = 0;

	wheelrot = 360.0 / ((180.0 * 2.0 * M_PI * wheelrad) / (M_PI * goepellen));

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glClearColor(0.7, 0.9, 1.0, 1.0);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);

	glShadeModel(GL_SMOOTH);
}

void
handlekey(uchar key, int x, int y)
{
	switch(key) {
	case 27: /* escape */
	case 'q':
		exit(0);
	}
}

void
handleresize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (double) w / (double) h, 1.0, 200.0);
}

void
update(int val)
{
	time += 0.01;
	if (time >= 60.0)
		time = 0.0;
	glutPostRedisplay();
	glutTimerFunc(val, update, val);
}

static void
calcangles(double t, double omega, double *alpha, double *beta)
{
	*alpha = omega * t * RAD2DEG;
	*beta = *alpha * wheelrot;
}

void
drawscene(void)
{
	int i;
	double angle, goepelang, wheelang;

	GLfloat ambientcolor[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat lightcolor[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat lightpos[] = { 0.0, 6.0, 0.0, 1.0 };

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	calcangles(time, rotspeed, &goepelang, &wheelang);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientcolor);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightcolor);
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

	glTranslated(0.0, 0.0, -camdist);
	glRotated(camangle, 1.0, 1.0, 0.0);

	/* ground */
	glPushMatrix();
	glColor3d(0.3, 0.1, 0.0);
	glBegin(GL_QUADS);
	glNormal3d(0.0, 1.0, 0.0);
	glVertex3d(goepellen + 1, -wheelrad, -goepellen - 1);
	glVertex3d(goepellen + 1, -wheelrad, goepellen + 1);
	glVertex3d(-goepellen - 1, -wheelrad, goepellen + 1);
	glVertex3d(-goepellen - 1, -wheelrad, -goepellen - 1);
	glEnd();
	glPopMatrix();

	/* middle socket */
	glPushMatrix();
	glColor3d(0.0, 0.6, 0.0);
	glBegin(GL_TRIANGLE_FAN);
	glNormal3d(0.0, 1.0, 0.0);
	glVertex3d(0.0, goepelrad, 0.0);
	for (i = 0; i <= sockres; i++) {
		angle = 2 * M_PI * (double) i / (double) sockres;
		glVertex3d(sockrad * cos(angle), goepelrad, sockrad * sin(angle));
	}
	glEnd();

	glBegin(GL_QUAD_STRIP);
	for (i = 0; i <= wheelres; i++) {
		angle = 2 * M_PI * ((double) i - 0.5) / (double) sockres;
		glNormal3d(cos(angle), sin(angle), 0.0);
		angle = 2 * M_PI * (double) i / (double) sockres;
		glVertex3d(sockrad * cos(angle), -wheelrad, sockrad * sin(angle));
		glVertex3d(sockrad * cos(angle), goepelrad, sockrad * sin(angle));
	}
	glEnd();
	glPopMatrix();

	/* goepel arm */
	glPushMatrix();
	//glColor3d(0.0, 0.6, 0.0);
	glRotated(-goepelang, 0.0, 1.0, 0.0);
	glBegin(GL_QUADS);
	glNormal3d(0.0, -1.0, 0.0);
	glVertex3d(-goepelrad, -goepelrad, 0.0);
	glVertex3d(goepelrad, -goepelrad, 0.0);
	glVertex3d(goepelrad, -goepelrad, -goepellen);
	glVertex3d(-goepelrad, -goepelrad, -goepellen);

	glNormal3d(1.0, 0.0, 0.0);
	glVertex3d(goepelrad, -goepelrad, 0.0);
	glVertex3d(goepelrad, -goepelrad, -goepellen);
	glVertex3d(goepelrad, goepelrad, -goepellen);
	glVertex3d(goepelrad, goepelrad, 0.0);

	glNormal3d(0.0, 1.0, 0.0);
	glVertex3d(-goepelrad, goepelrad, 0.0);
	glVertex3d(goepelrad, goepelrad, 0.0);
	glVertex3d(goepelrad, goepelrad, -goepellen);
	glVertex3d(-goepelrad, goepelrad, -goepellen);

	glNormal3d(-1.0, 0.0, 0.0);
	glVertex3d(-goepelrad, -goepelrad, 0.0);
	glVertex3d(-goepelrad, -goepelrad, -goepellen);
	glVertex3d(-goepelrad, goepelrad, -goepellen);
	glVertex3d(-goepelrad, goepelrad, 0.0);
	glEnd();
	glPopMatrix();

	/* wheel */
	glPushMatrix();
	glColor3d(0.2, 0.2, 0.2);
	glRotated(-goepelang, 0.0, 1.0, 0.0);
	glTranslated(0.0, 0.0, -goepellen);
	glRotated(-wheelang, 0.0, 0.0, 1.0);

	glBegin(GL_TRIANGLE_FAN);
	glNormal3d(0.0, 0.0, 1.0);
	glVertex3d(0.0, 0.0, 0.0);
	for (i = 0; i <= wheelres; i++) {
		angle = 2 * M_PI * (double) i / (double) wheelres;
		glVertex3d(wheelrad * cos(angle), wheelrad * sin(angle), 0);
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glNormal3d(0.0, 0.0, -1.0);
	glVertex3d(0.0, 0.0, -wheeldep);
	for (i = 0; i <= wheelres; i++) {
		angle = 2 * M_PI * (double) i / (double) wheelres;
		glVertex3d(wheelrad * cos(angle), wheelrad * sin(angle), -wheeldep);
	}
	glEnd();

	glBegin(GL_QUAD_STRIP);
	for (i = 0; i <= wheelres; i++) {
		if (i % 2)
			glColor3d(0.0, 0.0, 0.0);
		else
			glColor3d(1.0, 1.0, 0.0);
		angle = 2 * M_PI * ((double) i - 0.5) / (double) wheelres;
		glNormal3d(cos(angle), sin(angle), 0.0);
		angle = 2 * M_PI * (double) i / (double) wheelres;
		glVertex3d(wheelrad * cos(angle), wheelrad * sin(angle), 0.0);
		glVertex3d(wheelrad * cos(angle), wheelrad * sin(angle), -wheeldep);
	}
	glEnd();
	glPopMatrix();

	glutSwapBuffers();
}
