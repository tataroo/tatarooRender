//
//  main.cpp
//  tatarooRender
//
//  Created by 蒋睿 on 2020/8/7.
//  Copyright © 2020 蒋睿. All rights reserved.
//

#include <iostream>
#include "tgaimage.h"
#include "line.hpp"
#include "model.h"
#include "triangle.hpp"
#include "mvp.hpp"
#include "program.hpp"
#include "shader.hpp"
#include "core/platform.h"
using namespace  std;
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const int width = 800;
const int height = 800;
static int zBuffer[width][height];
Vec3f       eye(0,0,3);
Vec3f    center(0,0,0);
Vec3f        up(0,1,0);
void drawLine()
{
    TGAImage image(width, height, TGAImage::RGB);
       Model* blackman = new Model("/Users/jiangrui/myProject/tatarooRender/tatarooRender/obj/african_head.obj");
       for (int i = 0; i < blackman->nfaces(); ++i) {
           std::vector<int> face = blackman->face(i);
           for (int j = 0; j < 3; ++j) {
               Vec3f v0 = blackman->vert(face[j]);
               Vec3f v1 = blackman->vert(face[(j + 1) % 3]);
               int x0 = (v0.x + 1) * width / 2;
               int x1 = (v1.x + 1) * width / 2;
               int y0 = (v0.y + 1) * height / 2;
               int y1 = (v1.y + 1) * height / 2;
               line(x0, y0, x1, y1, image, white);
           }
           
       }
       image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
       image.write_tga_file("/Users/jiangrui/workspace/fsa/line.tga");
}
Matrix getMa(){
    Matrix a = Matrix::identity();
    a[0][0] = -0.001;
    a[0][2] = -0.00000000087;
    a[0][3] = -0.376905;
    a[1][1] = -0.001;
    a[1][3] = -0.169495;
    a[2][0] = -0.00000000087;
    a[2][2] = -0.001;
    a[2][3] = -0.0000000329;
    return a;
}
void test1()
{
    TGAImage image(width, height, TGAImage::RGB);
        Model* blackman = new Model("/Users/jiangrui/myProject/tatarooRender/tatarooRender/obj/african_head.obj");
        Matrix projection = Perspective(45, 1, 1, 1000);
}
void drawTria()
{
    TGAImage image(width, height, TGAImage::RGB);
//    Model* blackman = new Model("/Users/jiangrui/myProject/tatarooRender/tatarooRender/obj/body.obj");
    Model* blackman = new Model("/Users/jiangrui/myProject/tatarooRender/tatarooRender/obj/african_head.obj");
    // 光照方向
    Vec3f light_dir(0,0,-1);
    Matrix root = getMa();
    Matrix model = lookAt(eye, up, center);
    Matrix viewPort = viewport(0, 0, width, height);
    Matrix projection = Perspective(60, 1, 0.1, 10000);
    projection.log();
    float *zbuffer = new float[width*height];
    for (int i=width*height; i--; zbuffer[i] = -std::numeric_limits<float>::max());
    for (int i = 0; i < blackman->nfaces(); ++i) {
        std::vector<int> face = blackman->face(i);
        Vec3f triangle[3];
        Vec3f worldCoord[3];
        for (int j = 0; j < 3; ++j) {
            Vec3f coords = blackman->vert(face[j]);
            worldCoord[j] = coords;
            // 取-z，越靠近屏幕z越小
            triangle[j] = Vec3f(coords.x, coords.y , -coords.z);
            mat<4, 1, float> m =  v2m(triangle[j]);
            // 有时需要将模型归一化
            //            m = root * m;
            m.log();
            Matrix vp =  projection * model;
            mat<4, 1, float> k = vp * m;
            k.log();
            triangle[j] = m2v(k);
            cout << triangle[j] << endl;
            triangle[j] = Vec3f((triangle[j].x + 1) * width / 2, (triangle[j].y + 1) * height / 2, (triangle[j].z + 1) * 0.5);
            cout << triangle[j] << endl;
        }
        // 法线
        Vec3f n = (worldCoord[2] - worldCoord[0]) ^ (worldCoord[1] - worldCoord[0]);
        n.normalize();
        float intensity = n * light_dir;
        if (intensity > 0){
            Vec2f uv[3];
            for (int k = 0; k < 3; ++k) {
                uv[k] = blackman->uv(i, k);
            }
            TGAColor color(intensity * 255, intensity * 255,intensity * 255, 255);
            drawTriangle(triangle[0], triangle[1], triangle[2], zbuffer, image, intensity, uv[0], uv[1], uv[2], blackman);
        }
    }
    image.write_tga_file("/Users/jiangrui/workspace/fsa/trianglenojj.tga");
}

