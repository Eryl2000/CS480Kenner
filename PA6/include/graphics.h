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

class Engine;

class Graphics
{
  public:
    Graphics(Engine *_engine);
    ~Graphics();
    bool Initialize(int width, int height, std::string vertexShader, std::string fragmentShader, std::string objectPath);
    void Update(unsigned int dt);
    void Render();
    void HandleInput(SDL_Event event);

    std::vector<BaseObject *> objects;

    Engine *engine;

  private:
    std::string ErrorString(GLenum error);

    Camera *m_camera;
    Shader *m_shader;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;
};

#endif /* GRAPHICS_H */
