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

class Engine;

class Graphics{
public:
    Graphics(Engine *_engine);
    ~Graphics();
    bool Initialize(int width, int height, std::string vertexShader, std::string fragmentShader);
    void Update(double dt);
    void Render();
    void HandleInput(SDL_Event event);

    std::vector<BaseObject *> objects;
    std::vector<Planet *> planets;
    SDL_Window* gWindow;
    Engine *engine;

private:
    bool harrisButton;

    std::string ErrorString(GLenum error);
    void createObjects(int width, int height);
    std::vector<PlanetInfo> getPlanets(std::string csv_file_name);

    Camera *m_camera;
    Shader *m_shader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;

    btBroadphaseInterface *broadphase;
    btDefaultCollisionConfiguration *collisionConfiguration;
    btCollisionDispatcher *dispatcher;
    btSequentialImpulseConstraintSolver *solver;
    btDiscreteDynamicsWorld *dynamicsWorld;

};

#endif /* GRAPHICS_H */
