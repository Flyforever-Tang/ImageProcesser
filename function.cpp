#include "function.h"

int Function_x_plus_50::operator()(int x)
{
    return x + 50;
}

int Function_1p5_multi_x::operator()(int x)
{
    return x * 1.5 + 0.01;
}

int Function_nega_x_plus_256::operator()(int x)
{
    return -x + 256;
}

int Function_x_square::operator()(int x)
{
    return pow(x, 2);
}

int Function_256_multi_sin_lb_pi_multi_x_div_256_rb::operator()(int x)
{
    return 256 * sin(3.1415926535 * x / 256) + 0.01;
}
