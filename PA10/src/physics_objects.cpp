#include "physics_object.h"
#include <iostream>

PhysicsObject::PhysicsObject(std::string _name, BaseObject *parent_, std::string objectFile, struct PhysicsOptions physics)
    : BaseObject(_name, parent_, objectFile, false){
    collider = GetCollisionShape(physics);
    btDefaultMotionState *shapeMotionState = NULL;
    shapeMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(physics.position.x, physics.position.y, physics.position.z)));


    const float defaultMass = 1;
    btScalar mass(defaultMass);
    if(physics.physicsType != PhysicsType::Dynamic)
    {
        mass = 0;
    }

    btVector3 inertia(0, 0, 0);
    collider->calculateLocalInertia(mass, inertia);
    btRigidBody::btRigidBodyConstructionInfo shapeRigidBodyCI(mass, shapeMotionState, collider, inertia);
    rigidbody = new btRigidBody(shapeRigidBodyCI);

    if(physics.physicsType == PhysicsType::Kinematic)
    {
        rigidbody->setCollisionFlags(rigidbody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
    }
}

btCollisionShape * PhysicsObject::GetCollisionShape(struct PhysicsOptions physics)
{
    switch(physics.colliderType)
    {

        case ColliderType::None:
                return NULL;
                break;

        case ColliderType::Plane:
                if(physics.planeType == 0)
                {
                    return new btStaticPlaneShape(btVector3(0.0f, -1.0f, 0.0f), 1);
                }
                if(physics.planeType == 1)
                {
                    return new btStaticPlaneShape(btVector3(1.0f, 0.0f, 0.0f), 1);
                }
                if(physics.planeType == 2)
                {
                    return new btStaticPlaneShape(btVector3(-1.0f, 0.0f, 0.0f), 1);
                }
                if(physics.planeType == 3)
                {
                    return new btStaticPlaneShape(btVector3(0.0f, 0.0f, 1.0f), 1);
                }
                if(physics.planeType == 4)
                {
                    return new btStaticPlaneShape(btVector3(0.0f, 0.0f, -1.0f), 1);
                }
                break;

        case ColliderType::Sphere:
                return new btSphereShape(btScalar(.25f));
                break;

        case ColliderType::Cube:
                return new btBoxShape(btVector3(1.0f, 1.0f, 1.0f));
                break;

        case ColliderType::Cylinder:
                return new btCylinderShape(0.4f * btVector3(1.0f, 1.0f, 1.0f));
                break;

        case ColliderType::Mesh:
                return new btBvhTriangleMeshShape(objTriMesh, true);
                break;
    }
    return new btBoxShape(btVector3(1.0f, 1.0f, 1.0f));
}

void PhysicsObject::DerivedUpdate(float dt){
    /*
    std::cout << "Model mat" << std::endl;
    for(int i = 0; i < 4; ++i){
        for(int j = 0; j < 4; ++j){
            std::cout << model[i][j] << " ";
        }
        std::cout << std::endl;
    }
    */
}


void PhysicsObject::MouseDown(SDL_Event event){

}

void PhysicsObject::MouseUp(SDL_Event event){

}

void PhysicsObject::KeyDown(SDL_Event event){

}

void PhysicsObject::KeyUp(SDL_Event event){

}

void PhysicsObject::MouseWheel(SDL_Event event){

}

void PhysicsObject::MouseMotion(SDL_Event event){

}
