#include "flipper.h"

Flipper::Flipper(std::string _name, BaseObject *parent_, std::string objectPath, struct PhysicsOptions physics, bool _left, SDL_Keycode triggerKey)
: PhysicsObject(_name, parent_, objectPath, physics),
  left(_left),
  pressThis(triggerKey),
  threshold(40.0f),
  speed(15.0f)
{}

void Flipper::DerivedUpdate(float dt)
{
    if(rigidbody != NULL && !rigidbody->isStaticObject())
    {
        rigidbody->activate(true);

        float x, y, z;
        btTransform tran;
        rigidbody->getMotionState()->getWorldTransform(tran);
        tran.getRotation().getEulerZYX(z, y, x);

        float thres = glm::radians(threshold);

        // don't
        if(left)
        {
            if(y < -thres)
            {
                rigidbody->setAngularVelocity(btVector3(0, 0, 0));
            }

            if(y > -thres)
            {
                rigidbody->setAngularVelocity(btVector3(0, -speed / 2, 0));
            }

            if(pressed && y > thres)
            {
                rigidbody->setAngularVelocity(btVector3(0, 0, 0));
            }

            if(pressed && y < thres)
            {
                rigidbody->setAngularVelocity(btVector3(0, speed, 0));
            }
        } else
        {
            if(y > thres)
            {
                rigidbody->setAngularVelocity(btVector3(0, 0, 0));
            }

            if(y < thres)
            {
                rigidbody->setAngularVelocity(btVector3(0, speed / 2, 0));
            }

            if(pressed && y < -thres)
            {
                rigidbody->setAngularVelocity(btVector3(0, 0, 0));
            }

            if(pressed && y > -thres)
            {
                rigidbody->setAngularVelocity(btVector3(0, -speed, 0));
            }
        }


    } else
    {
        std::cerr << "Object \"" << name << "\" does not have a rigidbody (Movable::DerivedUpdate)" << std::endl;
        exit(1);
    }
}

void Flipper::KeyDown(SDL_Event event)
{
    if(event.key.keysym.sym == pressThis)
    {
        pressed = true;
    }
}

void Flipper::KeyUp(SDL_Event event)
{
    if(event.key.keysym.sym == pressThis)
    {
        pressed = false;
    }
}