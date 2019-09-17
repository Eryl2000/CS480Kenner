#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "graphics_headers.h"

class Object
{
public:
    Object(Object *parent);
    ~Object();
    void Update(unsigned int dt);
    virtual void DerivedUpdate(float dt) = 0;
    void Render();

    glm::mat4 GetModel();

    Object *Getparent();
    void SetParent(Object *parent_);

    //Current angle of the cube in radians
    float angle;
protected:
    glm::mat4 model;

    Object *parent;
    std::vector<Object *> children;
private:
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;
};

#endif /* OBJECT_H */
