//
//  frameBuffer.cpp
//  tatarooRender
//
//  Created by 蒋睿 on 2020/9/16.
//  Copyright © 2020 蒋睿. All rights reserved.
//

#include "frameBuffer.hpp"
#include "core/maths.h"
framebuffer_t *framebuffer_create(int width, int height) {
    int color_buffer_size = width * height * 4;
    int depth_buffer_size = sizeof(float) * width * height;
    Vec4f default_color(0, 0, 0, 1);
    float default_depth = 1;
    framebuffer_t *framebuffer;

    assert(width > 0 && height > 0);

    framebuffer = (framebuffer_t*)malloc(sizeof(framebuffer_t));
    framebuffer->width = width;
    framebuffer->height = height;
    framebuffer->color_buffer = (unsigned char*)malloc(color_buffer_size);
    framebuffer->depth_buffer = (float*)malloc(depth_buffer_size);

    framebuffer_clear_color(framebuffer, default_color);
    framebuffer_clear_depth(framebuffer, default_depth);

    return framebuffer;
}

void framebuffer_release(framebuffer_t *framebuffer) {
    free(framebuffer->color_buffer);
    free(framebuffer->depth_buffer);
    free(framebuffer);
}

void framebuffer_clear_color(framebuffer_t *framebuffer, Vec4f color) {
    int num_pixels = framebuffer->width * framebuffer->height;
    int i;
    for (i = 0; i < num_pixels; i++) {
        framebuffer->color_buffer[i * 4 + 0] = float_to_uchar(color[0]);
        framebuffer->color_buffer[i * 4 + 1] = float_to_uchar(color[1]);
        framebuffer->color_buffer[i * 4 + 2] = float_to_uchar(color[2]);
        framebuffer->color_buffer[i * 4 + 3] = float_to_uchar(color[3]);
    }
}

void framebuffer_clear_depth(framebuffer_t *framebuffer, float depth) {
    int num_pixels = framebuffer->width * framebuffer->height;
    int i;
    for (i = 0; i < num_pixels; i++) {
        framebuffer->depth_buffer[i] = depth;
    }
}


