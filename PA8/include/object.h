#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <string>
#include "graphics_headers.h"
#include "baseobject.h"

class Object : public BaseObject{
public:
    Object(std::string _name, BaseObject *parent_, std::string objectPath);
    Object(std::string _name, BaseObject *parent_, const aiScene * scene, unsigned int modelIndex);
    void DerivedUpdate(float dt);

    virtual void MouseDown(SDL_Event event);
    virtual void MouseUp(SDL_Event event);
    virtual void KeyDown(SDL_Event event);
    virtual void KeyUp(SDL_Event event);
    virtual void MouseWheel(SDL_Event event);
    virtual void MouseMotion(SDL_Event event);

private:
    glm::vec3 rotationVelocity;
    glm::vec3 positionVelocity;
    glm::vec3 scaleVelocity;

    bool buttonHeld_control;
    bool buttonHeld_alt;

};

#endif /* OBJECT_H */
