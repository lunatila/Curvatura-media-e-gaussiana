#include <stdio.h>
#include <cmath>

class Funcao
{
    float xmin, xmax, ymin, ymax;
    int points;
public:
    Funcao(float xm = -2, float xM = 2, float ym = -2, float yM = 2, int p = 50)
    {
         xmin = xm;
         ymin = ym;
         xmax = xM;
         ymax = yM;
         points = p;
    }

    float f(float x,float y)
    {
        //return sin(x*x-3*y*y);
        return sin(x*x) - y*y;
    }
    
    float dfx(float x, float y)
    {
        //return cos(x*x-3*y*y)*2*x;
        return cos(x*x)*2*x;
    }
    
    float dfy(float x, float y)
    {
        //return -cos(x*x-3*y*y)*6*y;
        return -2*y;
    }
    
    void normal(float *v, float x, float y);
    void hessiana(float *v, float x, float y);
    void hessiana_estrela(float *hess_star, float *hess);
    float curvatura_gaussiana(float *grad, float *hess, float *hess_star);
    float curvatura_media(float *grad, float *hess);
    
    void plota_funcao_01();
    void plota_funcao_02();
    void marching_squares(float x, float y, float dx, float dy, float niv);
    void visualiza_ms();
    void plota_normal();
};
