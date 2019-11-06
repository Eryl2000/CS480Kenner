#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "pointLight.h"

class SpotLight : public PointLight {
public:
    SpotLight(std::string _name, BaseObject *parent_, glm::vec4 _lightPosition, float _intensity, glm::vec4 _direction, float _cutoffAngleDegrees);
    virtual void DerivedUpdate(float dt);

    virtual void MouseDown(SDL_Event event);
    virtual void MouseUp(SDL_Event event);
    virtual void KeyDown(SDL_Event event);
    virtual void KeyUp(SDL_Event event);
    virtual void MouseWheel(SDL_Event event);
    virtual void MouseMotion(SDL_Event event);

    glm::vec4 direction;
    float cutoffAngleDegrees;


};

#endif
