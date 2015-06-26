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

int window_size_x = 500;
int window_size_y = window_size_x;
double node_Radius = 0.04;
int i = 0;
int j = 0;
int k = 0;
typedef struct{
	double x[3];
}position;
typedef struct{
	int number;
	int edge_flag;			//�m�[�h�ԍ�
	int none_flag;
	position pos;		//�ʒu
}node;
typedef struct{
	int number;
	int edge_flag;			//�m�[�h�ԍ�
	int none_flag;
	position pos;		//�ʒu
	position del_pos;	//���x
	position acc;		//�����x
	double color_grad;
}node2;
static node node_surface[2][2][2];
void sphere(double R, double precise, GLfloat sph_col[10]){
	// ���̕`��
	//�������\��
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, sph_col);
	GLUquadricObj *sphere; //�I�u�W�F�N�g�|�C���^������
	sphere = gluNewQuadric();	//�I�u�W�F�N�g�𐶐� 
	//�I�u�W�F�N�g�̕`��^�C�v��ݒ�i�ȗ��j
	gluQuadricDrawStyle(sphere, GLU_FILL);
	//����`�� ���a1.0�C�܌o���ꂼ��10.0����
	gluSphere(sphere, R, precise, precise);
}
void initiation(){
	
	node_surface[i][j][k].pos.x[0] = 3.0;
	node_surface[i][j][k].pos.x[1] = 3.0;
	node_surface[i][j][k].pos.x[2] = 0.0;

}
void node_simulation(){
	initiation();
	GLfloat changing[] = { 0.5, 0.5, 1.0, 1.0 };
	glPushMatrix();
	glCullFace(GL_BACK);
	glTranslated((GLdouble)node_surface[i][j][k].pos.x[0], (GLdouble)node_surface[i][j][k].pos.x[1], node_surface[i][j][k].pos.x[2]);
	sphere(node_Radius, 10.0, changing);
	glBegin(GL_POINT);
	glVertex3f(node_surface[i][j][k].pos.x[0], node_surface[i][j][k].pos.x[1], node_surface[i][j][k].pos.x[2]);

	glEnd();

	glPopMatrix();
}
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	node_simulation();
}
int main(int argc, char *argv[])
{

	glutInitWindowPosition(0, 0);
	glutInitWindowSize(window_size_x, window_size_y);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow(argv[0]);
	glutDisplayFunc(display);
	glutMainLoop();
	
	return 0;
	
}


	

