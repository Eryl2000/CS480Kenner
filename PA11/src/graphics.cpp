#include "graphics.h"
#include "engine.h"
#include "csv.h"
#include <cstdlib>
#include <ctime>
#include "movable.h"
#include "baseobject.h"
#include "flipper.h"
#include "plunger.h"
#include "car.h"
#include "cone.h"

Graphics::Graphics(Engine *_engine)
    : engine(_engine){

    broadphase = NULL;
    collisionConfiguration = NULL;
    dispatcher = NULL;
    solver = NULL;
    dynamicsWorld = NULL;
    wasCollidedWithObstacle = false;
    score = 0;
}

Graphics::~Graphics(){
    for(unsigned int i = 0; i < objects.size(); ++i){
        delete objects[i];
    }
    broadphase = NULL;
    collisionConfiguration = NULL;
    dispatcher = NULL;
    solver = NULL;
    dynamicsWorld = NULL;
}

/*
 * Create the objects in the scene with the correct parenting and store
 * them into a vector
 */
void Graphics::createObjects(int width, int height){
    m_pointLight = new PointLight("light", NULL, glm::vec4(2, 100, 0, 0), 0.4);

    m_camera = new Camera(engine);
    objects.push_back(m_camera);

    //car
    BaseObject *temp;
    PhysicsOptions car(true, ColliderType::Mesh, PhysicsType::Dynamic, 0, btVector3(-7.6, 0.2f, 0));
    Car *temp_car = new Car(std::string("purple"), NULL, std::string("../obj/car.obj"), car);
    temp = temp_car;
    sphere = temp_car;
    objects.push_back(temp);
    dynamicsWorld->addRigidBody(temp->rigidbody, 1, 1);

    temp->rigidbody->setLinearFactor(btVector3(1, 0, 0));

    if(!m_camera->Initialize(width, height, temp_car)){
        printf("Camera Failed to Initialize\n");
        exit(1);
    }

    //racetrack
    PhysicsOptions track_options(true, ColliderType::Mesh, PhysicsType::Static, 0, btVector3(0, 0, 0));
    temp = new PhysicsObject(std::string("granite"), NULL, std::string("../obj/racetrack.obj"), track_options);
    objects.push_back(temp);
    dynamicsWorld->addRigidBody(temp->rigidbody, 1, 1);

    PhysicsOptions cube(true, ColliderType::Cube, PhysicsType::Dynamic, 0, btVector3(-7.6, 1, 5));
    //temp = new PhysicsObject(std::string("orange"), NULL, std::string("../obj/cone.obj"), cube);
    //objects.push_back(temp);
    //dynamicsWorld->addRigidBody(temp->rigidbody, 1, 1);

    btVector3 conePositions[] = {btVector3(-7.6, 1, 5), btVector3(-7.6, 1, 10), btVector3(-8, 1, 15), btVector3(-8.6, 1, 20), btVector3(-8.6, 1, 25)};
    for(int i = 0; i < 5; i++)
    {
        cube.position = conePositions[i];
        Cone *c = new Cone(std::string("orange"), NULL, std::string("../obj/cone.obj"), cube);
        cones[c->rigidbody] = c;
        temp = c;
        objects.push_back(temp);
        dynamicsWorld->addRigidBody(temp->rigidbody, 1, 1);
    }

}

void Graphics::createBall()
{

    //ball
    if(ballNumber <= 2)
    {
        PhysicsOptions ps(true, ColliderType::Sphere, PhysicsType::Dynamic, 0, btVector3(3, 1, -3.25));
        sphere = new PhysicsObject(std::string("granite"), NULL, std::string("../obj/scaledball.obj"), ps);
        objects.push_back(sphere);
        dynamicsWorld->addRigidBody(sphere->rigidbody, 1, 1);
        ballNumber++;
    }
    else
    {
        std::cout<<"No more balls left!"<<std::endl;
    }

}

/*
 * Parses the CSV file to get the information for the solar system
 */
