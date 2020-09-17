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
#include "core/appDef.h"
#include "camera.hpp"
using namespace  std;
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const int width = 800;
const int height = 800;
static int zBuffer[width][height];
Vec3f       eye(0,0,3);
Vec3f    center(0,0,0);
Vec3f        up(0,1,0);
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
        Vec3f light_dir = camera::getInstance()->getForward().normalize(); // 光照方向
        spUniforms* uniforms = new spUniforms();
        Matrix model = modelScale(1, 1, 1);
        Matrix view = camera::getInstance()->getViewMatrix();
        Matrix projection = camera::getInstance()->getProjMatirx();
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
}

static Vec2f get_pos_delta(Vec2f old_pos, Vec2f new_pos) {
    Vec2f delta = new_pos - old_pos;
    return delta / (float)WINDOW_HEIGHT;
}

static Vec2f get_cursor_pos(window_t *window) {
    float xpos, ypos;
    input_query_cursor(window, &xpos, &ypos);
    return Vec2f(xpos, ypos);
}
static void button_callback(window_t *window, button_t button, int pressed) {
    record_t *record = (record_t*)window_get_userdata(window);
    Vec2f cursor_pos = get_cursor_pos(window);
    if (button == BUTTON_L) {
        float curr_time = platform_get_time();
        if (pressed) {
            record->is_orbiting = 1;
            record->orbit_pos = cursor_pos;
            record->press_time = curr_time;
            record->press_pos = cursor_pos;
        } else {
            float prev_time = record->release_time;
            Vec2f pos_delta = get_pos_delta(record->orbit_pos, cursor_pos);
            record->is_orbiting = 0;
            record->orbit_delta = record->orbit_delta + pos_delta;
            if (prev_time && curr_time - prev_time < CLICK_DELAY) {
                record->double_click = 1;
                record->release_time = 0;
            } else {
                record->release_time = curr_time;
                record->release_pos = cursor_pos;
            }
        }
    } else if (button == BUTTON_R) {
        if (pressed) {
            record->is_panning = 1;
            record->pan_pos = cursor_pos;
        } else {
            Vec2f pos_delta = get_pos_delta(record->pan_pos, cursor_pos);
            record->is_panning = 0;
            record->pan_delta = record->pan_delta + pos_delta;
        }
    }
}

static void scroll_callback(window_t *window, float offset) {
    record_t *record = (record_t*)window_get_userdata(window);
    record->dolly_delta += offset;
}

static void update_camera(window_t *window, camera *camera,
                          record_t *record) {
    Vec2f cursor_pos = get_cursor_pos(window);
    if (record->is_orbiting) {
        Vec2f pos_delta = get_pos_delta(record->orbit_pos, cursor_pos);
        record->orbit_delta = record->orbit_delta + pos_delta;
        record->orbit_pos = cursor_pos;
    }
    if (record->is_panning) {
        Vec2f pos_delta = get_pos_delta(record->pan_pos, cursor_pos);
        record->pan_delta = record->pan_delta + pos_delta;
        record->pan_pos = cursor_pos;
    }
    if (input_key_pressed(window, KEY_SPACE)) {
        camera->setTransform(CAMERA_POSITION, CAMERA_TARGET);
    } else {
        motion motion;
        motion.orbit = record->orbit_delta;
        motion.pan = record->pan_delta;
        motion.dolly = record->dolly_delta;
        camera->updateTransform(motion);
    }
}
void mainLoop(tickFunc func, void* userData){
    float aspect = width / height;
    window_t* window;
    framebuffer_t* frameBuffer = nullptr;
    context* pContext = new context();
    record_t record;
    memset(&record, 0, sizeof(record_t));
    callbacks_t callbacks;
    memset(&callbacks, 0, sizeof(callbacks_t));
    callbacks.button_callback = button_callback;
    callbacks.scroll_callback = scroll_callback;
    camera* camera = camera::create(CAMERA_POSITION, CAMERA_TARGET, aspect);
    frameBuffer = framebuffer_create(width, height);
    pContext->frameBuffer = frameBuffer;
    platform_initialize();
    window = window_create("test", width, height);
    window_set_userdata(window, &record);
    input_set_callbacks(window, callbacks);
    while (!window_should_close(window)) {
         update_camera(window, camera, &record);
         func(pContext, userData);
         window_draw_buffer(window, frameBuffer);
         framebuffer_clear_color(frameBuffer, Vec4f(0, 0, 0, 1));
         input_poll_events();
         record.orbit_delta = Vec2f(0, 0);
         record.pan_delta = Vec2f(0, 0);
         record.dolly_delta = 0;
    }
    delete pContext;
    delete frameBuffer;
    window_destroy(window);
}
int main(int argc, char** argv)
{

    Model::addModel("/Users/jiangrui/myProject/tatarooRender/tatarooRender/obj/african_head.obj");
//    Model::addModel("/Users/jiangrui/myProject/tatarooRender/tatarooRender/obj/body.obj");
    mainLoop(drawSpModel, nullptr);
    return 0;
}

