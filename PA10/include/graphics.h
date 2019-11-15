#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
#include <string.h>
#include <vector>
#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "baseobject.h"
#include "planet.h"
#include "object.h"
#include <SDL2/SDL.h>
#include <btBulletDynamicsCommon.h>
#include "pointLight.h"
#include "physics_object.h"

class Engine;

class Graphics{
public:
    Graphics(Engine *_engine);
    ~Graphics();
    bool Initialize(int width, int height, std::string vertexShader, std::string fragmentShader);
    void Update(double dt);
    void Render();
    void HandleInput(SDL_Event event);
    void toggleShader();
    void createBall();

    std::vector<BaseObject *> objects;
    std::vector<Planet *> planets;
    SDL_Window* gWindow;
    Engine *engine;

private:
    bool harrisButton;
    bool isVertexLighting = true;

    std::string ErrorString(GLenum error);
    void createObjects(int width, int height);
    std::vector<PlanetInfo> getPlanets(std::string csv_file_name);

    Camera *m_camera;
    Shader *m_current;
    Shader *shaderPerVert;
    Shader *shaderPerFrag;
    Shader *m_shader;
    PointLight *m_pointLight;
    PhysicsObject * sphere;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;
    GLint m_ambientProduct;
    GLint m_diffuseProduct;
    GLint m_specularProduct;
    GLint m_lightPosition;
    GLint m_shininess;
    GLint m_spotPosition;
    GLint m_spotDirection;
    GLint m_spotCutoff;
    GLint m_diffuseColor;

    btBroadphaseInterface *broadphase;
    btDefaultCollisionConfiguration *collisionConfiguration;
    btCollisionDispatcher *dispatcher;
    btSequentialImpulseConstraintSolver *solver;
    btDiscreteDynamicsWorld *dynamicsWorld;

};

#endif /* GRAPHICS_H */
