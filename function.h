#ifndef FUNCTION_H
#define FUNCTION_H

#include <math.h>

/*数学函数库*/

class Function	//函数抽象类
{
public:
    virtual int operator()(int x) = 0;
};

class Function_x_plus_50:public Function    //f(x) = x + 50
{
public:
    int operator()(int x);
};

class Function_1p5_multi_x:public Function  //f(x) = 1.5x
{
public:
    int operator()(int x);
};

class Function_nega_x_plus_256:public Function  //f(x) = -x + 256
{
public:
    int operator()(int x);
};

class Function_x_square:public Function //f(x) = x^2
{
public:
    int operator()(int x);
};

class Function_256_multi_sin_lb_pi_multi_x_div_256_rb:public Function   //f(x) = 256 * sin(pi * x / 256)
{
public:
    virtual int operator()(int x);
};

#endif // FUNCTION_H
