#include "Iluminacao.hpp"
#include <GLUT/glut.h>

iluminacao::iluminacao() {
    mat_specular[0] = 1.0f;
    mat_specular[1] = 1.0f;
    mat_specular[2] = 1.0f;
    mat_specular[3] = 1.0f;
    
    mat_shininess = 50.0f;
    
    light_position[0] = 2.0f;
    light_position[1] = 2.0f;
    light_position[2] = 2.0f;
    light_position[3] = 1.0f;
    
    light_diffuse[0] = 1.0f;
    light_diffuse[1] = 1.0f;
    light_diffuse[2] = 0.0f;
    light_diffuse[3] = 1.0f;
}
void iluminacao::light_on() {
    glShadeModel (GL_SMOOTH); // GL_SMOOTH ou GL_FLAT
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
    glEnable(GL_NORMALIZE);
}
void iluminacao::render_light() {
    glColor3f(1,0,0);
    glPointSize(8);
    glBegin(GL_POINTS);
    glVertex3f(light_position[0],
        light_position[1],
        light_position[2]);
    glEnd();
}
void iluminacao::light_off() {
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glDisable(GL_DEPTH_TEST);
}
