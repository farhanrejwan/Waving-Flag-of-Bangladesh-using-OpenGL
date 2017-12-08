#include <windows.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>
#include <stdlib.h>
#include <bits/stdc++.h>

using namespace std;

int wave_count = 0, on = 0, off = 0, p = 0;
double points[45][45][3], r = 1.85, s = 0.00681817;
double pi = acos(-1);

GLfloat hold;

void init(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glDepthFunc(GL_LEQUAL);

    for (int x=0; x<45; x++) {
        for (int y=0; y<45; y++) {
			points[x][y][0] = double((x / 3.0f) - 7.25f);
			points[x][y][1] = double((y / 5.0f) - 4.5f);
			points[x][y][2] = double(sin((((x / 5.0f) * 40.0f) / 360.0f) * pi * 2.0f));
		}
	}
}

void rectangle(void)
{
    int x, y;
	double double_x, double_y, double_xb, double_yb;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glColor3f(0.0, 0.4, 0.3);
	glTranslatef(0.0f, 0.0f, -12.0f);

	glBegin(GL_QUADS);
        for (x=0; x<44; x++) {
            for (y=0; y<44; y++) {
                double_x = double(x) / 44.0f;
                double_y = double(y) / 44.0f;
                double_xb = double(x + 1) / 44.0f;
                double_yb = double(y + 1) / 44.0f;

                glTexCoord2f(double_x, double_y);
                glVertex3f(points[x][y][0], points[x][y][1], points[x][y][2]);

                glTexCoord2f(double_x, double_yb);
                glVertex3f(points[x][y+1][0], points[x][y+1][1], points[x][y+1][2]);

                glTexCoord2f(double_xb, double_yb);
                glVertex3f(points[x+1][y+1][0], points[x+1][y+1][1], points[x+1][y+1][2]);

                glTexCoord2f(double_xb, double_y);
                glVertex3f(points[x+1][y][0], points[x+1][y][1], points[x+1][y][2]);
            }
        }
	glEnd();

	if (wave_count==2) {
		for (y=0; y<45; y++) {
			hold = points[0][y][2];

			for (x=0; x<44; x++) {
				points[x][y][2] = points[x+1][y][2];
			}

			points[44][y][2] = hold;
		}

		wave_count = 0;
	}

	wave_count++;
}

void circle(void)
{
    glColor3f(0.96, 0.16, 0.26);

	glBegin(GL_POLYGON);
        for (int i=0; i<100; i++) {
            double theta = (2 * pi * i) / 100;
            glVertex3f(r * cos(theta), r * sin(theta), 1.0);
        }
	glEnd();

	if (r<=1.7) {
        p = 1;
	}
	if (r>=2.0) {
        p = 0;
	}

	if (p) {
        r += s;
	}
	else {
        r -= s;
	}
}

void stand(void)
{
    glColor3f(0.45, 0.25, 0.13);

    glBegin(GL_QUADS);
        glVertex3f(-7.2, 6.0, -1.0);
        glVertex3f(-7.2, -6.0, -1.0);
        glVertex3f(-8.6, -6.0, -1.0);
        glVertex3f(-8.6, 6.0, -1.0);
    glEnd();

    glBegin(GL_QUADS);
        glVertex3f(-5.0, -6.0, -1.0);
        glVertex3f(-5.0, -7.0, -1.0);
        glVertex3f(-11.0, -7.0, -1.0);
        glVertex3f(-11.0, -6.0, -1.0);
    glEnd();
}

void display(void)
{
	rectangle();

	circle();

	stand();

	Sleep(5);

	glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glutPostRedisplay();
}

void timer(int unused)
{
    if (on==1 && off==1) {
        glutPostRedisplay();
        glutTimerFunc(30, timer, 0);
    }
}

void keyboard(unsigned char key, int x, int y)
{
    if (key==32) {
        if (on==0 && off==0) {
            on = 1;
            off = 1;
        }
        else if (on==1 && off==1) {
            on = 0;
            off = 0;
        }
    }
    else if (key==27) {
        glutDestroyWindow(1);
    }
    else {
        return;
    }

    glutTimerFunc(30, timer, 0);
}

void mouse(int button, int state, int x, int y)
{
    if (on==0 && off==0) {
        on = 1;
    }
    else if (on==1 && off==0) {
        off = 1;
    }
    else if (on==1 && off==1) {
        on = 0;
    }
    else if (on==0 && off==1) {
        off = 0;
    }

    glutTimerFunc(30, timer, 0);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(800, 480);
    glutInitWindowPosition(283, 100);
    glutCreateWindow("Waving Flag of Bangladesh");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(30, timer, 0);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMainLoop();

    return 0;
}
