#include <GL/glut.h>
#include <iostream>
#include <cmath>
#include <utility>
using namespace std;

const int INSIDE = 0;
const int LEFT = 1;
const int RIGHT = 2;
const int BOTTOM = 4;
const int TOP = 8;

double x_min = 50, y_min = 50, x_max = 100, y_max = 100;
double x_one,y_one,x_two,y_two;

void plotPoint(int x, int y) {
glBegin(GL_POINTS);
glVertex2i(x, y);
glEnd();
}

void DDA(double x1, double y1, double x2, double y2) {
double dx = x2 - x1;
double dy = y2 - y1;
int steps = max(abs(dx), abs(dy));
double xInc = dx / steps;
double yInc = dy / steps;
double x = x1;
double y = y1;
for (int i = 0; i <= steps; i++) {
plotPoint(round(x), round(y));
x += xInc;
y += yInc;
}
}

int computeCode(double x, double y) {
    int code = INSIDE;
    if (x < x_min) code |= LEFT;
    else if (x > x_max) code |= RIGHT;
    if (y < y_min) code |= BOTTOM;
    else if (y > y_max) code |= TOP;
    return code;
}
void cohenSutherlandClip(double x1, double y1, double x2, double y2) {
    int code1 = computeCode(x1, y1);
    int code2 = computeCode(x2, y2);
    bool accept = false;

    while (true) {
        if ((code1 == 0) && (code2 == 0)) {
            // Trivially accepted
            accept = true;
            break;
        } else if (code1 & code2) {
            // Trivially rejected
            break;
        } else {
            // Calculate line segment to clip from outside point
            int out_code = code1 ? code1 : code2;
            double x, y;

            if (out_code & TOP) {
                x = x1 + (x2 - x1) * (y_max - y1) / (y2 - y1);
                y = y_max;
            } else if (out_code & BOTTOM) {
                x = x1 + (x2 - x1) * (y_min - y1) / (y2 - y1);
                y = y_min;
            } else if (out_code & RIGHT) {
                y = y1 + (y2 - y1) * (x_max - x1) / (x2 - x1);
                x = x_max;
            } else {
                y = y1 + (y2 - y1) * (x_min - x1) / (x2 - x1);
                x = x_min;
            }

            if (out_code == code1) {
                x1 = x;
                y1 = y;
                code1 = computeCode(x1, y1);
            } else {
                x2 = x;
                y2 = y;
                code2 = computeCode(x2, y2);
            }
        }
    }

    if (accept) {
       DDA(x1,y1,x2,y2);
    } else {
        cout << "Line rejected.\n";
    }
}


void display() {
glClear(GL_COLOR_BUFFER_BIT);
glColor3f(0.0, 0.0, 1.0);
DDA(x_one,y_one,x_two,y_two);
glColor3f(1.0, 0.0, 0.0);
cohenSutherlandClip(x_one,y_one,x_two,y_two);

glFlush();
}
void init() {
glClearColor(0.0, 0.0, 0.0, 0.0);
glColor3f(0.0, 0.0, 0.0);
glPointSize(2.0);
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluOrtho2D(-300, 300, -300, 300);
}
int main(int argc, char** argv) {
    cin>>x_one>>y_one>>x_two>>y_two;
glutInit(&argc, argv);
glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
glutInitWindowSize(600, 600);
glutInitWindowPosition(100, 100);
glutCreateWindow("Sutherland");
init();
glutDisplayFunc(display);
glutMainLoop();}
