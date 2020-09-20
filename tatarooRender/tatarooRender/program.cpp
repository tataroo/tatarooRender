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
    this->nWidth = width;
    this->nHeight = height;
}
program::~program(){
    oShader = nullptr;
    delete oShader;
}

shader* program::getShader()
{
    return oShader;
}
