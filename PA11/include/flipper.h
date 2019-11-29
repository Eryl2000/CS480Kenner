#ifndef FLIPPER_H_
#define FLIPPER_H_

#include "physics_object.h"

class Flipper : public PhysicsObject
{
    public:
        Flipper(std::string _name, BaseObject *parent_, std::string objectPath, struct PhysicsOptions physics, bool _left, SDL_Keycode triggerKey);
        void DerivedUpdate(float dt);
        virtual void KeyDown(SDL_Event event);
        virtual void KeyUp(SDL_Event event);
        float threshold;
        float speed;

    protected:
        bool pressed = false;
        bool left;
        SDL_Keycode pressThis;
};

#endif