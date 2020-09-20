//
//  shader.cpp
//  tatarooRender
//
//  Created by 蒋睿 on 2020/9/10.
//  Copyright © 2020 蒋睿. All rights reserved.
//

#include "shader.hpp"

shader::shader(){
    
}
shader::~shader(){
    
}

spShader::spShader(){
    
}
spShader::~spShader(){
    
}

Vec4f spShader::vertex(void *attribs, void *varyings, void *uniforms){
    spAttribs* spAttirb = static_cast<spAttribs*>(attribs);
    spUniforms* spUn = static_cast<spUniforms*>(uniforms);
    spVarrings* spVarr = static_cast<spVarrings*>(varyings);
    spVarr->normal = spAttirb->normal;
    spVarr->texcoord = spAttirb->texcoord;
    spVarr->postion = spAttirb->position;
    mat<4, 1, float> homo = v2m(spAttirb->position);
    mat<4, 1, float> clipOord = spUn->projection * spUn->view * spUn->model * homo;
    return clipOord.col(0);
}

Vec4f spShader::fragment(void *varyings, void *uniforms){
    spVarrings* varying = static_cast<spVarrings*>(varyings);
    spUniforms* uniform = static_cast<spUniforms*>(uniforms);
    Vec2f uvf = varying->texcoord;
    Vec2i uv(uvf[0]*uniform->diffusemap.get_width(), uvf[1]*uniform->diffusemap.get_height());
    TGAColor color = uniform->diffusemap.get(uv[0], uv[1]);
    float specular = uniform->specularmap.get(uv[0], uv[1])[0] / 1.0f;
    Vec4f rst((float)color[0], (float)color[1], (float)color[2],  1);
//    Vec4f speColor((float)specular[0], (float)specular[1], (float)specular[2],  1);
    Vec3f lightPos(0, 1, 3);
    lightPos = uniform->eye; //testflag 为了旋转需要
    Vec3f nor = varying->normal.normalize();
    Vec3f light = (lightPos - varying->postion).normalize();
    Vec3f view = (uniform->eye - varying->postion).normalize();
    Vec3f half = (light + view).normalize();
    Vec4f envir(0, 0, 0, 0);
    float strenth = 0.6 * pow(max(nor * half, 0.f), specular);
    float intensity = max(0.f, light * varying->normal.normalize()) ;
    for (int i = 0; i < 3; ++i) {
        rst[i] = envir[i] + rst[i] * (intensity + strenth);
    }
    return rst;
}
int spShader::getSizeOfVarrings(){
    return sizeof(spVarrings);
}
