#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "baseobject.h"

class PointLight : public BaseObject {
public:
    PointLight(std::string _name, BaseObject *parent_, glm::vec4 _lightPosition, float _intensity);
    virtual void DerivedUpdate(float dt);

    virtual void MouseDown(SDL_Event event);
    virtual void MouseUp(SDL_Event event);
    virtual void KeyDown(SDL_Event event);
    virtual void KeyUp(SDL_Event event);
    virtual void MouseWheel(SDL_Event event);
    virtual void MouseMotion(SDL_Event event);

    glm::vec4 lightPosition;
    float intensity;


};

#endif
