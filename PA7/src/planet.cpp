#include "planet.h"

Planet::Planet(std::string _name, BaseObject *_parent, float _orbitRadius, float _orbitRate, float _rotateRate, bool hasRings, glm::vec3 _scale)
    : BaseObject(_name, _parent, hasRings ? std::string("../obj/sphererings.obj") : std::string("../obj/sphere.obj"), true),
    orbitRadius(_orbitRadius),
    orbitRate(_orbitRate),
    rotateRate(_rotateRate){

    angleInOrbit = 0.0f;

    rotateDirection = 1.0;
    orbitDirection = 1.0;
    scale = _scale;
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
            //rotateDirection = 1.0;
            break;
        case SDLK_d:
            //rotateDirection = -1.0;
            break;
        case SDLK_w:
            //orbitDirection = 0.0;
            break;
        case SDLK_s:
            //rotateDirection = -0.0;
            break;
        case SDLK_q:
            //orbitDirection = -1.0;
            break;
        case SDLK_e:
            //orbitDirection = 1.0;
            break;
        default:
            break;
    }
}

void Planet::KeyUp(SDL_Event event){

}

void Planet::MouseWheel(SDL_Event event){

}

const int simSpeed = 1e2;

Planet * PlanetInfo::FromInfo(BaseObject * parent)
{
    return new Planet(
        name,
        parent,
        (double) distFromSun / 8e9,
        1 / (double) orbitPeriod * simSpeed,
        1 / (double) rotationalPeriod * simSpeed,
        hasRings,
        glm::vec3(1.0, 1.0, 1.0) * (float) ((double) diameter / 1e7)
    );
}
