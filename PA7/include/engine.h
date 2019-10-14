#ifndef ENGINE_H
#define ENGINE_H

#include <sys/time.h>
#include <assert.h>
#include <string.h>
#include "window.h"

class Graphics;

class Engine{
public:
    Engine(std::string name, int width, int height);
    Engine(std::string name);
    ~Engine();
    bool Initialize(std::string vertexShader, std::string fragmentShader, std::string objectPath);
    void Run();

    bool running;
    int smoothedFPS;
    float timeScalar;

	//References to other classes
	Graphics *m_graphics;

private:
    // Window related variables
    Window *m_window;
    std::string m_WINDOW_NAME;
    int m_WINDOW_WIDTH;
    int m_WINDOW_HEIGHT;
    bool m_FULLSCREEN;

    double fps;
};

#endif // ENGINE_H
