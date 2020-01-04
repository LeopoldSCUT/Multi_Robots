#include <GL/glut.h>
#include <cmath>
#include <cstdio>
#include <algorithm>
#include <ctime>
#include "Police.h"
#include "Thief.h"

using namespace std;

constexpr auto pi = 3.1415926;
constexpr auto grid_width = 20;
constexpr auto grid_length = 20;

static GLdouble pos_x = _int64(grid_width) - 1, pos_y = 5, pos_z = 2;
static GLdouble unit = 1, step = unit / 5;
static double rotate_angle = 0, direction = 0, rotate_radius = max(grid_length, grid_width) * unit + 5;
static double center_x = grid_length * unit / 2, center_y = grid_width * unit / 2;

Thief* thief = nullptr;
Police* police_0 = nullptr;
Police* police_1 = nullptr;
Police* police_2 = nullptr;
Police* police_3 = nullptr;

// grid_flag是地图是否有障碍物的标记数据 test
bool grid_flag[grid_width][grid_length] =
{ {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
 {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
 {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
 {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
 {1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1},
 {1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1},
 {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
 {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
 {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
 {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
 {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
 {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
 {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
 {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
 {1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1},
 {1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1},
 {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
 {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
 {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
 {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1} };

void simulation_init()
{
    srand(time(nullptr));
    thief = new Thief(12, 10, grid_flag);
    // thief->pos[0] = 19;
    // thief->pos[1] = 2;
    police_0 = new Police(19, 2);
    police_1 = new Police(19, 2);
    police_2 = new Police(19, 2);
    police_3 = new Police(19, 2);

}

void polygon(double(*vertices)[3], const int a, const int b, const int c, const int d)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glColor3d(1.0, 1.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex3dv(vertices[a]);
    glVertex3dv(vertices[b]);
    glVertex3dv(vertices[c]);
    glVertex3dv(vertices[d]);
    glEnd();

    glLineWidth(4);
    glColor3d(0.6, 0.4, 0);
    glBegin(GL_LINE_LOOP);
    glVertex3dv(vertices[a]);
    glVertex3dv(vertices[b]);
    glVertex3dv(vertices[c]);
    glVertex3dv(vertices[d]);
    glEnd();
}

void map_cube(const double x1, const double x2, const double y1, const double y2,
    const double z1, const double z2, const bool is_cube)
{
    GLdouble vertices[8][3] = { {x1, z1, y1}, {x1, z1, y2}, {x1, z2, y1},
                               {x1, z2, y2}, {x2, z1, y1}, {x2, z1, y2},
                               {x2, z2, y1}, {x2, z2, y2} };
    if (is_cube) {
        polygon(vertices, 0, 1, 5, 4);
        polygon(vertices, 4, 5, 7, 6);
        polygon(vertices, 2, 3, 7, 6);
        polygon(vertices, 0, 1, 3, 2);
        polygon(vertices, 5, 1, 3, 7);
        polygon(vertices, 0, 4, 6, 2);
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glColor3d(0.41, 0.41, 0.41);
        glBegin(GL_POLYGON);
        glVertex3dv(vertices[0]);
        glVertex3dv(vertices[1]);
        glVertex3dv(vertices[5]);
        glVertex3dv(vertices[4]);
        glEnd();

        glLineWidth(4);
        glColor4d(0.9, 0.86, 0.4, 1);
        glBegin(GL_LINE_LOOP);
        glVertex3dv(vertices[0]);
        glVertex3dv(vertices[1]);
        glVertex3dv(vertices[5]);
        glVertex3dv(vertices[4]);
        glEnd();
    }
}

void reshape(const int width, const int height)
{
    glViewport(0, 0, GLsizei(width), GLsizei(height));
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    const GLdouble ratio = width >= height ? width / height : height / width;
    gluPerspective(30, ratio, 1, 1000);
    glMatrixMode(GL_MODELVIEW);
}

void robot(const double x, const double y, const double z, const bool is_target)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    if (is_target)
		glColor4d(1.0, 0.0, 0.0, 0.5);
    else
        glColor4d(0.0, 0.0, 1.0, 0.5);
	// TODO: 画个线圈表示视野范围 
	glBegin(GL_POLYGON);
    glVertex3d(x, 0.1, z);
    glVertex3d(x + 0.5 * cos((direction + 15) / 180 * pi), 0.1, z - 0.5 * sin((direction + 15) / 180 * pi));
    glVertex3d(x + 0.35 * cos(direction / 180 * pi), 0.1, z - 0.35 * sin(direction / 180 * pi));
    glVertex3d(x + 0.5 * cos((direction - 15) / 180 * pi), 0.1, z - 0.5 * sin((direction - 15) / 180 * pi));
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

	// 旧的镜头转换
    // gluLookAt(pos_x + 6 * cos(direction / 180 * pi), pos_y, pos_z - 6 * sin(direction / 180 * pi),
    //     pos_x, 0, pos_z,
    //     0, 0.5, 0);

    // 新的镜头转换
    gluLookAt(center_x + rotate_radius * cos(rotate_angle / 180 * pi), 10, center_y + rotate_radius * sin(rotate_angle / 180 * pi),
        center_x, 0, center_y,
        0, 0.5, 0);

    double police_pos[4][2] = { 
    	{police_0->pos[0], police_0->pos[1]},
    	{police_1->pos[0], police_1->pos[1]},
    	{police_2->pos[0], police_2->pos[1]},
    	{police_3->pos[0], police_3->pos[1]} };
    int police_state[4] = { police_0->status, police_1->status, police_2->status, police_3->status };

    thief->update(police_pos, police_state);
	
	// 目标
    robot(thief->pos[0], pos_y, thief->pos[1], true);
    // 警察
    robot(police_0->pos[0], pos_y, police_0->pos[1], false);
    robot(police_1->pos[0], pos_y, police_1->pos[1], false);
    robot(police_2->pos[0], pos_y, police_2->pos[1], false);
    robot(police_3->pos[0], pos_y, police_3->pos[1], false);


	// 绘制地图和障碍物
    for (auto i = 0; i < grid_width; ++i)
        for (auto j = 0; j < grid_length; ++j)
            map_cube(double(_int64(grid_width) - i) * unit, double(_int64(grid_width) - i + 1) * unit, j * unit,
                double(_int64(j) + 1) * unit, 0, unit, grid_flag[i][j]);

	
    glutSwapBuffers();
}

void char_keys(const unsigned char key, int x, int y)
{
	// A/D 控制地图左右水平旋转
	// TODO: W/S控制地图视角上下
	switch (key)
	{
    case 'A':
    case 'a':
		rotate_angle += 5;
		break;
    case 'D':
    case 'd':
        rotate_angle -= 5;
		break;
    default:
		break;
	}
    printf("X: %f, Z: %f\n", thief->pos[0], thief->pos[1]);
    display();
}

void direct_keys(const int key, int x, int y)
{
	auto ready_x = pos_x, ready_z = pos_z;
    // 上下方向键控制前进和后退，左右键控制左右转向
    switch (key)
	{
    case GLUT_KEY_UP:
    	ready_x -= step * cos(direction / 180 * pi);
        ready_z += step * sin(direction / 180 * pi);
        break;
    case GLUT_KEY_DOWN:
        ready_x += step * cos(direction / 180 * pi);
        ready_z -= step * sin(direction / 180 * pi);
        break;
    case GLUT_KEY_RIGHT:
        direction -= 5;
        break;
    case GLUT_KEY_LEFT:
        direction += 5;
        break;
    default:
        break;
    }
    if (!grid_flag[int(grid_width - floor(ready_x))][int(floor(ready_z))])
    {
        pos_x = ready_x;
        pos_z = ready_z;
    }

    display();
}

int main(int argc, char* argv[])
{
    simulation_init();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutCreateWindow("基于OpenGL的多机器人目标围捕模拟");
    glEnable(GL_DEPTH_TEST);
    glutReshapeFunc(&reshape);
    glutDisplayFunc(&display);
    glutIdleFunc(&display);
    glutKeyboardFunc(&char_keys);
    glutSpecialFunc(&direct_keys);
    glutMainLoop();
    return 0;
}
