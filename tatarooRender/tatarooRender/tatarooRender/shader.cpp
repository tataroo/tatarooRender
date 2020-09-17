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
    Vec4f rst((float)color[0], (float)color[1], (float)color[2],  1);
    return rst;
}
int spShader::getSizeOfVarrings(){
    return sizeof(spVarrings);
}
