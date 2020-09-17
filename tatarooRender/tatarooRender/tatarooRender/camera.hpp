//
//  camera.hpp
//  tatarooRender
//
//  Created by 蒋睿 on 2020/9/17.
//  Copyright © 2020 蒋睿. All rights reserved.
//

#ifndef camera_hpp
#define camera_hpp

#include <stdio.h>
#include "geometry.h"
typedef struct {
    Vec2f pan;
    Vec2f orbit;
    float dolly;
}motion;
class camera {
public:
    static camera* create(Vec3f pos, Vec3f target, float aspect);
    static camera* getInstance();
    void setTransform(Vec3f pos, Vec3f target);
    void updateTransform(motion change);
    Vec3f getPos();
    Vec3f getForward();
    Matrix getViewMatrix();
    Matrix getProjMatirx();
private:
    camera(Vec3f pos, Vec3f target, float aspect);
    ~camera();
    static camera* instance;
    Vec3f position;
    Vec3f target;
    float aspect;
};
#endif /* camera_hpp */