std::vector<PlanetInfo> Graphics::getPlanets(std::string csv_file_name)
{
    std::vector<PlanetInfo> ret;
    io::CSVReader<7> in(csv_file_name);
    in.read_header(io::ignore_extra_column, "planet", "diameter (km)", "rotation_period (hrs)", "distance_from_sun (10^6 km)", "orbital_period (days)", "number_of_moons", "has_ring_system");
    std::string name;
    long diameter;
    float rotationalPeriod;
    float distFromSun;
    float orbitPeriod;
    int numMoon;
    std::string hasRings;
    while(in.read_row(name, diameter, rotationalPeriod, distFromSun, orbitPeriod, numMoon, hasRings))
    {
        PlanetInfo p;

        p.name = name;
        p.diameter = diameter * 1000;
        p.rotationalPeriod = rotationalPeriod;
        p.distFromSun = (long) distFromSun * 1e9;
        p.orbitPeriod = (long) orbitPeriod * 24;
        p.numMoon = numMoon;
        p.hasRings = hasRings == "Yes";

        ret.push_back(p);
    }

    return ret;
}

bool Graphics::Initialize(int width, int height, std::string vertexShader, std::string fragmentShader){
    //Used for the linux OS
#if !defined(__APPLE__) && !defined(MACOSX)
    //cout << glewGetString(GLEW_VERSION) << endl;
    glewExperimental = GL_TRUE;

    auto status = glewInit();

    //This is here to grab the error that comes from glew init.
    //This error is an GL_INVALID_ENUM that has no effects on the performance
    glGetError();

    //Check for error
    if(status != GLEW_OK){
        std::cerr << "GLEW Error: " << glewGetErrorString(status) << "\n";
        return false;
    }
#endif

    //For OpenGL 3
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    broadphase = new btDbvtBroadphase();
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    solver = new btSequentialImpulseConstraintSolver;
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0, -9.81, 0));

    createObjects(width, height);

    //Set up the per vertex shader
    shaderPerVert = new Shader();
    if(!shaderPerVert->Initialize()){
        printf("Shader Failed to Initialize\n");
        return false;
    }

    //Add the vertex shader
    if(!shaderPerVert->AddShader(GL_VERTEX_SHADER, "../shaders/vertexLight.vert")){
        printf("Vertex Shader failed to Initialize\n");
        return false;
    }

    //Add the fragment shader
    if(!shaderPerVert->AddShader(GL_FRAGMENT_SHADER, "../shaders/vertexLight.frag")){
        printf("Fragment Shader failed to Initialize\n");
        return false;
    }

    //Connect the program
    if(!shaderPerVert->Finalize()){
        printf("Program to Finalize\n");
        return false;
    }

////////////////////////////////////////////////////////////////////////

    //Set up the per fragment shader
    shaderPerFrag = new Shader();
    if(!shaderPerFrag->Initialize()){
        printf("Shader Failed to Initialize\n");
        return false;
    }

    //Add the vertex shader
    if(!shaderPerFrag->AddShader(GL_VERTEX_SHADER, "../shaders/fragmentLight.vert")){
        printf("Vertex Shader failed to Initialize\n");
        return false;
    }

    //Add the fragment shader
    if(!shaderPerFrag->AddShader(GL_FRAGMENT_SHADER, "../shaders/fragmentLight.frag")){
        printf("Fragment Shader failed to Initialize\n");
        return false;
    }

    //Connect the program
    if(!shaderPerFrag->Finalize()){
        printf("Program to Finalize\n");
        return false;
    }

