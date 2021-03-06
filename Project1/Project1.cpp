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

#define MAX 30
#define node_Num_m /*face_info[0].node_Num_w*/ 11
#define node_Num_n /*face_info[0].node_Num_h*/ 21
#define judge 0.0001
#define rec_x node_point[s][1].pos.x[0] - node_point[s][0].pos.x[0]
#define rec_y node_point[s][2].pos.x[1] - node_point[s][1].pos.x[1]
double c[3];
int window_size_x = 1000;
int window_size_y = window_size_x;
int w_view;
int h_view;
int num_count = 0;
int tri_count = 0;
double View_from[3] = { 20.0, 20.0, 20.0 };
double View_to[3] = { 0.0, 0.0, 1.0 };
typedef struct{
	double x[3];
}position;
typedef struct{
	int t[3];
	int color;
	double normal[3];
}triangle_d;
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
typedef struct{
	int number;
	int node_Num_w;
	int node_Num_h;
}face;
typedef struct{
	int number;
	position pos;
	face face;
}point;
static node2 node_surface2[10000];
static face face_info[3];
static point node_point[3][4]; //node_point[face Num][cordinate Num]
static triangle_d triangle_data[11 * 21 * 4];
void get_info(){
	int i;
	int j;
	printf("    1...........2       \n ");
	printf("    :         :       \n ");
	printf("    :         :       \n ");
	printf("   4:.........:3      \n ");

	for (i = 0; i < 3; i++){
		for (j = 0; j < 4; j++){
			printf("Put Coordinate %d from face %d\n", j + 1, i + 1);
			scanf_s("%lf, %lf", &c[0], &c[1]);
			node_point[i][j].pos.x[0] = c[0];
			node_point[i][j].pos.x[1] = c[1];
			node_point[i][j].face.number = i + 1;
		}
	}
	for (i = 0; i < 1; i++){
		printf("Put division for face %d\n", i + 1);
		scanf_s("%d, %d", &face_info[i].node_Num_w, &face_info[i].node_Num_h);
		face_info[i].number = i;
	}
	printf("node_point[0][0].pos.x[0] = %d\n", node_point[0][0].pos.x[0]);
	printf("node_point[0][0].pos.x[1] = %d\n", node_point[0][0].pos.x[1]);
	printf("node_point[0][1].pos.x[0] = %f\n", node_point[0][1].pos.x[0]);
	printf("node_point[0][1].pos.x[1] = %f\n", node_point[0][1].pos.x[1]);
}
void sphere(double R, double precise){

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLUquadricObj *sphere;

	sphere = gluNewQuadric();
	gluQuadricDrawStyle(sphere, GLU_FILL);

	gluSphere(sphere, 1.0, 10.0, 10.0);
}
void initiation(){
	int i = 0;
	int j = 0;
	int k = 0;
	int h = 0;
	int s = 0;
	int trirem1[3];
	int trirem2[3];
	int trirem3[3];
	int trirem4[3];
	int trirem5[3];
	int tri_flag1;
	int tri_flag2;
	int tri_flag3;
	int tri_flag4;
	int tri_flag5;
	double tritemp_x;
	double tritemp_y;
	double natural_length_x;
	double natural_length_y;


	int flag = 0;
	// face Number "S"
	for (s = 0; s < 3; s++){
		/*printf("node_point[0][0].pos.x[0] = %lf\n", node_point[0][0].pos.x[0]);
		printf("node_point[0][1].pos.x[0] = %lf\n", node_point[0][1].pos.x[0]);*/

	natural_length_x = ((double)(node_point[s][1].pos.x[0] - node_point[s][0].pos.x[0]) / (double)(face_info[s].node_Num_w - 1));
	natural_length_y = ((double)(node_point[s][2].pos.x[1] - node_point[s][1].pos.x[1]) / (double)(face_info[s].node_Num_h - 1));

		/*natural_length_x = fabs(((double)(node_point[0][0].pos.x[0] - node_point[0][1].pos.x[0])) / (double)(node_Num_m - 1));
		natural_length_y = fabs(((double)(node_point[0][1].pos.x[1] - node_point[0][2].pos.x[1])) / (double)(node_Num_n - 1));*/
	/*	natural_length_x = (3.5) / (double)(node_Num_m - 1);
		natural_length_y = (7.0) / (double)(node_Num_n - 1);*/
		printf("natural length x = %lf\n", natural_length_x);
		printf("natural length y = %lf\n", natural_length_y);

		static node node_surface[100][100][2];

		for (i = 0; i < node_Num_m; i++){
			for (j = 0; j < node_Num_n; j++){
				for (k = 0; k < 1; k++){
						if (i == 0 || j == 0 || i == node_Num_m - 1 || j == node_Num_n - 1){
							if (k == 0){
							node_surface[i][j][k].edge_flag = 1;
							node_surface[i][j][k].none_flag = 0;
							}
							else{
							node_surface[i][j][k].edge_flag = 0;
							node_surface[i][j][k].none_flag = 1;
							}
							}
							else{
							node_surface[i][j][k].edge_flag = 0;
							node_surface[i][j][k].none_flag = 0;
							}
						node_surface[i][j][k].pos.x[0] = (double)(node_point[s][0].pos.x[0] + (double)(i * natural_length_x));
						node_surface[i][j][k].pos.x[1] = (double)(node_point[s][0].pos.x[1] + (double)(j * natural_length_y));
						node_surface[i][j][k].pos.x[2] = (double)0.0;
					}
				}
			}
		for (i = 0; i < node_Num_m; i++){
			for (j = 0; j < node_Num_n; j++){
				for (k = 0; k < 1; k++){
					node_surface[i][j][k].number = num_count;
					num_count++;
				}
			}
		}
		printf("num_count = %d\n", num_count);
        //////////////////////////////////////////////////////////////////////////////////////
		// Mesh
		/////////////////////////////////////////////////////////////////////////////////////
		for (h = 0; h < num_count; h++){
			tri_flag3 = 0;
			if (h < node_Num_n * node_Num_m){
				for (i = 0; i <= (node_Num_m - 1) / 2; i++){
					for (j = 0; j <= (node_Num_n - 1) / 2; j++){
						for (k = 0; k < 1; k++){
							if (node_surface[i][j][k].number == h && node_surface[i][j][k].none_flag != 1){
								trirem1[0] = i;
								trirem1[1] = j;
								trirem1[2] = k;
								tri_flag3 = 1;
							}
						}
					}
				}
				tri_flag1 = 0;
				tri_flag2 = 0;
				tri_flag4 = 0;
				tri_flag5 = 0;
				for (i = 0; i <= (node_Num_m - 1) / 2; i++){
					for (j = 0; j <= (node_Num_n - 1) / 2; j++){
						for (k = 0; k < 1; k++){
							if (node_surface[i][j][k].none_flag != 1){
								tritemp_x = node_surface[trirem1[0]][trirem1[1]][trirem1[2]].pos.x[0] - node_surface[i][j][k].pos.x[0];
								tritemp_y = node_surface[trirem1[0]][trirem1[1]][trirem1[2]].pos.x[1] - node_surface[i][j][k].pos.x[1];
								if (fabs(tritemp_x + natural_length_x) < judge && fabs(tritemp_y) < judge){
									trirem2[0] = i;
									trirem2[1] = j;
									trirem2[2] = k;
									tri_flag1 = 1;
								}
								if (fabs(tritemp_x + natural_length_x) < judge && fabs(tritemp_y + natural_length_y) < judge){
									trirem3[0] = i;
									trirem3[1] = j;
									trirem3[2] = k;
									tri_flag2 = 1;
								}
								if (fabs(tritemp_x + natural_length_x) < judge && fabs(tritemp_y + natural_length_y) < judge){
									trirem4[0] = i;
									trirem4[1] = j;
									trirem4[2] = k;
									tri_flag4 = 1;
								}
								if (fabs(tritemp_x) < judge && fabs(tritemp_y + natural_length_y) < judge){
									trirem5[0] = i;
									trirem5[1] = j;
									trirem5[2] = k;
									tri_flag5 = 1;
								}
							}
						}
					}
				}
				if (tri_flag1 == 1 && tri_flag2 == 1 && tri_flag3 == 1){
					if (tri_count == 0){
						triangle_data[tri_count].t[0] = h;
						triangle_data[tri_count].t[1] = node_surface[trirem2[0]][trirem2[1]][trirem2[2]].number;
						triangle_data[tri_count].t[2] = node_surface[trirem3[0]][trirem3[1]][trirem3[2]].number;
						triangle_data[tri_count].color = 1;
						tri_count++;
					}
					else{
						flag = 0;
						for (i = 0; i < tri_count; i++){
							if ((triangle_data[i].t[0] == h) && (triangle_data[i].t[1] == node_surface[trirem2[0]][trirem2[1]][trirem2[2]].number)
								&& (triangle_data[i].t[2] = node_surface[trirem3[0]][trirem3[1]][trirem3[2]].number)){
								flag = 1;
							}
							//printf("a\n");
						}
						if (flag == 0){
							triangle_data[tri_count].t[0] = h;
							triangle_data[tri_count].t[1] = node_surface[trirem2[0]][trirem2[1]][trirem2[2]].number;
							triangle_data[tri_count].t[2] = node_surface[trirem3[0]][trirem3[1]][trirem3[2]].number;
							triangle_data[tri_count].color = 1;
							tri_count++;
						}
						else{
							flag = 0;
						}
					}
				}
				if (tri_flag4 == 1 && tri_flag5 == 1 && tri_flag3 == 1){
					if (tri_count == 0){
						triangle_data[tri_count].t[0] = h;
						triangle_data[tri_count].t[1] = node_surface[trirem4[0]][trirem4[1]][trirem4[2]].number;
						triangle_data[tri_count].t[2] = node_surface[trirem5[0]][trirem5[1]][trirem5[2]].number;
						triangle_data[tri_count].color = 1;
						tri_count++;
					}
					else{
						flag = 0;
						for (i = 0; i < tri_count; i++){
							if ((triangle_data[i].t[0] == h) && (triangle_data[i].t[1] == node_surface[trirem4[0]][trirem4[1]][trirem4[2]].number)
								&& (triangle_data[i].t[2] = node_surface[trirem5[0]][trirem5[1]][trirem5[2]].number)){
								flag = 1;
							}
							//printf("a\n");
						}
						if (flag == 0){
							triangle_data[tri_count].t[0] = h;
							triangle_data[tri_count].t[1] = node_surface[trirem4[0]][trirem4[1]][trirem4[2]].number;
							triangle_data[tri_count].t[2] = node_surface[trirem5[0]][trirem5[1]][trirem5[2]].number;
							triangle_data[tri_count].color = 1;
							tri_count++;
						}
						else{
							flag = 0;
						}
					}
				}
			}
		}
		//右上
		for (h = 0; h < num_count; h++){
			tri_flag3 = 0;
			if (h < node_Num_n * node_Num_m){
				for (i = (node_Num_m - 1) / 2; i <= node_Num_m; i++){
					for (j = 0; j <= (node_Num_n - 1) / 2; j++){
						for (k = 0; k < 1; k++){
							if (node_surface[i][j][k].number == h && node_surface[i][j][k].none_flag != 1){
								trirem1[0] = i;
								trirem1[1] = j;
								trirem1[2] = k;
								tri_flag3 = 1;
							}
						}
					}
				}
				tri_flag1 = 0;
				tri_flag2 = 0;
				tri_flag4 = 0;
				tri_flag5 = 0;
				for (i = (node_Num_m - 1) / 2; i <= node_Num_m; i++){
					for (j = 0; j <= (node_Num_n - 1) / 2; j++){
						for (k = 0; k < 1; k++){
							if (node_surface[i][j][k].none_flag != 1){
								tritemp_x = node_surface[trirem1[0]][trirem1[1]][trirem1[2]].pos.x[0] - node_surface[i][j][k].pos.x[0];
								tritemp_y = node_surface[trirem1[0]][trirem1[1]][trirem1[2]].pos.x[1] - node_surface[i][j][k].pos.x[1];
								if (fabs(tritemp_x + natural_length_x) < judge && fabs(tritemp_y) < judge){
									trirem2[0] = i;
									trirem2[1] = j;
									trirem2[2] = k;
									tri_flag1 = 1;
								}
								if (fabs(tritemp_x) < judge && fabs(tritemp_y + natural_length_y) < judge){
									trirem3[0] = i;
									trirem3[1] = j;
									trirem3[2] = k;
									tri_flag2 = 1;
								}
								if (fabs(tritemp_x - natural_length_x) < judge && fabs(tritemp_y) < judge){
									trirem4[0] = i;
									trirem4[1] = j;
									trirem4[2] = k;
									tri_flag4 = 1;
								}
								if (fabs(tritemp_x) < judge && fabs(tritemp_y - natural_length_y) < judge){
									trirem5[0] = i;
									trirem5[1] = j;
									trirem5[2] = k;
									tri_flag5 = 1;
								}
							}
						}
					}
				}
				if (tri_flag1 == 1 && tri_flag2 == 1 && tri_flag3 == 1){
					flag = 0;
					for (i = 0; i < tri_count; i++){
						if ((triangle_data[i].t[0] == h) && (triangle_data[i].t[1] == node_surface[trirem2[0]][trirem2[1]][trirem2[2]].number)
							&& (triangle_data[i].t[2] = node_surface[trirem3[0]][trirem3[1]][trirem3[2]].number)){
							flag = 1;
						}
						//printf("a\n");
					}
					if (flag == 0){
						triangle_data[tri_count].t[0] = h;
						triangle_data[tri_count].t[1] = node_surface[trirem2[0]][trirem2[1]][trirem2[2]].number;
						triangle_data[tri_count].t[2] = node_surface[trirem3[0]][trirem3[1]][trirem3[2]].number;
						triangle_data[tri_count].color = 1;
						tri_count++;
					}
					else{
						flag = 0;
					}
				}
				if (tri_flag4 == 1 && tri_flag5 == 1 && tri_flag3 == 1){
					flag = 0;
					for (i = 0; i < tri_count; i++){
						if ((triangle_data[i].t[0] == h) && (triangle_data[i].t[1] == node_surface[trirem4[0]][trirem4[1]][trirem4[2]].number)
							&& (triangle_data[i].t[2] = node_surface[trirem5[0]][trirem5[1]][trirem5[2]].number)){
							flag = 1;
						}
						//printf("a\n");
					}
					if (flag == 0){
						triangle_data[tri_count].t[0] = h;
						triangle_data[tri_count].t[1] = node_surface[trirem4[0]][trirem4[1]][trirem4[2]].number;
						triangle_data[tri_count].t[2] = node_surface[trirem5[0]][trirem5[1]][trirem5[2]].number;
						triangle_data[tri_count].color = 1;
						tri_count++;
					}
					else{
						flag = 0;
					}
				}
			}
		}
		//左下
		for (h = 0; h < num_count; h++){
			tri_flag3 = 0;
			if (h < node_Num_n * node_Num_m){
				for (i = 0; i <= (node_Num_m - 1) / 2; i++){
					for (j = (node_Num_n - 1) / 2; j <= node_Num_n; j++){
						for (k = 0; k < 1; k++){
							if (node_surface[i][j][k].number == h && node_surface[i][j][k].none_flag != 1){
								trirem1[0] = i;
								trirem1[1] = j;
								trirem1[2] = k;
								tri_flag3 = 1;
							}
						}
					}
				}
				tri_flag1 = 0;
				tri_flag2 = 0;
				tri_flag4 = 0;
				tri_flag5 = 0;
				for (i = 0; i <= (node_Num_m - 1) / 2; i++){
					for (j = (node_Num_n - 1) / 2; j <= node_Num_n; j++){
						for (k = 0; k < 1; k++){
							if (node_surface[i][j][k].none_flag != 1){
								tritemp_x = node_surface[trirem1[0]][trirem1[1]][trirem1[2]].pos.x[0] - node_surface[i][j][k].pos.x[0];
								tritemp_y = node_surface[trirem1[0]][trirem1[1]][trirem1[2]].pos.x[1] - node_surface[i][j][k].pos.x[1];
								if (fabs(tritemp_x + natural_length_x) < judge && fabs(tritemp_y) < judge){
									trirem2[0] = i;
									trirem2[1] = j;
									trirem2[2] = k;
									tri_flag1 = 1;
								}
								if (fabs(tritemp_x)< judge && fabs(tritemp_y + natural_length_y) < judge){
									trirem3[0] = i;
									trirem3[1] = j;
									trirem3[2] = k;
									tri_flag2 = 1;
								}
								if (fabs(tritemp_x - natural_length_x) < judge && fabs(tritemp_y) < judge){
									trirem4[0] = i;
									trirem4[1] = j;
									trirem4[2] = k;
									tri_flag4 = 1;
								}
								if (fabs(tritemp_x) < judge && fabs(tritemp_y - natural_length_y) < judge){
									trirem5[0] = i;
									trirem5[1] = j;
									trirem5[2] = k;
									tri_flag5 = 1;
								}
							}
						}
					}
				}
				if (tri_flag1 == 1 && tri_flag2 == 1 && tri_flag3 == 1){
					if (tri_count == 0){
						triangle_data[tri_count].t[0] = h;
						triangle_data[tri_count].t[1] = node_surface[trirem2[0]][trirem2[1]][trirem2[2]].number;
						triangle_data[tri_count].t[2] = node_surface[trirem3[0]][trirem3[1]][trirem3[2]].number;
						triangle_data[tri_count].color = 1;
						tri_count++;
					}
					else{
						flag = 0;
						for (i = 0; i < tri_count; i++){
							if ((triangle_data[i].t[0] == h) && (triangle_data[i].t[1] == node_surface[trirem2[0]][trirem2[1]][trirem2[2]].number)
								&& (triangle_data[i].t[2] = node_surface[trirem3[0]][trirem3[1]][trirem3[2]].number)){
								flag = 1;
							}
							//printf("a\n");
						}
						if (flag == 0){
							triangle_data[tri_count].t[0] = h;
							triangle_data[tri_count].t[1] = node_surface[trirem2[0]][trirem2[1]][trirem2[2]].number;
							triangle_data[tri_count].t[2] = node_surface[trirem3[0]][trirem3[1]][trirem3[2]].number;
							triangle_data[tri_count].color = 1;
							tri_count++;
						}
						else{
							flag = 0;
						}
					}
				}
				if (tri_flag4 == 1 && tri_flag5 == 1 && tri_flag3 == 1){
					if (tri_count == 0){
						triangle_data[tri_count].t[0] = h;
						triangle_data[tri_count].t[1] = node_surface[trirem4[0]][trirem4[1]][trirem4[2]].number;
						triangle_data[tri_count].t[2] = node_surface[trirem5[0]][trirem5[1]][trirem5[2]].number;
						triangle_data[tri_count].color = 1;
						tri_count++;
					}
					else{
						flag = 0;
						for (i = 0; i < tri_count; i++){
							if ((triangle_data[i].t[0] == h) && (triangle_data[i].t[1] == node_surface[trirem4[0]][trirem4[1]][trirem4[2]].number)
								&& (triangle_data[i].t[2] = node_surface[trirem5[0]][trirem5[1]][trirem5[2]].number)){
								flag = 1;
							}
							//printf("a\n");
						}
						if (flag == 0){
							triangle_data[tri_count].t[0] = h;
							triangle_data[tri_count].t[1] = node_surface[trirem4[0]][trirem4[1]][trirem4[2]].number;
							triangle_data[tri_count].t[2] = node_surface[trirem5[0]][trirem5[1]][trirem5[2]].number;
							triangle_data[tri_count].color = 1;
							tri_count++;
						}
						else{
							flag = 0;
						}
					}
				}
			}
		}
		//右下
		for (h = 0; h < num_count; h++){
			tri_flag3 = 0;
			if (h < node_Num_n * node_Num_m){
				for (i = (node_Num_m - 1) / 2; i <= node_Num_m; i++){
					for (j = (node_Num_n - 1) / 2; j <= node_Num_n; j++){
						for (k = 0; k < 1; k++){
							if (node_surface[i][j][k].number == h && node_surface[i][j][k].none_flag != 1){
								trirem1[0] = i;
								trirem1[1] = j;
								trirem1[2] = k;
								tri_flag3 = 1;
							}
						}
					}
				}
				tri_flag1 = 0;
				tri_flag2 = 0;
				tri_flag4 = 0;
				tri_flag5 = 0;
				for (i = (node_Num_m - 1) / 2; i <= node_Num_m; i++){
					for (j = (node_Num_n - 1) / 2; j <= node_Num_n; j++){
						for (k = 0; k < 1; k++){
							if (node_surface[i][j][k].none_flag != 1){
								tritemp_x = node_surface[trirem1[0]][trirem1[1]][trirem1[2]].pos.x[0] - node_surface[i][j][k].pos.x[0];
								tritemp_y = node_surface[trirem1[0]][trirem1[1]][trirem1[2]].pos.x[1] - node_surface[i][j][k].pos.x[1];
								if (fabs(tritemp_x + natural_length_x) < judge && fabs(tritemp_y) < judge){
									trirem2[0] = i;
									trirem2[1] = j;
									trirem2[2] = k;
									tri_flag1 = 1;
								}
								if (fabs(tritemp_x + natural_length_x) < judge && fabs(tritemp_y + natural_length_y) < judge){
									trirem3[0] = i;
									trirem3[1] = j;
									trirem3[2] = k;
									tri_flag2 = 1;
								}
								if (fabs(tritemp_x + natural_length_x) < judge && fabs(tritemp_y + natural_length_y) < judge){
									trirem4[0] = i;
									trirem4[1] = j;
									trirem4[2] = k;
									tri_flag4 = 1;
								}
								if (fabs(tritemp_x) < judge && fabs(tritemp_y + natural_length_y) < judge){
									trirem5[0] = i;
									trirem5[1] = j;
									trirem5[2] = k;
									tri_flag5 = 1;
								}
							}
						}
					}
				}
				if (tri_flag1 == 1 && tri_flag2 == 1 && tri_flag3 == 1){
					flag = 0;
					for (i = 0; i < tri_count; i++){
						if ((triangle_data[i].t[0] == h) && (triangle_data[i].t[1] == node_surface[trirem2[0]][trirem2[1]][trirem2[2]].number)
							&& (triangle_data[i].t[2] = node_surface[trirem3[0]][trirem3[1]][trirem3[2]].number)){
							flag = 1;
						}
						//printf("a\n");
					}
					if (flag == 0){
						triangle_data[tri_count].t[0] = h;
						triangle_data[tri_count].t[1] = node_surface[trirem2[0]][trirem2[1]][trirem2[2]].number;
						triangle_data[tri_count].t[2] = node_surface[trirem3[0]][trirem3[1]][trirem3[2]].number;
						triangle_data[tri_count].color = 1;
						tri_count++;
					}
					else{
						flag = 0;
					}
				}
				if (tri_flag4 == 1 && tri_flag5 == 1 && tri_flag3 == 1){
					flag = 0;
					for (i = 0; i < tri_count; i++){
						if ((triangle_data[i].t[0] == h) && (triangle_data[i].t[1] == node_surface[trirem4[0]][trirem4[1]][trirem4[2]].number)
							&& (triangle_data[i].t[2] = node_surface[trirem5[0]][trirem5[1]][trirem5[2]].number)){
							flag = 1;
						}
					}
					if (flag == 0){
						triangle_data[tri_count].t[0] = h;
						triangle_data[tri_count].t[1] = node_surface[trirem4[0]][trirem4[1]][trirem4[2]].number;
						triangle_data[tri_count].t[2] = node_surface[trirem5[0]][trirem5[1]][trirem5[2]].number;
						triangle_data[tri_count].color = 1;
						tri_count++;
					}
					else{
						flag = 0;
					}
				}
			}
		}

		for (i = 0; i < node_Num_m; i++){
			for (j = 0; j < node_Num_n; j++){
				for (k = 0; k < 1; k++){
					if (node_surface[i][j][k].none_flag != 1){
						for (h = 0; h < 3; h++){
							node_surface2[node_surface[i][j][k].number].pos.x[h] = node_surface[i][j][k].pos.x[h];
						}
					}
				}
			}
		}
	}
}
void display(void)
{
		int i = 0;
		int s = 0;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		for (i = 0; i < num_count; i++){
			glPushMatrix();
			glColor3d(0.0, 1.0, 1.0);
			/*printf("%lf, %lf, %lf\n", node_surface2[i].pos.x[0], node_surface2[i].pos.x[1], node_surface2[i].pos.x[2]);*/
			glTranslated((GLdouble)node_surface2[i].pos.x[0], (GLdouble)node_surface2[i].pos.x[2], (GLdouble)node_surface2[i].pos.x[1]);
			glutSolidSphere(0.08, 10, 10);
			glPopMatrix();
		}
		glPushMatrix();

		for (i = 0; i < tri_count; i++){
			if (triangle_data[i].color == 1){
				glCullFace(GL_FRONT);
			}
			else{
				glCullFace(GL_BACK);
			}
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glBegin(GL_TRIANGLES);
			//printf("%f %f %f %f\n", blue[0], blue[1], blue[2], blue[3]);
			//glNormal3d(-triangle_data[i].normal[0], -triangle_data[i].normal[2], -triangle_data[i].normal[1]);
			glNormal3d(triangle_data[i].normal[0], triangle_data[i].normal[2], triangle_data[i].normal[1]);
			//glNormal3d(-triangle_data[i].normal[0],- triangle_data[i].normal[1], -triangle_data[i].normal[2]);
			glVertex3d(node_surface2[triangle_data[i].t[0]].pos.x[0], node_surface2[triangle_data[i].t[0]].pos.x[2], node_surface2[triangle_data[i].t[0]].pos.x[1]);
			glVertex3d(node_surface2[triangle_data[i].t[1]].pos.x[0], node_surface2[triangle_data[i].t[1]].pos.x[2], node_surface2[triangle_data[i].t[1]].pos.x[1]);
			glVertex3d(node_surface2[triangle_data[i].t[2]].pos.x[0], node_surface2[triangle_data[i].t[2]].pos.x[2], node_surface2[triangle_data[i].t[2]].pos.x[1]);
			/*glVertex3d(node_surface2[triangle_data[i].t[0]].pos.x[0] - rec_x / 2.0, node_surface2[triangle_data[i].t[0]].pos.x[2], node_surface2[triangle_data[i].t[0]].pos.x[1] - rec_y / 2.0);
			glVertex3d(node_surface2[triangle_data[i].t[1]].pos.x[0] - rec_x / 2.0, node_surface2[triangle_data[i].t[1]].pos.x[2], node_surface2[triangle_data[i].t[1]].pos.x[1] - rec_y / 2.0);
			glVertex3d(node_surface2[triangle_data[i].t[2]].pos.x[0] - rec_x / 2.0, node_surface2[triangle_data[i].t[2]].pos.x[2], node_surface2[triangle_data[i].t[2]].pos.x[1] - rec_y / 2.0);*/
		}
		glEnd();
		glPopMatrix();

		/*glPushMatrix();
		glColor3d(0.0, 1.0, 1.0);
		glTranslated(node_surface2[1].pos.x[0], node_surface2[1].pos.x[1], node_surface2[1].pos.x[2]);
		printf("node_surface2[1].pos.x[0] = %lf\n", node_surface2[1].pos.x[0]);
		printf("node_surface2[1].pos.x[1] = %lf\n", node_surface2[1].pos.x[1]);
		printf("node_surface2[1].pos.x[2] = %lf\n", node_surface2[1].pos.x[2]);
		glTranslated(0, 0.7, 0);
		glutSolidSphere(0.2, 20, 20);
		glPopMatrix();*/

		glutSwapBuffers();

}
void idle(void)
{
	glutPostRedisplay();
}
void init(){
	
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_DEPTH_TEST);

	gluPerspective(30.0, 1, 0.1, 1000.0);

	gluLookAt(View_from[0], View_from[1], View_from[2], View_to[0], View_to[1], View_to[2], 0.0, 1.0, 0.0);//正面
	
}
int main(int argc, char *argv[])
{
	get_info();

	glutInit(&argc, argv);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(window_size_x, window_size_y);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow(argv[0]);
	initiation();
	init();
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutMainLoop();
	
	return 0;
	
}


	

