#ifndef CAR_H_
#define CAR_H_

#include "physics_object.h"

class Car : public PhysicsObject
{
    public:
        Car(std::string _name, BaseObject *parent_, std::string objectPath, struct PhysicsOptions physics);
        void DerivedUpdate(float dt);
        virtual void KeyDown(SDL_Event event);
        virtual void KeyUp(SDL_Event event);
        float speed;
        float turn;
        glm::vec3 vel;
        float angle;

    protected:
        float speedChange;

};

#endif
