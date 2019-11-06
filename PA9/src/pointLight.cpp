#include "pointLight.h"
#include <iostream>

PointLight::PointLight(std::string _name, BaseObject *parent_, glm::vec4 _lightPosition, float _intensity)
    : BaseObject(_name, parent_, std::string(""), false),
    lightPosition(_lightPosition),
    intensity(_intensity){

}

void PointLight::DerivedUpdate(float dt){
    //lightPosition.x += 2*dt;
}


void PointLight::MouseDown(SDL_Event event){

}

void PointLight::MouseUp(SDL_Event event){

}

void PointLight::KeyDown(SDL_Event event){

}

void PointLight::KeyUp(SDL_Event event){

}

void PointLight::MouseWheel(SDL_Event event){

}

void PointLight::MouseMotion(SDL_Event event){

}
