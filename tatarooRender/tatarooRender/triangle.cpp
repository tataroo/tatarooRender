//
//  triangle.cpp
//  tatarooRender
//
//  Created by 蒋睿 on 2020/8/18.
//  Copyright © 2020 蒋睿. All rights reserved.
//

#include "triangle.hpp"
#include "core/maths.h"
using namespace  std;
Vec3i cross(Vec3i a, Vec3i b)
{
    return Vec3i(a.y * b.z - a.z * b.y, a.x * b.z - a.z * b.x, a.x * b.y - a.y * b.x);
}

//点乘
int dot(Vec3i a, Vec3i b)
{
    return  a.x * b.x + a.y * b.y + a.z * b.z;
}

// 判断三角形的c,p是否在ab同边
bool sameSide(Vec3i a, Vec3i b, Vec3i c, Vec3i p)
{
    Vec3i ab = b - a;
    Vec3i ac = c - a;
    Vec3i ap = p - a;
    Vec3i rs1 = cross(ab, ac);
    Vec3i rs2 = cross(ab, ap);
    return  dot(rs1, rs2) >= 0;
}
// 同边方式
bool isPointInTriangle(Vec3i a, Vec3i b, Vec3i c, Vec3i p)
{
    return sameSide(a, b, c, p) && sameSide(b, c, a, p) && sameSide(c, a, b, p);
}
// 重心坐标
bool isPointInTriangleOpti(Vec3f a,  Vec3f b, Vec3f c, Vec3f p)
{
    Vec3f v0 = c-a;
    Vec3f v1 = b-a;
    Vec3f v2 = p-a;
    
    // 计算点乘
    int dot00 = v0 * v0;
    int dot01 = v0 * v1;
    int dot02 = v0 * v2;
    int dot11 = v1 * v1;
    int dot12 = v1 * v2;

    // 计算重心坐标系下的坐标
    float invDenom = 1/(float)(dot00 * dot11 - dot01 * dot01);
    float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    float v = (dot00 * dot12 - dot01 * dot02) * invDenom;
    
    return u>=0 && v>=0 && (u+v)<=1;
}
// 重心坐标
Vec3f barycentric11(Vec3f a,  Vec3f b, Vec3f c, Vec3f p)
{
    Vec3f v0 = b-a;
    Vec3f v1 = c-a;
    Vec3f v2 = p-a;
    
    // 计算点乘
    int dot00 = v0 * v0;
    int dot01 = v0 * v1;
    int dot02 = v0 * v2;
    int dot11 = v1 * v1;
    int dot12 = v1 * v2;

    // 计算重心坐标系下的坐标
    float invDenom = 1/(float)(dot00 * dot11 - dot01 * dot01);
    float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    float v = (dot00 * dot12 - dot01 * dot02) * invDenom;
    
    return Vec3f(1 - u - v, u, v);
}
Vec3f barycentric(Vec3f A, Vec3f B, Vec3f C, Vec3f P) {
    Vec3f s[2];
    for (int i=1; i >= 0; --i) {
        s[i].x = C[i]-A[i];
        s[i].y = B[i]-A[i];
        s[i].z = A[i]-P[i];
    }
    Vec3f u = s[0] ^ s[1];
    if (std::abs(u[2])>1e-2)
    {
        return Vec3f(1.f-(u.x+u.y)/u.z, u.y/u.z, u.x/u.z);
    }
    return Vec3f(-1,1,1);
}

void test()
{
    Vec3f a(3, 0,0.46);
    Vec3f b(0, 0,0.48);
    Vec3f c(0,3,0.47);
    Vec3f p(1.5, 1, 0);
    Vec3f centersd = barycentric(a, b, c, p);
    int k = 0;
}

