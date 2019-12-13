#ifndef CONE_H_
#define CONE_H_

#include "physics_object.h"

class Cone : public PhysicsObject
{
    public:
        Cone(std::string _name, BaseObject *parent_, std::string objectPath, struct PhysicsOptions physics);
        void DerivedUpdate(float dt);
        bool isDead;
    protected:
        float timeSinceDead;
};

#endif
