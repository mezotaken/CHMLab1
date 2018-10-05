#include "model.h"

//Реализация функций структуры model0

double model0::count(double x,double v)
{
    return -5*v/2;
}


void model0::set(double i_x0,double i_u0,
             double i_h, double i_end, double i_epsBrd,
             double i_epsCtrl, int i_maxStep, bool i_isStepFixed)
{
    x0 = i_x0;
    u0 = i_u0;
    h = i_h;
    end = i_end;
    epsBrd = i_epsBrd;
    epsCtrl = i_epsCtrl;
    maxStep = i_maxStep;
    isStepFixed = i_isStepFixed;
}

void model0::start()
{
    x = x0;
    v = u0;
    isRun = true;
    hprev = h;
    maxS = 0;
    xmaxS = x0;
    minS =  10;
    xminS = x0;
    maxH = 0;
    xmaxH = x0;
    minH = 10;
    xminH = x0;
    maxE = 0;
    xmaxE = x0;
    minE = 10;
    xminE = x0;
}

void model0::stop()
{
    isRun = false;
    stDwn = stUp = 0;
}

double model0::countNext(double th, double tx, double tv)
{
    double k1,k2,k3,k4;
    double res;
    k1 = count(tx,tv);
    k2 = count(tx+th/2,tv+th/2*k1);
    k3 = count(tx+th/2,tv+th/2*k2);
    k4 = count(tx+th,tv+th*k3);
    res = tv+th/6*(k1+2*k2+2*k3+k4);
    return res;
}

double model0::halfStepCountNext(double th, double tx, double tv)
{
    return countNext(th/2,tx+th/2,countNext(th/2,tx,tv));
}

int model0::ctrlLocalErr()
{
    int res;
    if(std::abs(s)<epsCtrl/32)
        res = -1;
    else if(std::abs(s)<=epsCtrl)
        res = 0;
    else res = 1;
        return res;
}

void model0::halveStep()
{
    h/=2; stDwn++;
}

void model0::doubleStep()
{
    h*=2; stUp++;
}

double model0::countCorrect(double tx)
{

    double c = u0*exp(5*x0/2);
    return c*exp(-5*tx/2);

}

void model0::iterate()
{
    if(x+h>end)
        h = end-x;
    v2 = halfStepCountNext(h,x,v);
    v1 = countNext(h,x,v);

    s = (v2-v1)/15;
    if(!isStepFixed)
    {
        int ctrl = ctrlLocalErr();

        while(ctrl == 1)
        {

            halveStep();
            v2 = halfStepCountNext(h,x,v);
            v1 = countNext(h,x,v);
            s = (v2-v1)/15;
            ctrl = ctrlLocalErr();
        }
         hprev = h;

        if(ctrl == -1)
            doubleStep();

    }

    x = x+hprev;
    v = v1;
    u = countCorrect(x);
    E = u-v;
    if(hprev>maxH)
    {
        maxH = hprev;
        xmaxH = x;
    }
    if(hprev<minH)
    {
        minH = hprev;
        xminH = x;
    }
    if(std::abs(s)>maxS)
    {
        maxS = std::abs(s);
        xmaxS = x;
    }
    if(std::abs(s)<minS)
    {
        minS = std::abs(s);
        xminS = x;
    }
    if(std::abs(E)>maxE)
    {
        maxE = std::abs(E);
        xmaxE = x;
    }
    if(std::abs(E)<minE)
    {
        minE = std::abs(E);
        xminE = x;
    }
}

//model1

double model1::count(double x,double v)
{
    return log(x+1)/(x*x+1)*v*v+v-v*v*v*sin(10*x);
}


void model1::set(double i_x0,double i_u0,
             double i_h, double i_end, double i_epsBrd,
             double i_epsCtrl, int i_maxStep, bool i_isStepFixed)
{
    x0 = i_x0;
    u0 = i_u0;
    h = i_h;
    end = i_end;
    epsBrd = i_epsBrd;
    epsCtrl = i_epsCtrl;
    maxStep = i_maxStep;
    isStepFixed = i_isStepFixed;
}

void model1::start()
{
    x = x0;
    v = u0;
    isRun = true;
    hprev = h;
    maxS = 0;
    xmaxS = x0;
    minS =  10;
    xminS = x0;
    maxH = 0;
    xmaxH = x0;
    minH = 10;
    xminH = x0;
}

void model1::stop()
{
    isRun = false;
    stDwn = stUp = 0;
}

double model1::countNext(double th, double tx, double tv)
{
    double k1,k2,k3,k4;
    double res;
    k1 = count(tx,tv);
    k2 = count(tx+th/2,tv+th/2*k1);
    k3 = count(tx+th/2,tv+th/2*k2);
    k4 = count(tx+th,tv+th*k3);
    res = tv+th/6*(k1+2*k2+2*k3+k4);
    return res;
}

double model1::halfStepCountNext(double th, double tx, double tv)
{
    return countNext(th/2,tx+th/2,countNext(th/2,tx,tv));
}

int model1::ctrlLocalErr()
{
    int res;
    if(std::abs(s)<epsCtrl/32)
        res = -1;
    else if(std::abs(s)<=epsCtrl)
        res = 0;
    else res = 1;
        return res;
}

void model1::halveStep()
{
    h/=2; stDwn++;
}

void model1::doubleStep()
{
    h*=2; stUp++;
}

void model1::iterate()
{
    if(x+h>end)
        h = end-x;
    v2 = halfStepCountNext(h,x,v);
    v1 = countNext(h,x,v);

    s = (v2-v1)/15;
    if(!isStepFixed)
    {
        int ctrl = ctrlLocalErr();

        while(ctrl == 1)
        {

            halveStep();
            v2 = halfStepCountNext(h,x,v);
            v1 = countNext(h,x,v);
            s = (v2-v1)/15;
            ctrl = ctrlLocalErr();
        }
         hprev = h;

        if(ctrl == -1)
            doubleStep();

    }

    x = x+hprev;
    v = v1;

    if(hprev>maxH)
    {
        maxH = hprev;
        xmaxH = x;
    }
    if(hprev<minH)
    {
        minH = hprev;
        xminH = x;
    }
    if(std::abs(s)>maxS)
    {
        maxS = std::abs(s);
        xmaxS = x;
    }
    if(std::abs(s)<minS)
    {
        minS = std::abs(s);
        xminS = x;
    }
}

