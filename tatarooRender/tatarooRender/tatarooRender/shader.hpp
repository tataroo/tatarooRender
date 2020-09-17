//
//  shader.hpp
//  tatarooRender
//
//  Created by 蒋睿 on 2020/9/10.
//  Copyright © 2020 蒋睿. All rights reserved.
//

#ifndef shader_hpp
#define shader_hpp
#include <stdio.h>
#include "geometry.h"
#include "tgaimage.h"
class shader {
public:
    shader();
    virtual ~shader();
    // 顶点着色器 
    virtual Vec4f vertex(void* attribs,void* varyings,void* uniforms) = 0;
    // 片段着色器 负责计算最终某点的颜色
    virtual Vec4f fragment(void *varyings, void *uniforms) = 0;
    virtual int getSizeOfVarrings() = 0;
};

typedef struct {
    Vec3f position;
    Vec2f texcoord; //纹理坐标
    Vec3f normal; //法线
} spAttribs;

typedef struct {
    Vec3f postion; 
    Vec2f texcoord; //纹理坐标
    Vec3f normal; //法线
//    float intensity; // 光照强度
} spVarrings;

typedef struct {
    Vec3f light_dir; //光照方向
    Vec3f eye; // 相机位置
    Matrix model;
    Matrix view;
    Matrix projection;
    TGAImage diffusemap;
} spUniforms;

// 简单的渲染器 只考虑简单的背面剔除
class spShader : public shader{
public:
    spShader();
    virtual ~spShader();
    virtual Vec4f vertex(void *attribs, void *varyings, void *uniforms);
    virtual Vec4f fragment(void *varyings, void *uniforms);
    virtual int getSizeOfVarrings();
};
#endif /* shader_hpp */
