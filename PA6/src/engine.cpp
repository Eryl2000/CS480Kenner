#include "engine.h"
#include "graphics.h"

Engine::Engine(std::string name, int width, int height){
    m_WINDOW_NAME = name;
    m_WINDOW_WIDTH = width;
    m_WINDOW_HEIGHT = height;
    m_FULLSCREEN = false;
}

Engine::Engine(std::string name){
    m_WINDOW_NAME = name;
    m_WINDOW_HEIGHT = 0;
    m_WINDOW_WIDTH = 0;
    m_FULLSCREEN = true;
}

Engine::~Engine(){
    delete m_window;
    delete m_graphics;
    m_window = NULL;
    m_graphics = NULL;
}

bool Engine::Initialize(std::string vertexShader, std::string fragmentShader, std::string objectPath){
    // Start a window
    m_window = new Window();
    if(!m_window->Initialize(m_WINDOW_NAME, &m_WINDOW_WIDTH, &m_WINDOW_HEIGHT)){
        printf("The window failed to initialize.\n");
        return false;
    }

    // Start the graphics
    m_graphics = new Graphics(this);
    if(!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT, vertexShader, fragmentShader, objectPath)){
        printf("The graphics failed to initialize.\n");
        return false;
    }

    // Set the time
    m_currentTimeMillis = GetCurrentTimeMillis();

    // No errors
    return true;
}

void Engine::Run(){
    running = true;

    while(running){
        m_DT = getDT();

        SDL_Event event;
        while(SDL_PollEvent(&event) != 0){
            m_graphics->HandleInput(event);
        }

        // Update and render the graphics
        m_graphics->Update(m_DT);
        m_graphics->Render();

        // Swap to the Window
        m_window->Swap();
    }
}

unsigned int Engine::getDT(){
    long long TimeNowMillis = GetCurrentTimeMillis();
    assert(TimeNowMillis >= m_currentTimeMillis);
    unsigned int DeltaTimeMillis = (unsigned int)(TimeNowMillis - m_currentTimeMillis);
    m_currentTimeMillis = TimeNowMillis;
    return DeltaTimeMillis;
}

long long Engine::GetCurrentTimeMillis(){
    timeval t;
    gettimeofday(&t, NULL);
    long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
    return ret;
}
