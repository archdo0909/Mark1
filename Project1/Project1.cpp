// Project1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <gl/glut.h>
#include <math.h>
#include <string.h>
#include <vector>

float c[3];
int num_count = 0;
typedef struct{
	double x[3];
}position;
typedef struct{
	int number;
	int edge_flag;			//ノード番号
	int none_flag;
	position pos;		//位置
}node;
typedef struct{
	int number;
	int edge_flag;			//ノード番号
	int none_flag;
	position pos;		//位置
	position del_pos;	//速度
	position acc;		//加速度
	double color_grad;
}node2;
static node2 node_surface2[4];
void initiation(){
	
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_DEPTH_TEST);

	gluPerspective(30.0, 1, 0.1, 1000.0);

	gluLookAt(
		0.0, -100.0, 50.0,
		0.0, 100.0, 0.0,
		0.0, 0.0, 1.0);

}
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glPushMatrix();
	for (int i = 0; i < 1; i++){
		glColor3d(1.0, 0.0, 0.0);
		glTranslated(node_surface2[i].pos.x[0], node_surface2[i].pos.x[1], node_surface2[i].pos.x[2]);
		glutSolidSphere(4.0, 20, 20);
	}
	glPopMatrix();

	glutSwapBuffers();
}
int main(int argc, char *argv[])
{
	int i;
	for (i = 0; i < 1; i++){
		printf("Put Cordinate %d\n", i + 1);
		scanf_s("%f, %f, %f", &c[0], &c[1], &c[2]);
		printf("%f %f %f", c[0], c[1], c[2]);
		node_surface2[i].pos.x[0] = c[0];
		node_surface2[i].pos.x[1] = c[1];
		node_surface2[i].pos.x[2] = c[2];
	}

	glutInit(&argc, argv);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(500, 500);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow(argv[0]);
	glutDisplayFunc(display);
	initiation();
	glutMainLoop();
	
	return 0;
	
}


	

