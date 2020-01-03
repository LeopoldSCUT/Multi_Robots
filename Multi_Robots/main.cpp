#include <GL/glut.h>
#include <cmath>

constexpr auto pi = 3.1415926;
constexpr auto grid_width = 20;
constexpr auto grid_length = 20;

static GLdouble pos_x = _int64(grid_width) - 1, pos_y = 5, pos_z = 2, direction = 0;
static GLdouble unit = 1, step = unit / 5;
// grid_flag是地图是否有障碍物的标记数据
static GLboolean grid_flag[grid_width][grid_length] =
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

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(pos_x + 6 * cos(direction / 180 * pi), pos_y, pos_z - 6 * sin(direction / 180 * pi),
        pos_x, 0, pos_z,
        0, 0.5, 0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glColor4f(1, 0, 0, 0.5);
    glBegin(GL_POLYGON);
    glVertex3d(pos_x, 0.1, pos_z);
    glVertex3d(pos_x + 0.5 * cos((direction + 15) / 180 * pi), 0.1, pos_z - 0.5 * sin((direction + 15) / 180 * pi));
    glVertex3d(pos_x + 0.35 * cos(direction / 180 * pi), 0.1, pos_z - 0.35 * sin(direction / 180 * pi));
    glVertex3d(pos_x + 0.5 * cos((direction - 15) / 180 * pi), 0.1, pos_z - 0.5 * sin((direction - 15) / 180 * pi));
    glEnd();

    for (auto i = 0; i < grid_width; ++i)
        for (auto j = 0; j < grid_length; ++j)
            map_cube(double(_int64(grid_width) - i) * unit, double(_int64(grid_width) - i + 1) * unit, j * unit,
                double(_int64(j) + 1) * unit, 0, unit, grid_flag[i][j]);
	
    glutSwapBuffers();
}

void keys(const int key, int x, int y)
{
	auto ready_x = pos_x, ready_z = pos_z;
    // 上下方向键控制前进和后退，左右键控制左右转向
    switch (key) {
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
    if (!grid_flag[int(grid_width - floor(ready_x))][int(floor(ready_z))]) {
        pos_x = ready_x;
        pos_z = ready_z;
    }
    display();
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutCreateWindow("基于OpenGL的多机器人目标围捕模拟");
    glEnable(GL_DEPTH_TEST);
    glutReshapeFunc(&reshape);
    glutDisplayFunc(&display);
    glutIdleFunc(&display);
    glutSpecialFunc(&keys);
    glutMainLoop();
    return 0;
}
