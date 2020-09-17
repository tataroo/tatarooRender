//
//  triangle.hpp
//  tatarooRender
//
//  Created by 蒋睿 on 2020/8/18.
//  Copyright © 2020 蒋睿. All rights reserved.
//

#ifndef triangle_hpp
#define triangle_hpp

#include <stdio.h>
#include "geometry.h"
#include "tgaimage.h"
#include "model.h"
#include "program.hpp"
void drawTriangle(Vec3f a, Vec3f b, Vec3f c, float* zBuffer, TGAImage& image, float intensity, Vec2f uv0, Vec2f uv1, Vec2f uv2, Model* model);
void drawTriangle(Vec3f* screenCoord,framebuffer_t* pFrameBuffer, float intensity, program* oProgram, TGAImage& image);
// 对uv坐标，法线进行插值
void interpolate_varyings(void *src_varyings[3], void *dst_varyings, int sizeof_varyings, Vec3f weights);
#endif /* triangle_hpp */
