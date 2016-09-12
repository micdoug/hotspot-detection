#include <movement.h>

std::istream &operator>>(std::istream &in, hd::Movement &mov) 
{
    in >> mov.time;
    in >> mov.node;
    in >> mov.x;
    in >> mov.y;
 
    return in;
}