#ifndef BASEOBJECT_H
#define BASEOBJECT_H

#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include <assimp/scene.h> //includes the aiScene object
#include "graphics_headers.h"
#include <Magick++.h>
#include <btBulletDynamicsCommon.h>

enum class ColliderType {None, Plane, Sphere, Cube, Cylinder, Mesh};
enum class PhysicsType {Static, Kinematic, Dynamic};

struct PhysicsOptions{
    bool hasPhysics;
    ColliderType colliderType;
    PhysicsType physicsType;
};

class BaseObject{
public:
    BaseObject(std::string _name, BaseObject *parent_, std::string objectFile, bool _onlyCopyParentPos = false);
    BaseObject(std::string _name, BaseObject *parent_, const aiScene * scene, unsigned int modelIndex, bool _onlyCopyParentPos = false);
    virtual ~BaseObject();
    void Update(float dt);
    void Render();

    glm::mat4 GetModel();
    void printModel() const;

    BaseObject *Getparent();
    void SetParent(BaseObject *parent_);

    void HarrisButton(bool _harrisButton);

    glm::vec3 getPosition() const;
    glm::vec3 getEulerAngle() const;
    glm::vec3 getScale() const;

    std::string name;

    virtual void MouseDown(SDL_Event event);
    virtual void MouseUp(SDL_Event event);
    virtual void KeyDown(SDL_Event event);
    virtual void KeyUp(SDL_Event event);
    virtual void MouseWheel(SDL_Event event);
    virtual void MouseMotion(SDL_Event event);

    std::vector<BaseObject *> children;

    btRigidBody * rigidbody;

protected:
    virtual void DerivedUpdate(float dt) = 0;
    glm::mat4 model;

    bool onlyCopyParentPos;

    glm::vec3 position;
    glm::vec3 eulerAngle;
    glm::vec3 scale;

    BaseObject *parent;

    btCollisionShape *collider;

private:
    void Setup(BaseObject *parent_);
    bool LoadObject(std::string objectPath);
    bool LoadObject(const aiScene * scene, unsigned int modelIndex);
    void Bind();

    void SetTransform(glm::vec3 _position, glm::vec3 _eulerAngle, glm::vec3 _scale);

    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;

    Magick::Image *image = NULL;
    Magick::Blob blob;

    GLuint texture;
    const bool doNotRender;
    bool harrisButton;
};

#endif /* BASEOBJECT_H */
