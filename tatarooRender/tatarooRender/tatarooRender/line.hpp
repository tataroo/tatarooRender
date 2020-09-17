//
//  line.hpp
//  tatarooRender
//
//  Created by 蒋睿 on 2020/8/9.
//  Copyright © 2020 蒋睿. All rights reserved.
//

#ifndef line_hpp
#define line_hpp

#include <stdio.h>
#include "tgaimage.h"
#include "geometry.h"
class point{
public:
    float x;
    float y;
    
    point(float x1, float y1){
        x = x1;
        y = y1;
    }
};
static float getK(point a, point b){
    return (b.y - a.y) / (b.x - a.x);
}
static void swapPoint(point &a, point &b){
    point c(a.x, a.y);
    a.x = b.x;
    a.y = b.y;
    b.x = c.x;
    b.y = c.y;
}
void line(int x0, int y0, int x1,int y1, TGAImage &image, TGAColor color);
#endif /* line_hpp */
