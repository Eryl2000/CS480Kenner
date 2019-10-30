#include "movable.h"

Movable::Movable(std::string _name, BaseObject *parent_, std::string objectPath)
    : BaseObject(_name, parent_, objectPath),
      force(5),
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
        forceVect.z = force;
    }

    if(keycode == SDLK_DOWN){
        forceVect.z = -force;
    }

    if(keycode == SDLK_LEFT){
        forceVect.x = -force;
    }

    if(keycode == SDLK_RIGHT){
        forceVect.x = force;
    }

}

void Movable::KeyUp(SDL_Event event){
    SDL_Keycode keycode = event.key.keysym.sym;
    if(keycode == SDLK_UP){
        forceVect.z = 0;
    }

    if(keycode == SDLK_DOWN){
        forceVect.z = 0;
    }

    if(keycode == SDLK_LEFT){
        forceVect.x = 0;
    }

    if(keycode == SDLK_RIGHT){
        forceVect.x = 0;
    }
}

