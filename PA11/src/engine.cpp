#include <chrono>
#include <iomanip>
#include "engine.h"
#include "graphics.h"


Engine::Engine(std::string name, int width, int height){
    m_WINDOW_NAME = name;
    m_WINDOW_WIDTH = width;
    m_WINDOW_HEIGHT = height;
    m_FULLSCREEN = false;
    timeScalar = 1.0;
}

Engine::Engine(std::string name){
    m_WINDOW_NAME = name;
    m_WINDOW_HEIGHT = 0;
    m_WINDOW_WIDTH = 0;
    m_FULLSCREEN = true;
    timeScalar = 1.0;
}

Engine::~Engine(){
    delete m_window;
    delete m_graphics;
    m_window = NULL;
    m_graphics = NULL;
}

bool Engine::Initialize(std::string vertexShader, std::string fragmentShader){
    // Start a window
    m_window = new Window();
    if(!m_window->Initialize(m_WINDOW_NAME, &m_WINDOW_WIDTH, &m_WINDOW_HEIGHT)){
        printf("The window failed to initialize.\n");
        return false;
    }

    // Start the graphics
    m_graphics = new Graphics(this);
    m_graphics->gWindow = m_window->getSDLWindow();
    if(!m_graphics->Initialize(m_WINDOW_WIDTH, m_WINDOW_HEIGHT, vertexShader, fragmentShader)){
        printf("The graphics failed to initialize.\n");
        return false;
    }

    // No errors
    return true;
}

void Engine::Run(){
    using namespace std::chrono;
    running = true;

    const double FPS_Alpha = 0.25;
    fps = 1.0;

    // Set how long the game plays for
    const int gameTime = 60;

    double accumulator = 0.0;
    double runTime = 0.0;
    int reportTime = 1;
    bool frozen = false;
    high_resolution_clock::time_point prevTime = high_resolution_clock::now();
    high_resolution_clock::time_point curTime = prevTime;
    while(running){
        //Calculate dt
        curTime = high_resolution_clock::now();
        dt = duration_cast<nanoseconds>(curTime - prevTime).count() / 1000000000.0;
        runTime += dt;

        if((int)runTime >= reportTime && !frozen)
        {
            std::cout << "Remaining time: " << std::to_string(gameTime - (int) runTime) << std::endl;
            reportTime++;

            if(reportTime == gameTime + 1)
            {
                std::cout << "Final score: " << std::to_string(m_graphics->score) << std::endl;
                frozen = true;
            }
        }

        if(frozen)
        {
            dt = 0.0000001;
        }

        //Calculate smooth fps
        fps = (1.0 / dt) * FPS_Alpha + fps * (1.0 - FPS_Alpha);
        smoothedFPS = (int)(fps + 0.5);

        SDL_Event event;
        while(SDL_PollEvent(&event) != 0){
            m_graphics->HandleInput(event);
        }

        //Fixed-rate physics update loop
        accumulator += dt;
        m_graphics->Render();
        while(accumulator >= dt){
            m_graphics->Update(dt * timeScalar);
            accumulator -= dt;
        }


        // Swap to the Window
        m_window->Swap();

        if(time_point_cast<seconds>(curTime) != time_point_cast<seconds>(prevTime)){
            //std::cout << "FPS: " << smoothedFPS << std::endl;
        }

        prevTime = curTime;
    }
}
