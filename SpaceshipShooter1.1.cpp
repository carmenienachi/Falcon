/*
Carmen-Michelle Ienachi
Coursework 1
3D Graphics Programming
12.11.2019
*/
#include "include\freeglut.h"
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <math.h>


#define SPACEBAR 32
#define MAX_BULLET_ON_SCREEN 8
#define MAX_ASTEROIDS 10
#define MAX_VELO_BULLET 10
#define PI 3.14159265
#define X_TEXT_POS 4
#define Y_TEXT_POS 480

using namespace std;

void drawSpaceship(GLfloat length, GLfloat xCentre, GLfloat yCentre);
void drawAsteroidShape(GLfloat xCentre, GLfloat yCentre, GLfloat radius, int type);

int Score = 0;
int menuEntry;
GLsizei wh = 500, ww = 500;
GLfloat red = 1.0, green = 1.0, blue = 1.0;

//mouse interaction
vector<GLfloat> points;


struct spaceship
{
	GLfloat angle = 0;
	GLfloat x = 250, y = 250;
	GLfloat size = 25;
	int shoot = 0;
}Spaceship;

struct bullet
{
	bool active;
	GLfloat x, y;
	GLfloat speed, dx, dy;
	GLfloat angle;
};
static bullet bullets[MAX_BULLET_ON_SCREEN];

struct asteroid
{
	GLfloat xCentre, yCentre, radius, speed, angle;
	GLint type, direction = 1;
	bool active;
}Asteroids[MAX_ASTEROIDS];


