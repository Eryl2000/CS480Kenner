#ifndef BASEOBJECT_H
#define BASEOBJECT_H

#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include "graphics_headers.h"

class BaseObject{
public:
    BaseObject(BaseObject *parent, std::string objectFile);
    virtual ~BaseObject();
    void Update(double dt);
    void Render();

    glm::mat4 GetModel();

    BaseObject *Getparent();
    void SetParent(BaseObject *parent_);

    void setPosition(glm::vec3 pos);
    void setEulerAngle(glm::vec3 eulerAngle);
    void setScale(glm::vec3 scale);

    glm::vec3 getPosition() const;
    glm::vec3 getEulerAngle() const;
    glm::vec3 getScale() const;

    float angle;

    virtual void MouseDown(SDL_Event event);
    virtual void MouseUp(SDL_Event event);
    virtual void KeyDown(SDL_Event event);
    virtual void KeyUp(SDL_Event event);
    virtual void MouseWheel(SDL_Event event);

protected:
    virtual void DerivedUpdate(double dt) = 0;
    glm::mat4 model;

    glm::vec3 position;

    BaseObject *parent;
    std::vector<BaseObject *> children;

private:
    bool LoadObject(std::string objectPath);

    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;
};

#endif /* BASEOBJECT_H */
