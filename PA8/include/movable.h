#ifndef MOVABLE_H_
#define MOVANLE_H_

#include "physics_object.h"
#include <btBulletDynamicsCommon.h>

class Movable : public PhysicsObject
{
    public:
        Movable(std::string _name, BaseObject *parent_, std::string objectPath, struct PhysicsOptions physics);
        void DerivedUpdate(float dt);
        virtual void KeyDown(SDL_Event event);
        virtual void KeyUp(SDL_Event event);

        float force;

    protected:
        btVector3 forceVect;
};


#endif