//
//  mvp.hpp
//  tatarooRender
//
//  Created by 蒋睿 on 2020/8/27.
//  Copyright © 2020 蒋睿. All rights reserved.
//

#ifndef mvp_hpp
#define mvp_hpp

#include <stdio.h>
#include "geometry.h"

mat<4, 1, float> v2m(Vec3f v);

Vec3f m2v(mat<4, 1, float> v);
Matrix modelScale(float x, float y, float z);
Matrix modelMove(float x, float y, float z);
Matrix modelRotate(Vec3f center, float angleX, float angleY, float angleZ);
Matrix lookAt(Vec3f location, Vec3f up, Vec3f  target);
Matrix viewport(int x, int y, int w, int h);
Matrix Perspective(float fov, float aspect, float near, float far);
void divs(mat<4, 1, float>& v, float z);
#endif /* mvp_hpp */
