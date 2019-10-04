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
