#include "cube.h"

Cube::Cube(std::string _name, BaseObject *parent_, std::string objectFile)
    : BaseObject(_name, parent_, objectFile, false){
    collider = new btBoxShape(btVector3(1.0f, 1.0f, 1.0f));
    btDefaultMotionState *shapeMotionState = NULL;
    shapeMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 0), btVector3(0, 0, 0)));
    btScalar mass(1);
    btVector3 inertia(0, 0, 0);
    collider->calculateLocalInertia(mass, inertia);
    btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCI(mass, shapeMotionState, collider, inertia);
    //rigidbody = new btRigidBody(shapeRigidBodyCI);
}

void Cube::DerivedUpdate(float dt){

}


void Cube::MouseDown(SDL_Event event){

}

void Cube::MouseUp(SDL_Event event){

}

void Cube::KeyDown(SDL_Event event){

}

void Cube::KeyUp(SDL_Event event){

}

void Cube::MouseWheel(SDL_Event event){

}

void Cube::MouseMotion(SDL_Event event){

}
