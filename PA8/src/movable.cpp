#include "movable.h"

Movable::Movable(std::string _name, BaseObject *parent_, std::string objectPath, struct PhysicsOptions physics)
    : PhysicsObject(_name, parent_, objectPath, physics),
      force(100),
      forceVect(btVector3(0, 0, 0))
{}

void Movable::DerivedUpdate(float dt)
{
    if(rigidbody != NULL && !rigidbody->isStaticObject())
    {
        rigidbody->activate(true);
        rigidbody->applyCentralForce( dt * forceVect);
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