////////////////////////////////////////////////////////////////////////

    m_current = shaderPerFrag;

    //Locate the projection matrix in the shader
    m_projectionMatrix = m_current->GetUniformLocation("Projection");
    if (m_projectionMatrix == INVALID_UNIFORM_LOCATION){
        printf("m_projectionMatrix not found\n");
        return false;
    }

    m_viewMatrix = m_current->GetUniformLocation("View");
    if (m_viewMatrix == INVALID_UNIFORM_LOCATION){
        printf("m_viewMatrix not found\n");
        return false;
    }

    //Locate the model view matrix in the shader
    m_modelMatrix = m_current->GetUniformLocation("Model");
    if (m_modelMatrix == INVALID_UNIFORM_LOCATION){
        printf("m_modelMatrix not found\n");
        return false;
    }

    m_ambientProduct = m_current->GetUniformLocation("AmbientProduct");
    if (m_ambientProduct == INVALID_UNIFORM_LOCATION){
        printf("m_ambientProduct not found. Are you using a non-lighting shader?\n");
        return false;
    }

    m_diffuseProduct = m_current->GetUniformLocation("DiffuseProduct");
    if (m_diffuseProduct == INVALID_UNIFORM_LOCATION){
        printf("m_diffuseProduct not found. Are you using a non-lighting shader?\n");
        return false;
    }

    m_specularProduct = m_current->GetUniformLocation("SpecularProduct");
    if (m_specularProduct == INVALID_UNIFORM_LOCATION){
        printf("m_specularProduct not found. Are you using a non-lighting shader?\n");
        return false;
    }

    m_lightPosition = m_current->GetUniformLocation("LightPosition");
    if (m_lightPosition == INVALID_UNIFORM_LOCATION){
        printf("m_lightPosition not found. Are you using a non-lighting shader?\n");
        return false;
    }

    m_shininess = m_current->GetUniformLocation("Shininess");
    if (m_shininess == INVALID_UNIFORM_LOCATION){
        printf("m_shininess not found. Are you using a non-lighting shader?\n");
        return false;
    }

    m_spotPosition = m_current->GetUniformLocation("SpotPos");
    if (m_spotPosition == INVALID_UNIFORM_LOCATION){
        printf("m_spotPosition not found. Are you using a non-lighting shader?\n");
        return false;
    }

    m_spotDirection = m_current->GetUniformLocation("SpotDir");
    if (m_spotDirection == INVALID_UNIFORM_LOCATION){
        printf("m_spotDirection not found. Are you using a non-lighting shader?\n");
        return false;
    }

    m_spotCutoff = m_current->GetUniformLocation("SpotCutOff");
    if (m_spotCutoff == INVALID_UNIFORM_LOCATION){
        printf("m_spotCutoff not found. Are you using a non-lighting shader?\n");
        return false;
    }

    m_diffuseColor = m_current->GetUniformLocation("DiffuseColor");
    if (m_diffuseColor == INVALID_UNIFORM_LOCATION){
        printf("m_diffuseColor not found. Are you using a non-lighting shader?\n");
        return false;
    }


    //enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    return true;
}

//Updates each object in the scene
void Graphics::Update(double dt){
    dynamicsWorld->stepSimulation(dt, 10);
    int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();
    for (int i = 0;i < numManifolds;i++){
        btPersistentManifold* contactManifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
        const btCollisionObject* obA = (contactManifold->getBody0());
        const btCollisionObject* obB = (contactManifold->getBody1());

        if(obA == sphere->rigidbody){
            auto iter = cones.find(obB);
            if(iter != cones.end()){
                iter->second->isDead = true;
                cones.erase(iter);
                score++;
                std::cout << "Score: " << score << std::endl;
            }
        }
        if(obB == sphere->rigidbody){
            auto iter = cones.find(obA);
            if(iter != cones.end()){
                iter->second->isDead = true;
                cones.erase(iter);
                score++;
                std::cout << "Score: " << score << std::endl;
            }
        }
    }
    for(unsigned int i = 0; i < objects.size(); ++i){
        objects[i]->Update(dt);
    }
}

