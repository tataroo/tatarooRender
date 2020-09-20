//
//  frameBuffer.hpp
//  tatarooRender
//
//  Created by 蒋睿 on 2020/9/16.
//  Copyright © 2020 蒋睿. All rights reserved.
//

#ifndef frameBuffer_hpp
#define frameBuffer_hpp

#include <stdio.h>
#include "geometry.h"
typedef struct {
    int width, height;
    unsigned char *color_buffer;
    float *depth_buffer;
} framebuffer_t;

framebuffer_t *framebuffer_create(int width, int height);
void framebuffer_release(framebuffer_t *framebuffer);
void framebuffer_clear_color(framebuffer_t *framebuffer, Vec4f color);
void framebuffer_clear_depth(framebuffer_t *framebuffer);
#endif /* frameBuffer_hpp */

