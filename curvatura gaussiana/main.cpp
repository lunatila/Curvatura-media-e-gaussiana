#include <GLUT/glut.h>
#include "Funcao.hpp"
#include "Iluminacao.hpp"

int xm, xb, ym, yb;

float Gamma = 90;
float phi = 45;
float theta = 135;
float scale = 1;
Funcao f;
iluminacao I;

void draw_eixos()
{
    glColor3f(1.0,0.0,0.0);
    glBegin(GL_LINES);
     glVertex3f(10.0,0.0,0.0);
     glVertex3f(0.0,0.0,0.0);
    glEnd();
    glColor3f(0.0,1.0,0.0);
    glBegin(GL_LINES);
     glVertex3f(0.0,10.0,0.0);
     glVertex3f(0.0,0.0,0.0);
    glEnd();
    glColor3f(0.0,0.0,1.0);
    glBegin(GL_LINES);
     glVertex3f(0.0,0.0,10.0);
     glVertex3f(0.0,0.0,0.0);
    glEnd();
}
void display()
{
    glClearColor(0.0,0.0,0.0,0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    draw_eixos();
    //I.light_on();
    //glColor3f(1,0,0);
    f.plota_funcao_02();
    //f.plota_normal();
    //I.render_light();
    //I.light_off();
    glColor3f(0, 0, 1);
    f.visualiza_ms();
    glutSwapBuffers();
}
void inicia_config()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-2.0,2.0,-2.0,2.0,-10.0,10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(Gamma,0.0,0.0,1.0);
    glRotatef(phi,0.0,1.0,0.0);
    glRotatef(theta,0.0,0.0,1.0);
    glScalef(scale, scale, scale);
}
void keyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
    case 'S':
            scale += 0.2;
            inicia_config();
            display();
            break;
    case 's':
            scale -= 0.2;
            inicia_config();
            display();
            break;
    }
}
void botao_mouse(int b, int state, int x, int y)
{
    switch (b) {
        case GLUT_LEFT_BUTTON:
            switch (state) {
                case GLUT_DOWN:
                    xb = x;
                    yb = y;
                    break;
                case GLUT_UP:
                    theta = theta + xm - xb;
                    phi = phi - ym + yb ;
                    break;
            }
            break;
    }
}
void mov_mouse(int x, int y)
{
    xm = x;
    ym = y;
    theta = theta + xm - xb;
    phi = phi - ym + yb ;
    inicia_config();
    theta = theta - xm + xb;
    phi = phi + ym - yb;
    display();
}
int main(int argc, char **argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(400,400);
    glutInitWindowPosition(50,50);
    glutCreateWindow("Cubo 3D");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(botao_mouse);
    glutMotionFunc(mov_mouse);
    inicia_config();
    glutMainLoop();
}