//Renders a single image to the screen
void Graphics::Render(){
    //clear the screen
    glClearColor(0.0, 0.0, 0.1f, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Start the correct program
    m_current->Enable();

    // Send in the projection to the shader
    glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection()));
    glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView()));
    glUniform4fv(m_lightPosition, 1, glm::value_ptr(m_pointLight->lightPosition));
    glUniform3fv(m_diffuseColor, 1, glm::value_ptr(glm::vec3(0.5f, 0.5f, 0.5f)));

    // Get the position of the ball
    glm::vec3 spherePos = sphere->GetModel()[3];
    glm::vec3 carForward = sphere->GetModel()[2];
    glm::vec3 lightRot = glm::vec3(0, -0.25f, 0);
    glm::vec3 * printVec = &carForward;
    //std::cout << "Car pos: " << printVec->x << ", " << printVec->y << ", " << printVec->z << std::endl;
    glm::vec4 lightPos = glm::vec4(0, 0.5f, -0.5f, 0);
    glUniform3fv(m_spotPosition, 1, glm::value_ptr(glm::vec4(spherePos, 1) + sphere->GetModel() * lightPos));
    glUniform3fv(m_spotDirection, 1, glm::value_ptr(lightRot + carForward));
    float cutOff = glm::cos(glm::radians(20.0f));
    glUniform1f(m_spotCutoff, cutOff);

    // Render the objects
    for(unsigned int i = 0; i < objects.size(); ++i){
        glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(objects[i]->GetModel()));
        glUniform4fv(m_ambientProduct, 1, glm::value_ptr(objects[i]->GetAmbient()));
        glUniform4fv(m_diffuseProduct, 1, glm::value_ptr(objects[i]->GetDiffuse()));
        glUniform4fv(m_specularProduct, 1, glm::value_ptr(objects[i]->GetSpecular()));
        glUniform1f(m_shininess, objects[i]->GetShininess());
        objects[i]->Render();
    }

    // Get any errors from OpenGL
    auto error = glGetError();
    if(error != GL_NO_ERROR){
        std::string val = ErrorString( error );
        std::cout<< "Error initializing OpenGL! " << error << ", " << val << std::endl;
    }
}

std::string Graphics::ErrorString(GLenum error){
    if(error == GL_INVALID_ENUM){
        return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
    } else if(error == GL_INVALID_VALUE){
        return "GL_INVALID_VALUE: A numeric argument is out of range.";
    } else if(error == GL_INVALID_OPERATION){
        return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
    } else if(error == GL_INVALID_FRAMEBUFFER_OPERATION){
        return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
    } else if(error == GL_OUT_OF_MEMORY){
        return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
    } else{
        return "None";
    }
}

/*
 * Checks what type of event has occurred and call the corresponding callback
 * for each object in the scene
 */
