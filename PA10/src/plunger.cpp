#include "plunger.h"

Plunger::Plunger(std::string _name, BaseObject *parent_, std::string objectPath, struct PhysicsOptions physics, SDL_Keycode triggerKey)
: PhysicsObject(_name, parent_, objectPath, physics),
  pressThis(triggerKey),
  threshold(1.5f),
  speed(15.0f)
{
    btTransform tran;
    rigidbody->getMotionState()->getWorldTransform(tran);
    startPos = tran.getOrigin();
}

void Plunger::DerivedUpdate(float dt)
{
    if(rigidbody != NULL && !rigidbody->isStaticObject())
    {
        rigidbody->activate(true);

        btTransform tran;
        rigidbody->getMotionState()->getWorldTransform(tran);
        btVector3 pos = tran.getOrigin();

        if(!rising)
        {
            if(pos.x() < startPos.x())
            {
                rigidbody->setLinearVelocity(btVector3(speed, 0, 0));
            } else 
            {
                rigidbody->setLinearVelocity(btVector3(0, 0, 0));
                if(pressed)
                {
                    rising = true;
                }
            }
        } else
        {
            rigidbody->setLinearVelocity(btVector3(-speed, 0, 0));
            if(pos.x() - startPos.x() < -threshold)
            {
                rising = false;
            }
        }


    } else
    {
        std::cerr << "Object \"" << name << "\" does not have a rigidbody (Movable::DerivedUpdate)" << std::endl;
        exit(1);
    }
}

void Plunger::KeyDown(SDL_Event event)
{
    if(event.key.keysym.sym == pressThis)
    {
        pressed = true;
    }
}

void Plunger::KeyUp(SDL_Event event)
{
    if(event.key.keysym.sym == pressThis)
    {
        pressed = false;
    }
}