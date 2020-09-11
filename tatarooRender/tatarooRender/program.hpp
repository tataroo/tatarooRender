//
//  program.hpp
//  tatarooRender
//
//  Created by 蒋睿 on 2020/9/10.
//  Copyright © 2020 蒋睿. All rights reserved.
//

#ifndef program_hpp
#define program_hpp

#include <stdio.h>
#include "shader.hpp"
class program {
public:
    program(shader* oShader, int width, int height);
    ~program();
    shader* getShader();
    bool discard(int x, int y, float z);
public:
    void* uniforms;
    void* varyings[3];
    void* outVaryings[3];
private:
    shader* oShader;
    float* zBuffer;
    int nWidth;
    int nHeight;
};
#endif /* program_hpp */