void Graphics::HandleInput(SDL_Event event){

    const float orbitParamIncrease = 2;

    if(event.type == SDL_QUIT){
        engine->running = false;
	} else if (event.type == SDL_KEYDOWN){
        if(event.key.keysym.sym == SDLK_ESCAPE){
            engine->running = false;
        } else if(event.key.keysym.sym == SDLK_o){
            engine->timeScalar -= 0.1;
        } else if(event.key.keysym.sym == SDLK_p){
            engine->timeScalar += 0.1;
        } else if(event.key.keysym.sym == SDLK_h){
            harrisButton = true;
            for(unsigned int i = 1; i < objects.size(); ++i){
                objects[i]->HarrisButton(harrisButton);
            }
        } else if(event.key.keysym.sym == SDLK_j){
            harrisButton = false;
            for(unsigned int i = 1; i < objects.size(); ++i){
                objects[i]->HarrisButton(harrisButton);
            }
        } else if(event.key.keysym.sym == SDLK_i){
            for(unsigned int i = 1; i < planets.size(); ++i){
                planets[i]->orbitParamVel = orbitParamIncrease;
            }
        } else if(event.key.keysym.sym == SDLK_k){
            for(unsigned int i = 1; i < planets.size(); ++i){
                planets[i]->orbitParamVel = -orbitParamIncrease;
            }
        } else if(event.key.keysym.sym == SDLK_m){
            toggleShader();
        } else if(event.key.keysym.sym == SDLK_n){
            for(unsigned int i = 0; i < objects.size(); ++i){
                objects[i]->incrementAmbient();
            }
        } else if(event.key.keysym.sym == SDLK_b){
            for(unsigned int i = 0; i < objects.size(); ++i){
                objects[i]->decrementAmbient();
            }
        } else if(event.key.keysym.sym == SDLK_g){
            for(unsigned int i = 0; i < objects.size(); ++i){
                objects[i]->incrementSpecular();
            }
        } else if(event.key.keysym.sym == SDLK_f){
            for(unsigned int i = 0; i < objects.size(); ++i){
                objects[i]->decrementSpecular();
            }
        } else if(event.key.keysym.sym == SDLK_t){
            for(unsigned int i = 0; i < objects.size(); ++i){
                objects[i]->incrementDiffuse();
            }
        } else if(event.key.keysym.sym == SDLK_r){
            for(unsigned int i = 0; i < objects.size(); ++i){
                objects[i]->decrementDiffuse();
            }
        } else if(event.key.keysym.sym == SDLK_z){
                createBall();
        }else{
            for(unsigned int i = 0; i < objects.size(); ++i){
                objects[i]->KeyDown(event);
            }
        }
	} else if (event.type == SDL_KEYUP){
        if(event.key.keysym.sym == SDLK_i){
            for(unsigned int i = 1; i < planets.size(); ++i){
                planets[i]->orbitParamVel = 0;
            }
        } else if(event.key.keysym.sym == SDLK_k){
            for(unsigned int i = 1; i < planets.size(); ++i){
                planets[i]->orbitParamVel = 0;
            }
        } else {
            for(unsigned int i = 0; i < objects.size(); ++i){
                objects[i]->KeyUp(event);
            }
        }
    } else if(event.type == SDL_MOUSEBUTTONDOWN){
        for(unsigned int i = 0; i < objects.size(); ++i){
            objects[i]->MouseDown(event);
        }
    } else if(event.type == SDL_MOUSEBUTTONUP){
        for(unsigned int i = 0; i < objects.size(); ++i){
            objects[i]->MouseUp(event);
        }
    } else if(event.type == SDL_MOUSEWHEEL){
        for(unsigned int i = 0; i < objects.size(); ++i){
            objects[i]->MouseWheel(event);
        }
	} else if(event.type == SDL_MOUSEMOTION){
        SDL_WarpMouseInWindow(gWindow,400,300);
        for(unsigned int i = 0; i < objects.size(); ++i){
            objects[i]->MouseMotion(event);
        }
	}
}

void Graphics::toggleShader(){
    if(isVertexLighting == true)
    {
        std::cout<<"Switching to per fragment lighting"<<std::endl;
        m_current = shaderPerFrag;
        isVertexLighting = false;
    }
    else
    {
        std::cout<<"Switching to per vertex lighting"<<std::endl;
        m_current = shaderPerVert;
        isVertexLighting = true;
    }

    m_projectionMatrix = m_current->GetUniformLocation("Projection");

    m_viewMatrix = m_current->GetUniformLocation("View");

    m_modelMatrix = m_current->GetUniformLocation("Model");

    m_ambientProduct = m_current->GetUniformLocation("AmbientProduct");

    m_diffuseProduct = m_current->GetUniformLocation("DiffuseProduct");

    m_specularProduct = m_current->GetUniformLocation("SpecularProduct");

    m_lightPosition = m_current->GetUniformLocation("LightPosition");

    m_shininess = m_current->GetUniformLocation("Shininess");

    m_spotPosition = m_current->GetUniformLocation("SpotPos");

    m_spotDirection = m_current->GetUniformLocation("SpotDir");

    m_spotCutoff = m_current->GetUniformLocation("SpotCutOff");

    m_diffuseColor = m_current->GetUniformLocation("DiffuseColor");

}
