//
//  program.cpp
//  tatarooRender
//
//  Created by 蒋睿 on 2020/9/10.
//  Copyright © 2020 蒋睿. All rights reserved.
//

#include "program.hpp"

program::program(shader* oShader, int width, int height){
    this->oShader = oShader;
    this->zBuffer = new float[width * height];
    for (int i=width*height; i--; zBuffer[i] = std::numeric_limits<float>::max());
    this->nWidth = width;
    this->nHeight = height;
}
program::~program(){
    oShader = nullptr;
    zBuffer = nullptr;
    delete oShader;
    delete zBuffer;
}

shader* program::getShader()
{
    return oShader;
}
bool program::discard(int x, int y, float z){
    int index = int(y * nWidth + x);
    bool isDicard = z > zBuffer[index];
    if (!isDicard) {
        zBuffer[index] = z;
    }
    return isDicard;
}
