//
//  appDef.h
//  tatarooRender
//
//  Created by 蒋睿 on 2020/9/17.
//  Copyright © 2020 蒋睿. All rights reserved.
//

#ifndef appDef_h
#define appDef_h
#include "geometry.h"
#include "macro.h"
typedef struct {
    /* orbit */
    int is_orbiting;
    Vec2f orbit_pos;
    Vec2f orbit_delta;
    /* pan */
    int is_panning;
    Vec2f pan_pos;
    Vec2f pan_delta;
    /* zoom */
    float dolly_delta;
    /* light */
    float light_theta;
    float light_phi;
    /* click */
    float press_time;
    float release_time;
    Vec2f press_pos;
    Vec2f release_pos;
    int single_click;
    int double_click;
    Vec2f click_pos;
} record_t;
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;

const Vec3f CAMERA_POSITION = {0, 0, 3};
const Vec3f CAMERA_TARGET = {0, 0, 0};

const float LIGHT_THETA = TO_RADIANS(45);
const float LIGHT_PHI = TO_RADIANS(45);
const float LIGHT_SPEED = PI;
const float CLICK_DELAY = 0.25f;
const float NEAR = 0.1f;
const float FAR = 10000;
const float FOVY = TO_RADIANS(60);
const Vec3f UP = {0, 1, 0};
#endif /* appDef_h */
