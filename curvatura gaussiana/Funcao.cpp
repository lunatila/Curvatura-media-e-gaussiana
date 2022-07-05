#include "Funcao.hpp"
#include <GLUT/glut.h>
#undef GAUSS
#define MEDIA

void Funcao::normal(float *v,float x,float y) {
    v[0] = -dfx(x,y); // -cos(x*x)*2*x;
    v[1] = -dfy(x,y); // 2y
    v[2] = 1;
}
void Funcao::hessiana(float *h, float x, float y){
    //h[0] = sin(x*x-3*y*y)*2*x*2*x - cos(x*x-3*y*y)*2;
    h[0] = sin(x*x)*2*x - cos(x*x)*2;
    //h[1] = -sin(x*x-3*y*y)*2*x*6*y;
    h[1] = 0;
    h[2] = 0;
    //h[3] = -sin(x*x-3*y*y)*2*x*6*y;
    h[3] = 0;
    //h[4] = -sin(x*x-3*y*y)*6*y*6*y + cos(x*x-3*y*y)*6;
    h[4] = 2;
    h[5] = 0;
    h[6] = 0; // | fxx0 fxy1 fxz2 |
    h[7] = 0; // | fyx3 fyy4 fyz5 |
    h[8] = 0; // | fzx6 fzy7 fzz8 |
}
void Funcao::hessiana_estrela(float *hess_star, float *hess){
    hess_star[0] = hess[4] * hess[8] - hess[5] * hess[7]; // ok
    hess_star[1] = hess[5] * hess[6] - hess[3] * hess[8]; // ok
    hess_star[2] = hess[3] * hess[7] - hess[4] * hess[6]; // ok
    hess_star[3] = hess[2] * hess[7] - hess[1] * hess[8]; // ok
    hess_star[4] = hess[0] * hess[8] - hess[2] * hess[6]; // ok
    hess_star[5] = hess[1] * hess[6] - hess[0] * hess[7]; // ok
    hess_star[6] = hess[1] * hess[5] - hess[2] * hess[4]; // ok
    hess_star[7] = hess[3] * hess[2] - hess[0] * hess[5]; // ok
    hess_star[8] = hess[0] * hess[4] - hess[1] * hess[3]; // ok
}
float Funcao::curvatura_gaussiana(float *grad, float *hess, float *hess_star)
{
    float modulo_grad = sqrt(grad[0] * grad[0] + grad[1] * grad[1] + grad[2] * grad[2]);

    return ((grad[0] * hess_star[0] + grad[1] * hess_star[3] + grad[2] * hess_star[6]) * grad[0] +
                (grad[0] * hess_star[1] + grad[1] * hess_star[4] + grad[2] * hess_star[7]) * grad[1] +
                (grad[0] * hess_star[2] + grad[1] * hess_star[5] + grad[2] * hess_star[8]) * grad[2]) /
                (modulo_grad * modulo_grad * modulo_grad * modulo_grad);
}
float Funcao::curvatura_media(float *grad, float *hess)
{
    float modulo_grad = sqrt(grad[0] * grad[0] + grad[1] * grad[1] + grad[2] * grad[2]);
    float traceH = hess[0] + hess[4] + hess[8];
    float GxHxGt = (grad[0] * hess[0] + grad[1] * hess[3] + grad[2] * hess[6]) * grad[0] +
                (grad[0] * hess[1] + grad[1] * hess[4] + grad[2] * hess[7]) * grad[1] +
                (grad[0] * hess[2] + grad[1] * hess[5] + grad[2] * hess[8]) * grad[2];

    return (GxHxGt - modulo_grad * modulo_grad * traceH) / (2 * modulo_grad * modulo_grad * modulo_grad);
}
void Funcao::visualiza_ms() {
    float dx = (xmax - xmin) / points;
    float dy = (ymax - ymin) / points;
    float x = xmin;
    float y;

    for (int i = 0; i < points; i++) {
        y = ymin;
        for (int j = 0; j < points; j++) {
            glColor3f(0, 1, 1);
            marching_squares(x, y, dx, dy, 0);
            glColor3f(0, 1, 0);
            marching_squares(x, y, dx, dy, -0.2);
            glColor3f(0, 0, 1);
            marching_squares(x, y, dx, dy, 0.2);
            /*glColor3f(0, 0, 1);
            glBegin(GL_LINE_LOOP);
            glVertex3f(x, y, f(x,y));
            glVertex3f(x + dx, y, f(x+dx,y));
            glVertex3f(x + dx, y + dy, f(x+dx,y+dy));
            glVertex3f(x, y + dy,f(x,y+dy));
            glEnd();*/
            y += dy;
        }
        x += dx;
    }
}

