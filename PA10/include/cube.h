#ifndef CUBE_H
#define CUBE_H

#include "baseobject.h"

class Cube : public BaseObject {
public:
    Cube(std::string _name, BaseObject *parent_, std::string objectFile);
    virtual void DerivedUpdate(float dt);

    virtual void MouseDown(SDL_Event event);
    virtual void MouseUp(SDL_Event event);
    virtual void KeyDown(SDL_Event event);
    virtual void KeyUp(SDL_Event event);
    virtual void MouseWheel(SDL_Event event);
    virtual void MouseMotion(SDL_Event event);

protected:


private:

};

#endif
