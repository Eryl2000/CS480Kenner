#include "planet.h"

Planet::Planet(Object *parent_, float orbitRadius_, float orbitRate_, float rotateRate_)
    : Object(parent_),
    orbitRadius(orbitRadius_),
    orbitRate(orbitRate_),
    rotateRate(rotateRate_){

    angleInOrbit = 0.0f;

    rotateDirection = 1.0;
    orbitDirection = 1.0;
}

void Planet::DerivedUpdate(float dt){
    //Unrotate by the parent's rotation
    if(parent != NULL){
        model = glm::rotate(model, -parent->angle, glm::vec3(0.0, 1.0, 0.0));
    }

    //Calculate the angle of the object
    angle += dt * 2 * M_PI * rotateRate * rotateDirection;

    //Calculate the angle the object has gone so far in orbit
    angleInOrbit += dt * 2 * M_PI * orbitRate * orbitDirection;

    //Update the object's position and rotation
    glm::vec3 translation(orbitRadius * cos(angleInOrbit), 0.0f, orbitRadius * sin(angleInOrbit));
    model = glm::translate(model, translation);
    model = glm::rotate(model, angle, glm::vec3(0.0, 1.0, 0.0));
}
