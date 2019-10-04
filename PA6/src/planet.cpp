#include "planet.h"

Planet::Planet(BaseObject *parent_, float orbitRadius_, float orbitRate_, float rotateRate_)
    : BaseObject(parent_, std::string("../obj/sphere.obj")),
    orbitRadius(orbitRadius_),
    orbitRate(orbitRate_),
    rotateRate(rotateRate_){

    angleInOrbit = 0.0f;

    rotateDirection = 1.0;
    orbitDirection = 1.0;
}

void Planet::DerivedUpdate(float dt){
    eulerAngle.y += 2 * M_PI * rotateRate * rotateDirection * dt;
    angleInOrbit += 2 * M_PI * orbitRate * orbitDirection * dt;

    position = glm::vec3(orbitRadius * cos(angleInOrbit), 0.0f, orbitRadius * sin(angleInOrbit));
}

void Planet::MouseDown(SDL_Event event){

}

void Planet::MouseUp(SDL_Event event){

}

void Planet::KeyDown(SDL_Event event){
    switch(event.key.keysym.sym){
        case SDLK_a:
            rotateDirection = 1.0;
            break;
        case SDLK_d:
            rotateDirection = -1.0;
            break;
        case SDLK_w:
            orbitDirection = 0.0;
            break;
        case SDLK_s:
            rotateDirection = -0.0;
            break;
        case SDLK_q:
            orbitDirection = -1.0;
            break;
        case SDLK_e:
            orbitDirection = 1.0;
            break;
        default:
            break;
    }
}

void Planet::KeyUp(SDL_Event event){

}

void Planet::MouseWheel(SDL_Event event){

}