void drawTriangle(Vec3f a, Vec3f b, Vec3f c, float* zBuffer, TGAImage& image, float intensity, Vec2f uv0, Vec2f uv1, Vec2f uv2, Model* model)
{
    int width = image.get_width();
    int height = image.get_height();
    float minX = max(0.f, min(c.x, min(a.x, b.x)));
    float maxX = min((float)width - 1, max(c.x, max(a.x, b.x)));
    float minY = max(0.f, min(c.y, min(a.y, b.y)));
    float maxY = min((float)height - 1,max(c.y, max(a.y, b.y)));
    
    for (int i = minX; i <= maxX; ++i) {
        for (int j = minY; j <= maxY; ++j) {
            // 通过插值计算z
            Vec3f p(i, j, 0);
            Vec3f ratio = barycentric(a, b, c, p);
            if (ratio.x < 0 || ratio.y < 0 || ratio.z < 0) {
                continue;
            }
            p.z += a.z * ratio[0] + b.z * ratio[1] + c.z * ratio[2];
            
            // p.z > zBuffer[j * width + i]
            int index = int(j * width + i);
            if (p.z > zBuffer[index]){
                Vec2f textureUv = uv0 * ratio[0] + uv1 * ratio[1] + uv2 * ratio[2];
                TGAColor color = model->diffuse(textureUv) * intensity;
                image.set(i, j, color);
                zBuffer[index] = p.z;
            }
        }
    }
}

void drawTriangle(Vec3f* screenCoord, framebuffer_t* pFrameBuffer, float intensity, program* oProgram, TGAImage& image){
    Vec3f a = screenCoord[0];
    Vec3f b = screenCoord[1];
    Vec3f c = screenCoord[2];
    int width = oProgram->nWidth;
    int height = oProgram->nHeight;
    float minX = max(0.f, min(c.x, min(a.x, b.x)));
    float maxX = min((float)width - 1, max(c.x, max(a.x, b.x)));
    float minY = max(0.f, min(c.y, min(a.y, b.y)));
    float maxY = min((float)height - 1,max(c.y, max(a.y, b.y)));
    void* varying = nullptr;
    varying = malloc(oProgram->getShader()->getSizeOfVarrings());
    memset(varying, 0, oProgram->getShader()->getSizeOfVarrings());
    for (int i = minX; i <= maxX; ++i) {
        for (int j = minY; j <= maxY; ++j) {
            // 通过插值计算z
            Vec3f p(i, j, 0);
            Vec3f ratio = barycentric(a, b, c, p);
            if (ratio.x < 0 || ratio.y < 0 || ratio.z < 0) {
                continue;
            }
            p.z += a.z * ratio[0] + b.z * ratio[1] + c.z * ratio[2];
            if (!oProgram->discard(i, j, p.z)){
                interpolate_varyings(oProgram->varyings, varying, oProgram->getShader()->getSizeOfVarrings(), ratio);
                Vec4f color = oProgram->getShader()->fragment(varying, oProgram->uniforms) * intensity;
                int index = j * width + i;
                pFrameBuffer->color_buffer[index * 4 + 0] = float_to_uchar(color.z);
                pFrameBuffer->color_buffer[index * 4 + 1] = float_to_uchar(color.y);
                pFrameBuffer->color_buffer[index * 4 + 2] = float_to_uchar(color.x);
                pFrameBuffer->color_buffer[index * 4 + 3] = float_to_uchar(color.w);
            }
        }
    }
    free(varying);
}
void interpolate_varyings(void *src_varyings[3], void *dst_varyings, int sizeof_varyings, Vec3f weights){
    int num_floats = sizeof_varyings / sizeof(float);
    float *src0 = (float*)src_varyings[0];
    float *src1 = (float*)src_varyings[1];
    float *src2 = (float*)src_varyings[2];
    float *dst = (float*)dst_varyings;
    float weight0 = weights.x;
    float weight1 = weights.y;
    float weight2 = weights.z;
    float normalizer = 1 / (weight0 + weight1 + weight2);
    int i;
    for (i = 0; i < num_floats; i++) {
        float sum = src0[i] * weight0 + src1[i] * weight1 + src2[i] * weight2;
        dst[i] = sum * normalizer;
    }
}
