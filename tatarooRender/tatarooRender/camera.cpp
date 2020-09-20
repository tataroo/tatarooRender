//
//  camera.cpp
//  tatarooRender
//
//  Created by 蒋睿 on 2020/9/17.
//  Copyright © 2020 蒋睿. All rights reserved.
//

#include "camera.hpp"
#include <assert.h>
#include "macro.h"
#include "mvp.hpp"
#include "core/maths.h"
#include "core/appDef.h"
camera* camera::instance = nullptr;

camera* camera::create(Vec3f pos, Vec3f target, float aspect){
    assert((pos - target).norm() > EPSILON && aspect > 0);
    if (instance) {
        delete instance;
    }
    instance = new camera(pos, target, aspect);
    return instance;
}
camera* camera::getInstance(){
    return instance;
}
void camera::setTransform(Vec3f pos, Vec3f target){
    assert((pos - target).norm() > EPSILON);
    this->position = pos;
    this->target = target;
}
static Vec3f calculate_pan(Vec3f from_camera, motion motion) {
    Vec3f forward = from_camera.normalize();
    Vec3f left = UP ^ forward;
    Vec3f up = forward ^ left;

    float distance = from_camera.norm();
    float factor = distance * (float)tan(FOVY / 2) * 2;
    Vec3f delta_x = left * motion.pan.x * factor;
    Vec3f delta_y = up * motion.pan.y * factor;
    return delta_x + delta_y;
}

static Vec3f calculate_offset(Vec3f from_target, motion motion) {
    float radius = from_target.norm();
    float theta = (float)atan2(from_target.x, from_target.z);  /* azimuth */
    float phi = (float)acos(from_target.y / radius);           /* polar */
    float factor = PI * 2;
    Vec3f offset;

    radius *= (float)pow(0.95, motion.dolly);
    theta -= motion.orbit.x * factor;
    phi -= motion.orbit.y * factor;
    phi = float_clamp(phi, EPSILON, PI - EPSILON);

    offset.x = radius * (float)sin(phi) * (float)sin(theta);
    offset.y = radius * (float)cos(phi);
    offset.z = radius * (float)sin(phi) * (float)cos(theta);
    return offset;
}


void camera::updateTransform(motion change){
    Vec3f from_target = position - target;
    Vec3f from_camera = target - position;
    Vec3f pan = calculate_pan(from_camera, change);
    Vec3f offset = calculate_offset(from_target, change);
    target = target + pan;
    position = target + offset;
}
Vec3f camera::getPos()
{
    return position;
}
Vec3f camera::getForward(){
    return (target - position).normalize();
}
Matrix camera::getViewMatrix(){
    
    return lookAt(position, UP, target);
}
Matrix camera::getProjMatirx(){
    return Perspective(FOVY, aspect, NEAR, FAR);
}

camera::camera(Vec3f pos, Vec3f target, float aspect)
{
    this->position = pos;
    this->target = target;
    this->aspect = aspect;
}
camera::~camera(){
    
}

