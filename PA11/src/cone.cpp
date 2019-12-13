#include "cone.h"

Cone::Cone(std::string _name, BaseObject *parent_, std::string objectPath, struct PhysicsOptions physics)
: PhysicsObject(_name, parent_, objectPath, physics)
{
    isDead = false;
    timeSinceDead = 0.0f;
}

void Cone::DerivedUpdate(float dt)
{
    if(isDead){
        timeSinceDead += dt;
        if(timeSinceDead > 5.0f){
            btTransform transform;
            transform.setIdentity();

            transform.setOrigin(btVector3(-10000.0f, -10000.0f, -10000.0f));
            transform.setRotation(btQuaternion(0, 0, 0, 1));
            rigidbody->setWorldTransform(transform);
            rigidbody->getMotionState()->setWorldTransform(transform);

            rigidbody->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
            rigidbody->setAngularVelocity(btVector3(0.0f, 0.0f, 0.0f));
            rigidbody->clearForces();
        }
    }
}
