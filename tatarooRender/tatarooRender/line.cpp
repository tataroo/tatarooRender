//
//  line.cpp
//  tatarooRender
//
//  Created by 蒋睿 on 2020/8/9.
//  Copyright © 2020 蒋睿. All rights reserved.
//

#include "line.hpp"
#include <math.h>
using namespace  std;
void line(int x0, int y0, int x1,int y1, TGAImage &image, TGAColor color)
{
    bool steep = false;
    if (abs(x0 - x1) < abs(y0 - y1))
    {
        swap(x0, y0);
        swap(x1, y1);
        steep = true;
    }
    if (x0 > x1)
    {
        swap(x0, x1);
    }
    int dx = x1 - x0;
    int dy = y1- y0;
    float sf = dy / (float)dx;
    int derror = abs(dy) * 2;
    int error = 0;
    int y = y0;
    for (float x = x0; x <= x1; ++x) {
//        float t = (x - x0) / (float)(x1 - x0);
//        int y = t * (y1 - y0) + y0;
        if (steep)
        {
            image.set(y, x, color);
        }
        else
        {
            image.set(x, y, color);
        }
        error += derror;
        if (error > dx) {
            y += (y1 > y0 ? 1 : -1);
            error -= dx * 2;
        }
    }
}
