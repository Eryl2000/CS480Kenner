#include "movable.h"

Movable::Movable(std::string _name, BaseObject *parent_, std::string objectPath, struct PhysicsOptions physics)
    : PhysicsObject(_name, parent_, objectPath, physics),
      force(500),
      forceVect(btVector3(0, 0, 0))
{}

void Movable::DerivedUpdate(float dt)
{
    if(rigidbody != NULL && !rigidbody->isStaticObject())
    {
        rigidbody->activate(true);
        //rigidbody->applyTorque( dt * forceVect.z() * btVector3(0, 1, 0));

        float x, y, z;
        btTransform tran;
        rigidbody->getMotionState()->getWorldTransform(tran);
        tran.getRotation().getEulerZYX(z, y, x);

        float thres = glm::radians(60.0f);
        const float speed = 15;

        if(y < -thres)
        {
            rigidbody->setAngularVelocity(btVector3(0, 0, 0));
        }

        if(y > -thres)
        {
            rigidbody->setAngularVelocity(btVector3(0, -speed / 2, 0));
        }

        if(forceVect.z() > 0 && y < thres)
        {
            rigidbody->setAngularVelocity(btVector3(0, speed, 0));
        }
    } else
    {
        std::cerr << "Object \"" << name << "\" does not have a rigidbody (Movable::DerivedUpdate)" << std::endl;
        exit(1);
    }
}

void Movable::KeyDown(SDL_Event event){
    SDL_Keycode keycode = event.key.keysym.sym;
    if(keycode == SDLK_UP){
        forceVect.setZ(force);
    }

    if(keycode == SDLK_DOWN){
        forceVect.setZ(-force);
    }

    if(keycode == SDLK_LEFT){
        forceVect.setX(force);
    }

    if(keycode == SDLK_RIGHT){
        forceVect.setX(-force);
    }

}

void Movable::KeyUp(SDL_Event event){
    SDL_Keycode keycode = event.key.keysym.sym;
    if(keycode == SDLK_UP){
        forceVect.setZ(0);
    }

    if(keycode == SDLK_DOWN){
        forceVect.setZ(0);
    }

    if(keycode == SDLK_LEFT){
        forceVect.setX(0);
    }

    if(keycode == SDLK_RIGHT){
        forceVect.setX(0);
    }
}

