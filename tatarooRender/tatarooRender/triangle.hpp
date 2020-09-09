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
//static Model* model = new Model();
void drawTriangle(Vec3f a, Vec3f b, Vec3f c, float* zBuffer, TGAImage& image, float intensity, Vec2f uv0, Vec2f uv1, Vec2f uv2, Model* model);
#endif /* triangle_hpp */
