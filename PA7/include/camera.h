#ifndef CAMERA_H
#define CAMERA_H

#include <SDL2/SDL.h>
#include "baseobject.h"
#include "graphics_headers.h"

class Camera : public BaseObject{
public:
    Camera();
    bool Initialize(int w, int h);
    void DerivedUpdate(float dt);

    glm::mat4 GetProjection();
    glm::mat4 GetView();

    virtual void MouseDown(SDL_Event event);
    virtual void MouseUp(SDL_Event event);
    virtual void KeyDown(SDL_Event event);
    virtual void KeyUp(SDL_Event event);
    virtual void MouseWheel(SDL_Event event);
    virtual void MouseMotion(SDL_Event event);

private:
    glm::vec2 rotateVector(float angleRadians, glm::vec2 original) const;

    glm::mat4 projection;
    glm::mat4 view;

    glm::vec3 forward;

    glm::vec3 rotationVelocity;
    glm::vec3 positionVelocity;

    float speedUp;

    float width;
    float height;

    bool motionEvent = false;
};

#endif /* CAMERA_H */