void drawSpModel(context* pContext, void* userData)
{
    TGAImage image(width, height, TGAImage::RGB);
    map<const char*, Model*> oMap = Model::getModels();
    for (auto iter = oMap.begin(); iter != oMap.end(); ++iter) {
        Model* pModel = iter->second;
        // 设置program 和 shader的uniform参数
        spShader* pShader = new spShader();
        program* oProgram = new program(pShader, width, height);
        // 设置uniform
        Vec3f light_dir(0,0,-1); // 光照方向
        spUniforms* uniforms = new spUniforms();
        Matrix model = modelScale(1, 1, -1);
        Matrix view = lookAt(eye, up, center);
        Matrix projection = Perspective(60, 1, 0.1, 10000);
        uniforms->model = model;
        uniforms->view = view;
        uniforms->projection = projection;
        uniforms->light_dir = light_dir;
        uniforms->diffusemap = pModel->diffusemap_;
        oProgram->uniforms = uniforms;
        for (int i = 0; i < pModel->nfaces(); ++i) {
            std::vector<int> face = pModel->face(i);
            Vec3f triangle[3];
            Vec3f worldCoord[3];
            for (int j = 0; j < 3; ++j) {
                // 设置attribs
                spAttribs* attribs = new spAttribs();
                oProgram->varyings[j] = new spVarrings();
                Vec3f coords = pModel->vert(face[j]);
                Vec2f uv = pModel->uv(i, j);
                attribs->position = coords;
                attribs->texcoord = uv;
                attribs->normal = pModel->normal(i, j);
                Vec4f stdPosition = oProgram->getShader()->vertex(attribs, oProgram->varyings[j], oProgram->uniforms);
                worldCoord[j] = coords;
                // 透视除法和视口变换
                triangle[j] = m2v(stdPosition);
                triangle[j] = Vec3f((triangle[j].x + 1) * width / 2, (triangle[j].y + 1) * height / 2, (triangle[j].z + 1) * 0.5);
            }
            //背面剔除
            Vec3f n = (worldCoord[2] - worldCoord[0]) ^ (worldCoord[1] - worldCoord[0]);// 法线
            n.normalize();
            float intensity = n * light_dir;
            if (intensity > 0){
                drawTriangle(triangle, pContext->frameBuffer, intensity, oProgram, image);
            }
        }
        delete oProgram;
    }
//   image.write_tga_file("/Users/jiangrui/workspace/fsa/safsa.tga");
}
void mainLoop(tickFunc func, void* userData){
    window_t* window;
    framebuffer_t* frameBuffer = nullptr;
    context* pContext = new context();
    frameBuffer = framebuffer_create(width, height);
    pContext->frameBuffer = frameBuffer;
    platform_initialize();
    window = window_create("test", width, height);
    while (!window_should_close(window)) {
         func(pContext, userData);
         window_draw_buffer(window, frameBuffer);
         framebuffer_clear_color(frameBuffer, Vec4f(0, 0, 0, 1));
         input_poll_events();     }
    delete pContext;
    delete frameBuffer;
    window_destroy(window);
}
int main(int argc, char** argv)
{
    Model::addModel("/Users/jiangrui/myProject/tatarooRender/tatarooRender/obj/african_head.obj");
    mainLoop(drawSpModel, nullptr);
    return 0;
}

