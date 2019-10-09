#ifndef BASEOBJECT_H
#define BASEOBJECT_H

#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include <Magick++.h>
#include "graphics_headers.h"

class BaseObject{
public:
    BaseObject(std::string _name, BaseObject *parent, std::string objectFile);
    virtual ~BaseObject();
    void Update(float dt);
    void Render();

    glm::mat4 GetModel();
    void printModel() const;

    BaseObject *Getparent();
    void SetParent(BaseObject *parent_);

    glm::vec3 getPosition() const;
    glm::vec3 getEulerAngle() const;
    glm::vec3 getScale() const;

    std::string name;

    virtual void MouseDown(SDL_Event event);
    virtual void MouseUp(SDL_Event event);
    virtual void KeyDown(SDL_Event event);
    virtual void KeyUp(SDL_Event event);
    virtual void MouseWheel(SDL_Event event);

protected:
    virtual void DerivedUpdate(float dt) = 0;
    glm::mat4 model;

    glm::vec3 position;
    glm::vec3 eulerAngle;
    glm::vec3 scale;

    BaseObject *parent;
    std::vector<BaseObject *> children;

private:
    bool LoadObject(std::string objectPath);

    void SetTransform(glm::vec3 _position, glm::vec3 _eulerAngle, glm::vec3 _scale);

    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;

    Magick::Image *image;
    Magick::Blob blob;

    GLuint texture;

};

#endif /* BASEOBJECT_H */
