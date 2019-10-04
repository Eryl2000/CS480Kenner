#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <string>
#include "graphics_headers.h"
#include "baseobject.h"

class Object : public BaseObject
{
public:
    Object(BaseObject *parent_, std::string objectPath);
    void DerivedUpdate(float dt);

    virtual void MouseDown(SDL_Event event);
    virtual void MouseUp(SDL_Event event);
    virtual void KeyDown(SDL_Event event);
    virtual void KeyUp(SDL_Event event);
    virtual void MouseWheel(SDL_Event event);
private:
    float rotateRate;

};

#endif /* OBJECT_H */