void reshape(GLsizei w, GLsizei h)
{
	// Prevent a divide by zero
	if (h == 0)
		h = 1;

	// Set Viewport to window dimensions
	glViewport(0, 0, w, h);

	// Set current stuck to projection
	glMatrixMode(GL_PROJECTION);

	// Reset coordinate system - or set the current matrix to the Identity matrix
	glLoadIdentity();

	// Establish clipping volume (left, right, bottom, top, near, far)
	// Set the aspect ratio of the clipping area to match the viewport
	if (w <= h)
		glOrtho(0.0f, ww, 0.0f, wh * h / w, 1.0, -1.0);
	else
		glOrtho(0.0f, ww * w / h, 0.0f, wh, 1.0, -1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//Collision between two elements on the screen 
int Collision(GLfloat x1, GLfloat y1, GLfloat radius1, GLfloat x2, GLfloat y2, GLfloat radius2)
{
	GLdouble distance = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
	GLfloat distance2 = radius1 + radius2;
	if (distance > distance2)
		return 0;
	//return 0;
	else return 1;
}

//asteroids related functions
void randomisePolygons()
{
	//for each asteroid randomise attributes
	for (int i = 0; i < MAX_ASTEROIDS; i++)
	{
		//get radius
		Asteroids[i].radius = rand() % 11 + 10;
		//get coordinates
		int okSpaceship = 1;//1 to be able to get in the while loop
		int okAsteroids = 1;
		int okBoundaries = 1;
		while (okSpaceship || okAsteroids)
		{
			okSpaceship = 0;//meaning the asteroid is not colliding with spaceship
			okAsteroids = 0;//asteroid is not colliding with other asteroids
			okBoundaries = 0; //not colliding with the boundaries
			//randomise coordinates
			Asteroids[i].xCentre = rand() % 501;
			Asteroids[i].yCentre = rand() % 501;
			//check if is not on the spaceship
			if (Collision(Asteroids[i].xCentre, Asteroids[i].yCentre, Asteroids[i].radius, Spaceship.x, Spaceship.y, Spaceship.size))
				okSpaceship = 1;// is coliding with the spaceship
			for (int j = 0; j < i; j++)
			{
				if (Collision(Asteroids[i].xCentre, Asteroids[i].yCentre, Asteroids[i].radius, Asteroids[j].xCentre, Asteroids[j].yCentre, Asteroids[j].radius))
					okAsteroids = 1;//is colliding with at least one asteroid
			}
			if (Asteroids[i].xCentre > ww - Asteroids[i].radius || Asteroids[i].xCentre <Asteroids[i].radius || Asteroids[i].yCentre >wh - Asteroids[i].radius || Asteroids[i].yCentre < Asteroids[i].radius)
				okAsteroids = 1;//coliding with the boundries
		}
		//set it active
		Asteroids[i].active = 1;
		//get the type: hexagon, pentagon, etc.
		Asteroids[i].type = rand() % 4 + 5;
		//set the direction clockwise or anti-clockwise
		int direction = rand() % 2;
		if (direction == 0)
			Asteroids[i].direction = -1;
		else
			Asteroids[i].direction = 1;
		//get a speed for the rotation 
		Asteroids[i].speed = rand() % 5 + 1;
	}

}
void drawAsteroids()
{
	for (int i = 0; i < MAX_ASTEROIDS; i++)
	{
		if (Asteroids[i].active) {
			glPushMatrix();
			//rotate Asteroid
			glTranslatef(Asteroids[i].xCentre, Asteroids[i].yCentre, 0.0);
			glRotatef(Asteroids[i].angle, 0.0, 0.0, 1.0);
			glTranslatef(-Asteroids[i].xCentre, -Asteroids[i].yCentre, 0.0);
			//draw asteroid
			drawAsteroidShape(Asteroids[i].xCentre, Asteroids[i].yCentre, Asteroids[i].radius, Asteroids[i].type);
			glPopMatrix();
		}

	}
}
void drawAsteroidShape(GLfloat xCentre, GLfloat yCentre, GLfloat radius, int type)
{
	//get the pattern angle of the polygon ex. 360/6= 60degrees menaing is a hexagon
	GLfloat PatternAngle = 360 / type;
	GLfloat StartAngle = 0;
	//the direction of each point of the polygon depending on the pattern angle and start angle 
	GLfloat dx = sin(StartAngle * PI / 180);
	GLfloat dy = cos(StartAngle * PI / 180);
	glBegin(GL_POLYGON);
	glColor3f(red, green, blue);
	for (int i = 0; i < type; i++)
	{
		//get direction
		dx = sin(StartAngle * PI / 180);
		dy = cos(StartAngle * PI / 180);
		//get coordinates
		glVertex2f(xCentre + dx * radius, yCentre + dy * radius);
		//increase the angle to be able to draw the other point which is at 60 distance from the first one(hor hexagon)
		StartAngle += PatternAngle;
	}
	glEnd();
	glFlush();

}
void updateAsteroidsAngle()
{
	//for each asteroid
	for (int i = 0; i < MAX_ASTEROIDS; i++)
	{
		//depending on direction change angle
		Asteroids[i].angle = Asteroids[i].angle + Asteroids[i].direction * Asteroids[i].speed;
		if (Asteroids[i].angle < 0)
			Asteroids[i].angle = 360;
		else if (Asteroids[i].angle > 360)
			Asteroids[i].angle = 0;
	}
}
bool checkActiveAsteroids()
{
	for (int i = 0; i < MAX_ASTEROIDS; i++)
	{
		if (Asteroids[i].active)
		{
			return 0;
		}
	}
	return 1;
}


//player(spaceship) related functions
void drawSpaceship(GLfloat radius, GLfloat xCentre, GLfloat yCentre)
{
	glColor3f(red, green, blue);
	glBegin(GL_POLYGON);
	glVertex2f(xCentre + radius, yCentre);
	glVertex2f(xCentre - radius / 2, yCentre - (0.86 * radius));
	glVertex2f(xCentre, yCentre);
	glVertex2f(xCentre - radius / 2, yCentre + (0.86 * radius));
	glEnd();
}
void rotateSpaceship() {

	glLineWidth(1.5);
	glEnable(GL_LINE_SMOOTH);
	glColor3f(red, green, blue);
	glPushMatrix();
	glTranslatef(Spaceship.x, Spaceship.y, 0.0);
	glRotatef(Spaceship.angle, 0.0, 0.0, 1.0);
	glTranslatef(-Spaceship.x, -Spaceship.y, 0.0);
	drawSpaceship(Spaceship.size, Spaceship.x, Spaceship.y);
	glPopMatrix();
}
void MoveSpaceshipRight()
{
	Spaceship.angle = Spaceship.angle - 3.0;
	if (Spaceship.angle < 0)Spaceship.angle = 360;


}
void MoveSpaceshipLeft()
{
	Spaceship.angle = Spaceship.angle + 3.0;
	if (Spaceship.angle > 360) Spaceship.angle = 0;

}


//bullet related functions
void drawBullet(bullet* b)
{

	glLineWidth(0.5);
	glColor3f(red, green, blue);
	glPushMatrix();
	glTranslatef(b->x, b->y, 0.0);
	glRotatef(b->angle, 0.0, 0.0, 1.0);
	glTranslatef(-b->x, -b->y, 0.0);
	glBegin(GL_POLYGON);
	glVertex2f(b->x + 1, b->y - 1);
	glVertex2f(b->x + 5, b->y - 1);
	glVertex2f(b->x + 5, b->y + 1);
	glVertex2f(b->x + 1, b->y + 1);
	glEnd();
	glPopMatrix();
}
void spawnBullets()
{
	//get the position +direction+speed for bullets 
	for (int i = 0; i < MAX_BULLET_ON_SCREEN; i++) {

		if (bullets[i].active == 0) {
			bullets[i].active = 1;
			//starting point
			bullets[i].y = Spaceship.x + Spaceship.size * sin(Spaceship.angle * PI / 180);
			bullets[i].x = Spaceship.y + Spaceship.size * cos(Spaceship.angle * PI / 180);
			//get the angle of direction
			bullets[i].angle = Spaceship.angle;
			//get direction+velocity
			bullets[i].dy = MAX_VELO_BULLET * sin(bullets[i].angle * PI / 180);
			bullets[i].dx = MAX_VELO_BULLET * cos(bullets[i].angle * PI / 180);
			break;
		}
	}
}
void moveBullet()
{
	if (Spaceship.shoot == 1) {

		spawnBullets();
		//disable shooting 
		Spaceship.shoot = 0;
	}

	//moving the bullets
	for (int i = 0; i < MAX_BULLET_ON_SCREEN; i++) {

		if (bullets[i].active == 1) {
			bullets[i].x = bullets[i].x + bullets[i].dx;
			bullets[i].y = bullets[i].y + bullets[i].dy;
		}
		//Destory bullet outside boundries
		if (bullets[i].active == 1 && (bullets[i].x > ww || bullets[i].x < 0 || bullets[i].y >wh || bullets[i].y < 0)) {
			bullets[i].active = 0;
		}
	}

	//check collision with asteroids
	for (int j = 0; j < MAX_BULLET_ON_SCREEN; j++)
		for (int i = 0; i < MAX_ASTEROIDS; i++)
		{
			if (Asteroids[i].active)
			{
				if (Collision(Asteroids[i].xCentre, Asteroids[i].yCentre, Asteroids[i].radius, bullets[j].x + 3 * bullets[j].dx, bullets[j].y + 3 * bullets[j].dy, 3))
				{
					Asteroids[i].active = 0;
					bullets[j].active = 0;
					Score += 10;
				}
			}

		}
}	


void displayScore()
{
	glColor3f(1, 1, 1);
	glRasterPos2f(X_TEXT_POS, Y_TEXT_POS);
	std::string str = "Points: " + std::to_string(Score);
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, reinterpret_cast<const unsigned char*>(str.c_str()));
}


//pop-up menu interraction
void drawline(float x1, float y1, float x2, float y2)
{
	glBegin(GL_LINES);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glEnd();
}
void drawGrid(int mode)
{
	switch (mode)
	{
	case 1: {

		glColor3f(1.0, 0.0, 0.0);
		glPushAttrib(GL_ENABLE_BIT);
		glLineStipple(1, 0x00ff);
		//glLineStipple(1, 0xAAAA);  
		glEnable(GL_LINE_STIPPLE);

		for (int i = 0; i < wh; i += 10)
		{
			glLineWidth(1.0);
			drawline(0, i, (float)ww, i);
		}
		for (int i = 0; i < ww; i += 10)
		{
			glLineWidth(1.0);
			drawline(i, 0, i, (float)wh);
		}
		glPopAttrib();
		glFlush();
		break;
		//glEnd();
	}
	case 2: {
		//pattern background
		break;
	}
	case 3: break;
	};
}
void createMenu(GLint id)
{
	if (id == 1)
	{
		menuEntry = 1;
	}
	else if (id == 2)
	{
		menuEntry = 3;
	}
	else if (id == 3)
		menuEntry = 3;

	glutPostRedisplay();
}


//hardware intreraction
void MyKeyboardFunc(unsigned char Key, int x, int y)
{
	switch (Key)
	{
	case 'd': MoveSpaceshipRight(); break;
	case 'a': MoveSpaceshipLeft(); break;
	case ' ': Spaceship.shoot = 1; break;
	};
}
void Special(int Key, int x, int y)
{
	switch (Key)
	{
	case GLUT_KEY_RIGHT: MoveSpaceshipRight(); break;
	case GLUT_KEY_LEFT: MoveSpaceshipLeft(); break;
	};
}
void MouseInterraction(GLint button, GLint state, GLint x, GLint y)
{

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		points.push_back(x);
		points.push_back(wh - y);
	}
	glutPostRedisplay();
}


