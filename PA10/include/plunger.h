#ifndef PLUNGER_H_
#define PLUNGER_H_

#include "physics_object.h"

class Plunger : public PhysicsObject
{
    public:
        Plunger(std::string _name, BaseObject *parent_, std::string objectPath, struct PhysicsOptions physics, SDL_Keycode triggerKey);
        void DerivedUpdate(float dt);
        virtual void KeyDown(SDL_Event event);
        virtual void KeyUp(SDL_Event event);
        float threshold;
        float speed;

    protected:
        bool pressed = false;
        SDL_Keycode pressThis;
        btVector3 startPos;
        bool rising = false;
};

#endif