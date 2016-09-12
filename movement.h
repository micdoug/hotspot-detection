#pragma once

#include <iostream>

namespace hd
{
    struct Movement 
    {
        long time;
        int node;
        int x;
        int y;
    };
}

std::istream &operator>>(std::istream &in, hd::Movement &mov);