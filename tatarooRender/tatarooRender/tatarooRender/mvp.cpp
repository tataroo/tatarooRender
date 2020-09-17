//
//  mvp.cpp
//  tatarooRender
//
//  Created by 蒋睿 on 2020/8/27.
//  Copyright © 2020 蒋睿. All rights reserved.
//

#include "mvp.hpp"
#define PI 3.1415927
Matrix modelScale(float x, float y, float z){
    Matrix model = Matrix::identity();
    model[0][0] = x;
    model[1][1] = y;
    model[2][2] = z;
    model[3][3] = 1;
    return model;
}

Matrix modelMove(float x, float y, float z){
    Matrix model = Matrix::identity();
    model[0][3] = x;
    model[1][3] = y;
    model[2][3] = z;
    model.log();
    return model;
}

Matrix modelRotate(Vec3f center, float angleX, float angleY, float angleZ)
{
    float radius = PI / 180;
    float sinX = sin(angleX * radius);
    float cosX = cos(angleX * radius);
    float sinY = sin(angleY * radius);
    float cosY = cos(angleY * radius);
    float sinZ = sin(angleZ * radius);
    float cosZ = cos(angleZ * radius);
    Matrix model = Matrix::identity();
    model[0][0] = cosY * cosZ;
    model[1][0] = -cosY * sinZ;
    model[2][0] = sinY;
    model[0][1] = sinX * sinY * cosZ + cosX * sinZ;
    model[1][1] = -sinX * sinY * sinZ + cosX * cosZ;
    model[2][1] = -sinX * cosY;
    model[0][2] = -cosX * sinY * cosZ + sinX * sinZ;
    model[1][2] = cosX * sinY * sinZ + sinX * cosZ;
    model[2][2] = cosX * cosY;
    model.log();
//    Matrix move = modelMove(center.x, center.y, center.z);
//    Matrix move1 = modelMove(-center.x, -center.y, -center.z);
//    Matrix rst = move * model * move1;
//    rst.log();
    return model;
}

void divs(mat<4, 1, float>& v, float z){
    v[0] = v[0] / z;
    v[1] = v[1] / z;
    v[2] = v[2] / z;
    v[3] = v[3] / z;
}
Matrix lookAt(Vec3f location, Vec3f up, Vec3f target){
    Matrix rst = Matrix::identity();
    Vec3f z = (location - target).normalize();
    Vec3f x = (up ^ z).normalize();
    Vec3f y = (z ^ x).normalize();
    for (int i = 0; i < 3; ++i) {
        rst[0][i] = x[i];
        rst[1][i] = y[i];
        rst[2][i] = z[i];
        
    }
    rst[0][3] = -(x * location);
    rst[1][3] = -(y * location);
    rst[2][3] = -(z * location);
    return rst;
}
const int depth = 255;
Matrix viewport(int x, int y, int w, int h) {
    Matrix m = Matrix::identity();
    m[0][3] = x+w/2.f;
    m[1][3] = y+h/2.f;
    m[2][3] = depth/2.f;

    m[0][0] = w/2.f;
    m[1][1] = h/2.f;
    m[2][2] = depth/2.f;
    return m;
}

Matrix Perspective(float fov, float aspect, float near, float far) {
    Matrix mat =  Matrix::identity();
    float theta = fov;
    float divisor = tan(theta / 2);
    float factor = 1.0f / divisor;
    float f_n = 1.0f / (far - near);
    mat[1][1]= 1.0f / divisor;
    mat[0][0] = 1.0f / (aspect * divisor);
    mat[2][2] = -(far + near) * f_n;
    mat[2][3] = -2.0f * far * near * f_n;
    mat[3][2] = -1.0f;
    mat[3][3] = 0.0f;
    return mat;
}
