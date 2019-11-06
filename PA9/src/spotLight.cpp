#include "spotLight.h"
#include <iostream>

SpotLight::SpotLight(std::string _name, BaseObject *parent_, glm::vec4 _lightPosition, float _intensity, glm::vec4 _direction, float _cutoffAngleDegrees)
    : PointLight(_name, parent_, _lightPosition, _intensity),
    direction(_direction),
    cutoffAngleDegrees(_cutoffAngleDegrees){

}

void SpotLight::DerivedUpdate(float dt){

}


void SpotLight::MouseDown(SDL_Event event){

}

void SpotLight::MouseUp(SDL_Event event){

}

void SpotLight::KeyDown(SDL_Event event){

}

void SpotLight::KeyUp(SDL_Event event){

}

void SpotLight::MouseWheel(SDL_Event event){

}

void SpotLight::MouseMotion(SDL_Event event){

}
