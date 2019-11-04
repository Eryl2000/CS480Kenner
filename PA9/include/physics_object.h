#ifndef CUBE_H
#define CUBE_H

#include "baseobject.h"

class PhysicsObject : public BaseObject {
public:
    PhysicsObject(std::string _name, BaseObject *parent_, std::string objectFile, struct PhysicsOptions physics);
    virtual void DerivedUpdate(float dt);

    virtual void MouseDown(SDL_Event event);
    virtual void MouseUp(SDL_Event event);
    virtual void KeyDown(SDL_Event event);
    virtual void KeyUp(SDL_Event event);
    virtual void MouseWheel(SDL_Event event);
    virtual void MouseMotion(SDL_Event event);

protected:
    btCollisionShape * GetCollisionShape(struct PhysicsOptions physics);

private:

};

#endif
