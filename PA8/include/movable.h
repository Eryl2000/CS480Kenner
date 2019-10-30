#ifndef MOVABLE_H_
#define MOVANLE_H_

#include "cube.h"
#include <btBulletDynamicsCommon.h>

class Movable : public Cube
{
    public:
        Movable(std::string _name, BaseObject *parent_, std::string objectPath);
        void DerivedUpdate(float dt);
        virtual void KeyDown(SDL_Event event);
        virtual void KeyUp(SDL_Event event);

        float force;

    protected:
        btVector3 forceVect;
};


#endif