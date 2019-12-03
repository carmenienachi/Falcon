/*
* 5CCGD003W - Tutorial 4-PART1 - example solution code
* This tutorial introduces different even-driven programming examples that can be used for the cw
*/
// Code by Dr Anastassia Angelopoulou 


#include "include\freeglut.h"	// OpenGL toolkit - in the local shared folder
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>
#include <stdio.h>
#include <math.h>


//set up some constants
#define X_CENTRE 200.0      /* centre point of square */
#define Y_CENTRE 200.0
#define LENGTH   20.0      /* lengths of sides of square */

// Use the STL extension of C++
using namespace std;


//Initialise constants
GLdouble PI = 3.14159;	    //Mathematical constant used for cos and sin functions
GLfloat angle = 2.0 * PI;
GLfloat angle_z = 0.0;
GLfloat angle_x = 0.0;

GLsizei wh = 500, ww = 500;

GLfloat red = 1.0, green = 1.0, blue = 1.0;

//use to set/track star coordinate for moving/translated star
GLfloat starX = 0.0;
GLfloat starY = 250.0;

bool star = true;

void moveStar(GLfloat size);
void drawStar(GLfloat radius, GLfloat x, GLfloat y);

void reshape(GLsizei w, GLsizei h)
{
	// Prevent a divide by zero
	if (h == 0)
		h = 1;

	glMatrixMode(GL_PROJECTION);
	// Set Viewport to window dimensions
	glViewport(0, 0, w, h);
	// Reset coordinate system
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


/* display callback function
   called whenever contents of window need to be re-displayed */
   //this is the all important drawing method - all drawing code goes in here
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(red, green, blue);        /* white drawing objects */
	glLoadIdentity();

	glPushMatrix();
	// Draws a 5 point star and rotates by 30 degrees
	for (GLfloat i = 0; i < 13; i++)
	{

		// modeling transformation
		//Since we want to roate the star from its own axis we need first to translate to the orginin,
		//rotate it and then translate back
		glTranslatef(250.0, 250.0, 0.0);
		glRotatef(30.0, 0.0, 0.0, 1.0);
		glTranslatef(-250.0, -250.0, 0.0);
		drawStar(100.0, 250.0, 250.0);

	}

	glPopMatrix();

	glColor3f(0.0, 0.0, blue);
	moveStar(30.0);


	/*This is the same with --the move a star function-- but using the translation function*/
	/*	if (star)
		{
			for (GLint i = 0; i < 25; i++)
			{
				glLoadIdentity();
				//Move from - left corner to top centre
				glTranslatef(0 + 10 * i, 250 + 10 * i, 0);
				drawStar(30.0, 0.0, 0.0);

				glLoadIdentity();
				//Move from top centre to right corner
				glTranslatef(250 + 10 * i, 500 - 10 * i, 0);
				drawStar(30.0, 0.0, 0.0);

				glLoadIdentity();
				//Move from right to lower centre corner
				glTranslatef(500 - 10 * i, 250 - 10 * i, 0);
				drawStar(30.0, 0.0, 0.0);

				glLoadIdentity();
				//Move from lower centre corner to the left
				glTranslatef(250 - 10 * i, 0 + 10 * i, 0);
				drawStar(30.0, 0.0, 0.0);
			}
		}*/

	glutSwapBuffers();

}

//Moves the star 
void moveStar(GLfloat size)
{
	//first we move the star until it reach the upper boundary
	//declare some local variable for x and y
	GLfloat x = starX;
	GLfloat y = starY;
	do
	{
		drawStar(size, x, y);
		//increment x any y
		x = x + 10;
		y = y + 10;
	} while (500 - y > 0.001);//floating point comparison with tolerance of 0.001
						   //now move diagonally down to right boundary
	do
	{
		drawStar(size, x, y);
		//increment x and decrement y
		x = x + 10;
		y = y - 10;
	} while (500 - x > 0.001);//floating point comparison with tolerance of 0.001
/*						   //move to lower centre - lower boundary
	do
	{
		drawStar(size, x, y);
		//decrement x any y
		x = x - 10;
		y = y - 10;
	} while (0 + y>0.001);//floating point comparison with tolerance of 0.001

						   //move to original position
	do
	{
		drawStar(size, x, y);
		//decrement x any increment y
		x = x - 10;
		y = y + 10;
	} while (0 + x>0.001);//floating point comparison with tolerance of 0.001*/

}

void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'q':
		exit(0);
		break;
	case 'a':
		star = 0.0;
		break;
	case 'd':
		star = 1.0;
		break;
	}
	glutPostRedisplay();
}


//Draws a 5 pointed star using lines
void drawStar(GLfloat radius, GLfloat x, GLfloat y)
{
	//x1,y1 is the first coordinate at 72 degrees from the unit circle
	//glColor3f(red, green, blue);
	GLfloat x1, y1, x2, y2, x3, y3, x4, y4, x5, y5;

	x1 = x + cos(72 * PI / 180) * radius;
	y1 = y + sin(72 * PI / 180) * radius;
	x2 = x + cos(144 * PI / 180) * radius;
	y2 = y + sin(144 * PI / 180) * radius;
	x3 = x + cos(216 * PI / 180) * radius;
	y3 = y + sin(216 * PI / 180) * radius;
	x4 = x + cos(288 * PI / 180) * radius;
	y4 = y + sin(288 * PI / 180) * radius;
	x5 = x + radius;
	y5 = y;

	//Change the width of the line. You can have a thicker line.
	glLineWidth(1.0);

	glBegin(GL_LINES);
	glVertex2f(x1, y1);
	glVertex2f(x3, y3);
	glVertex2f(x1, y1);
	glVertex2f(x4, y4);
	glVertex2f(x2, y2);
	glVertex2f(x4, y4);
	glVertex2f(x2, y2);
	glVertex2f(x5, y5);
	glVertex2f(x3, y3);
	glVertex2f(x5, y5);
	glEnd();

	glFlush();
}

/* graphics initialisation */
void init(void)
{
	/*glClearColor (0.0, 0.0, 0.0, 0.0);   /* window will be cleared to black */
	glClearColor(1.0, 0.0, 0.0, 0.0);     /* window will be cleared to red */
}

//rename this to main(...) and change example 2 to run this main function
int main(int argc, char** argv)
{
	/* window management code ... */
/* initialises GLUT and processes any command line arguments */
	glutInit(&argc, argv);
	/* use single-buffered window and RGBA colour model */
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	/* window width = 400 pixels, height = 400 pixels */
	/* window width = 640 pixels, height = 480 pixels for a 4:3 ascpect ratio */
	/* window width = 1024 pixels, height = 576 pixels for a 16:9 ascpect ratio */
	glutInitWindowSize(500, 500);
	/* window upper left corner at (100, 100) */
	glutInitWindowPosition(100, 100);
	/* creates an OpenGL window with command argument in its title bar */
	glutCreateWindow("Example 4");

	init();

	glutKeyboardFunc(keyInput);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}

