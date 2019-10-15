#include "planet.h"

const float planetOffset = 0.25;

Planet::Planet(std::string _name,
               BaseObject *_parent,
               Planet * sun,
               Planet * previousPlanet,
               float _orbitRadius,
               bool _orbitScaled,
               float _orbitRate, 
               float _rotateRate,
               bool _hasRings,
               glm::vec3 _scale)
    : BaseObject(_name, _parent, _hasRings ? std::string("../obj/sphererings.obj") : std::string("../obj/sphere.obj"), true),
    orbitRadius(_orbitRadius),
    orbitRate(_orbitRate),
    rotateRate(_rotateRate){

    angleInOrbit = 0.0f;

    rotateDirection = 1.0;
    orbitDirection = 1.0;
    scale = _scale;
    hasRings = _hasRings;

    scaledRadius = 0;
    
    if(sun != NULL)
    {
        scaledRadius = (orbitRadius + sun->GetRadius()) * (1 + planetOffset);
    }
    
    normalizedRadius = 0;
    if(sun != NULL)
    {
        if(previousPlanet != NULL)
        {
            normalizedRadius =  (GetRadius() + previousPlanet->GetRadius()) * (1 + planetOffset) + previousPlanet->normalizedRadius;
        } else
        {
            normalizedRadius = (GetRadius() + sun->GetRadius()) * (1 + planetOffset);
        }
    }

    radiusParam = 0;
    orbitParamVel = 0;

    orbitScaled = _orbitScaled;
}

void Planet::DerivedUpdate(float dt){

    if(orbitScaled)
    {
        radiusParam += orbitParamVel * dt;
        if(radiusParam > 1)
        {
            radiusParam = 1;
        } else if(radiusParam < 0)
        {
            radiusParam = 0;
        }

        orbitRadius = (1 - radiusParam) * normalizedRadius + radiusParam * scaledRadius;
    }

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

float Planet::GetRadius()
{
    return getScale().x * (hasRings ? 2 : 1);
}

Planet * PlanetInfo::FromInfo(BaseObject * parent, Planet * sun, Planet * previousPlanet)
{
    return new Planet(
        name,
        parent,
        sun,
        previousPlanet,
        (double) distFromSun / distMod,
        true,
        1 / (double) orbitPeriod * simSpeed,
        1 / (double) rotationalPeriod * simSpeed,
        hasRings,
        glm::vec3(1.0, 1.0, 1.0) * (float) ((double) diameter / scaleMod)
    );
}