void displayInstructions()
{
	cout << "SPACESHIP GAME" << endl;
	cout << "How to play the game:controlls and interraction." << endl << endl;
	cout << "1) Press the arrow keys to rotate the Shaceship." << endl;
	cout << "2) Press Spacebar to shoot bullets." << endl;
	cout << "3)Right click for menu options." << endl;
}


void init(void)
{
	glutInitDisplayMode(GLUT_DOUBLE);
	//glClearColor (0.0, 0.0, 0.0, 0.0);   /* window will be cleared to black */
	//glClearColor(1.0, 0.0, 0.0, 0.0);     /* window will be cleared to red */
	randomisePolygons();
	displayInstructions();

}
void TimerFunction(int value)
{
	updateAsteroidsAngle();
	moveBullet();
	glutPostRedisplay();
	glutTimerFunc(33, TimerFunction, value);
}
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(red, green, blue);
	glLoadIdentity();
	
	drawGrid(menuEntry);
	rotateSpaceship();

	glPopMatrix();
	for (int i = 0; i < MAX_BULLET_ON_SCREEN; i++) {
		if (bullets[i].active) {
			drawBullet(&bullets[i]);
		}
	}
	if (checkActiveAsteroids())
	{
		randomisePolygons();
	}

	
	drawAsteroids();
	displayScore();
	glutSwapBuffers();

}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Spaceship Shooter");

	//keyboard functions
	glutKeyboardFunc(MyKeyboardFunc);
	glutSpecialFunc(Special);

	glutCreateMenu(createMenu);
	glutAddMenuEntry("grid on", 1);
	glutAddMenuEntry("pattern on", 2);
	glutAddMenuEntry("off", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	glutTimerFunc(60, TimerFunction, 0);

	glutMainLoop();
	return 0;
}