void Funcao::marching_squares(float x, float y, float dx, float dy, float niv) {
    float s[4];
    float v[3];
    float h[9];
    float hstar[9];
    
#ifdef MEDIA
    normal(v,x,y);
    hessiana(h, x, y);
    hessiana_estrela(hstar, h);
    s[0] = curvatura_media(v, h) + niv;

    normal(v,x + dx,y);
    hessiana(h, x + dx, y);
    hessiana_estrela(hstar, h);
    s[1] = curvatura_media(v, h) + niv;
    
    normal(v,x+dx,y+dy);
    hessiana(h, x+dx, y+dy);
    hessiana_estrela(hstar, h);
    s[2] = curvatura_media(v, h) + niv;
    
    normal(v,x,y+dy);
    hessiana(h, x, y);
    hessiana_estrela(hstar, h);
    s[3] = curvatura_media(v, h) + niv;
#endif
    
#ifdef GAUSS
    normal(v,x,y);
    hessiana(h, x, y);
    hessiana_estrela(hstar, h);
    s[0] = curvatura_gaussiana(v, h, hstar) + niv;

    normal(v,x + dx,y);
    hessiana(h, x + dx, y);
    hessiana_estrela(hstar, h);
    s[1] = curvatura_gaussiana(v, h, hstar) + niv;
    
    normal(v,x+dx,y+dy);
    hessiana(h, x+dx, y+dy);
    hessiana_estrela(hstar, h);
    s[2] = curvatura_gaussiana(v, h, hstar) + niv;
    
    normal(v,x,y+dy);
    hessiana(h, x, y);
    hessiana_estrela(hstar, h);
    s[3] = curvatura_gaussiana(v, h, hstar) + niv;
#endif
    
    /*s[0]= normal(v,x,y);
    s[1]= f(x+dx,y)+0.5;
    s[2]= f(x+dx,y+dy)+0.5;
    s[3]= f(x,y+dy)+0.5;*/

    int ind = (s[0] < 0) ? 1 : 0; //indice dos vertices
    ind += (s[1] < 0) ? 2 : 0;
    ind += (s[2] < 0) ? 4 : 0;
    ind += (s[3] < 0) ? 8 : 0;

    int e1 = 0, e2 = 0; //indice das arestas

    switch (ind) {
    case 1:
    case 14:
        e1 = 0;
        e2 = 3;
        break;
    case 2:
    case 13:
        e1 = 0;
        e2 = 1;
        break;
    case 4:
    case 11:
        e1 = 1;
        e2 = 2;
        break;
    case 8:
    case 7:
        e1 = 2;
        e2 = 3;
        break;
    case 3:
    case 12:
        e1 = 1;
        e2 = 3;
        break;
    case 6:
    case 9:
        e1 = 0;
        e2 = 2;
        break;
    case 5:
    case 10:
        break;
    }

    if (ind > 0 && ind < 15) {
        
            float xc[4], yc[4];

            xc[0] = x; xc[1] = x + dx; xc[2] = x + dx; xc[3] = x;
            yc[0] = y; yc[1] = y; yc[2] = y + dy; yc[3] = y + dy;

        if (ind != 5 && ind != 10) {

            float px[2], py[2];

            float t = -s[e1] / (s[e1 + 1] - s[e1]);
            px[0] = xc[e1] + t * (xc[e1 + 1] - xc[e1]);
            py[0] = yc[e1] + t * (yc[e1 + 1] - yc[e1]);

            t = -s[e2] / (s[(e2 + 1) % 4] - s[e2]);
            px[1] = xc[e2] + t * (xc[(e2 + 1) % 4] - xc[e2]);
            py[1] = yc[e2] + t * (yc[(e2 + 1) % 4] - yc[e2]);

            glBegin(GL_LINES);
            glVertex3f(px[0], py[0], f(px[0], py[0]));
            glVertex3f(px[1], py[1], f(px[1], py[1]));
            glEnd();
        }
        else {
            float pxx[4], pyy[4];

            float t = -s[0] / (s[1] - s[0]);
            pxx[0] = xc[0] + t * (xc[1] - xc[0]);
            pyy[0] = yc[0] + t * (yc[1] - yc[0]);

            t = -s[1] / (s[2] - s[1]);
            pxx[1] = xc[1] + t * (xc[2] - xc[1]);
            pyy[1] = yc[1] + t * (yc[2] - yc[1]);

            t = -s[2] / (s[2 + 1] - s[2]);
            pxx[2] = xc[2] + t * (xc[3] - xc[2]);
            pyy[2] = yc[2] + t * (yc[3] - yc[2]);

            t = -s[3] / (s[0] - s[3]);
            pxx[3] = xc[3] + t * (xc[0] - xc[3]);
            pyy[3] = yc[3] + t * (yc[0] - yc[3]);

            if ((s[0] * s[2] - s[1] * s[3]) > 0) {
                glBegin(GL_LINES);
                glVertex3f(pxx[0], pyy[0], f(pxx[0], pyy[0]));
                glVertex3f(pxx[1], pyy[1], f(pxx[1], pyy[1]));
                glEnd();
                glBegin(GL_LINES);
                glVertex3f(pxx[2], pyy[2], f(pxx[2], pyy[2]));
                glVertex3f(pxx[3], pyy[3], f(pxx[3], pyy[3]));
                glEnd();
            }
            else {
                glBegin(GL_LINES);
                glVertex3f(pxx[0], pyy[0], f(pxx[0], pyy[0]));
                glVertex3f(pxx[3], pyy[3], f(pxx[3], pyy[3]));
                glEnd();
                glBegin(GL_LINES);
                glVertex3f(pxx[2], pyy[2], f(pxx[2], pyy[2]));
                glVertex3f(pxx[1], pyy[1], f(pxx[1], pyy[1]));
                glEnd();
            }
        }
    }
}
void Funcao::plota_funcao_01()
{
    float x,y;
    float dx = (xmax-xmin)/points;
    float dy = (ymax-ymin)/points;
    
    glColor3f (1.0f, 1.0f, 1.0f);
    x = xmin;
    for(int i=0;i<points;i++) {
        y = ymin;
        for(int j=0;j<points;j++){
            glBegin(GL_QUADS);
            glVertex3f(x ,y ,f(x ,y) );
            glVertex3f(x+dx,y ,f(x+dx,y) );
            glVertex3f(x+dx,y+dy,f(x+dx,y+dy));
            glVertex3f(x ,y+dy,f(x ,y+dy));
            glEnd();
            y += dy;
        }
        x += dx;
    }
}
void Funcao::plota_funcao_02()
{
    float x,y,v[3],h[9],hstar[9];
    float cm[4], cg[4];
    float dx = (xmax-xmin)/points;
    float dy = (ymax-ymin)/points;
    
    x = xmin;
    for(int i=0;i<points;i++) {
        y = ymin;
        for(int j=0;j<points;j++){
#ifdef MEDIA
            glBegin(GL_QUADS);
            normal(v,x,y);
            hessiana(h, x, y);
            hessiana_estrela(hstar, h);
            cm[0] = curvatura_media(v, h);
                glColor3f(-cm[0], 0, 0);
                glNormal3f(v[0],v[1],v[2]);
                glVertex3f(x ,y ,f(x ,y));
            normal(v,x+dx,y);
            hessiana(h, x+dx, y);
            hessiana_estrela(hstar, h);
            cm[1] = curvatura_media(v, h);
                glColor3f(-cm[1], 0, 0);
                glNormal3f(v[0],v[1],v[2]);
                glVertex3f(x+dx,y ,f(x+dx,y) );
            normal(v,x+dx,y+dy);
            hessiana(h, x+dx, y+dy);
            hessiana_estrela(hstar, h);
            cm[2] = curvatura_media(v, h);
                glColor3f(-cm[2], 0, 0);
                glNormal3f(v[0],v[1],v[2]);
                glVertex3f(x+dx,y+dy,f(x+dx,y+dy));
                normal(v,x,y+dy);
            hessiana(h, x, y+dy);
            hessiana_estrela(hstar, h);
            cm[3] = curvatura_media(v, h);
                glColor3f(-cm[3], 0, 0);
                glNormal3f(v[0],v[1],v[2]);
                glVertex3f(x ,y+dy,f(x ,y+dy));
            glEnd();
#endif
#ifdef GAUSS
            glBegin(GL_QUADS);
            normal(v,x,y);
            hessiana(h, x, y);
            hessiana_estrela(hstar, h);
            cg[0] = curvatura_gaussiana(v, h, hstar);
                glColor3f(-(cg[0]-1)/2, (cg[0]+1)/2, 0);
                glNormal3f(v[0],v[1],v[2]);
                glVertex3f(x ,y ,f(x ,y) );
            normal(v,x+dx,y);
            hessiana(h, x+dx, y);
            hessiana_estrela(hstar, h);
            cg[1] = curvatura_gaussiana(v, h, hstar);
                glColor3f(-(cg[1]-1)/2, (cg[1]+1)/2, 0);
                glNormal3f(v[0],v[1],v[2]);
                glVertex3f(x+dx,y ,f(x+dx,y) );
            normal(v,x+dx,y+dy);
            hessiana(h, x+dx, y+dy);
            hessiana_estrela(hstar, h);
            cg[2] = curvatura_gaussiana(v, h, hstar);
                glColor3f(-(cg[2]-1)/2, (cg[2]+1)/2, 0);
                glNormal3f(v[0],v[1],v[2]);
                glVertex3f(x+dx,y+dy,f(x+dx,y+dy));
                normal(v,x,y+dy);
            hessiana(h, x, y+dy);
            hessiana_estrela(hstar, h);
            cg[3] = curvatura_gaussiana(v, h, hstar);
                glColor3f(-(cg[3]-1)/2, (cg[3]+1)/2, 0);
                glNormal3f(v[0],v[1],v[2]);
                glVertex3f(x ,y+dy,f(x ,y+dy));
            glEnd();
#endif
            y += dy;
        }
        x += dx;
    }
}
void Funcao::plota_normal() {
     float x,y,v[3];
     float dx = (xmax-xmin)/points;
     float dy = (ymax-ymin)/points;
     float n = 10;

     glColor3f (1.0f, 1.0f, 1.0f);
     x = xmin;
     for(int i=0;i<points;i++) {
        y = ymin;
        for(int j=0;j<points;j++){
             glBegin(GL_LINES);
                 normal(v,x,y);
                 glVertex3f(x ,y ,f(x ,y) );
                 glVertex3f(x+v[0]/n,y+v[1]/n,f(x ,y)+v[2]/n);

                 normal(v,x+dx,y);
                 glVertex3f(x+dx,y ,f(x+dx,y) );
                 glVertex3f(x+dx+v[0]/n,y+v[1]/n,f(x+dx,y)+v[2]/n);

                 normal(v,x+dx,y+dy);
                 glVertex3f(x+dx,y+dy,f(x+dx,y+dy));
                 glVertex3f(x+dx+v[0]/n,y+dy+v[1]/n,f(x+dx,y+dy)+v[2]/n);

                 normal(v,x,y+dy);
                 glVertex3f(x ,y+dy,f(x ,y+dy));
                 glVertex3f(x+v[0]/n,y+dy+v[1]/n,f(x,y+dy)+v[2]/n);
             glEnd();
             y += dy;
        }
     x += dx;
     }
}
