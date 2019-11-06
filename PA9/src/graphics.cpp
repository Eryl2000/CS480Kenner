#include "graphics.h"
#include "engine.h"
#include "csv.h"
#include <cstdlib>
#include <ctime>
#include "movable.h"
#include "baseobject.h"

Graphics::Graphics(Engine *_engine)
    : engine(_engine){

    broadphase = NULL;
    collisionConfiguration = NULL;
    dispatcher = NULL;
    solver = NULL;
    dynamicsWorld = NULL;
}

Graphics::~Graphics(){
    for(unsigned int i = 0; i < objects.size(); ++i){
        delete objects[i];
    }
    //delete broadphase;
    //delete collisionConfiguration;
    //delete dispatcher;
    //delete solver;
    //delete dynamicsWorld;
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
    m_camera = new Camera(engine);
    objects.push_back(m_camera);
    if(!m_camera->Initialize(width, height)){
        printf("Camera Failed to Initialize\n");
        exit(1);
    }

    dynamicsWorld->addRigidBody(m_camera->rigidbody, 1, 1);

    BaseObject *temp;
    PhysicsOptions ps(true, ColliderType::Cube, PhysicsType::Dynamic, 0);
    ps.position = glm::vec3(0, 2, 0);
    temp = new Movable(std::string("granite"), NULL, std::string("../obj/newcube.obj"), ps);
    objects.push_back(temp);
    dynamicsWorld->addRigidBody(temp->rigidbody, 1, 1);

    ps.colliderType = ColliderType::Sphere;
    ps.position = glm::vec3(0, 2, 2);
    temp = new PhysicsObject(std::string("granite"), NULL, std::string("../obj/newsphere.obj"), ps);
    objects.push_back(temp);
    dynamicsWorld->addRigidBody(temp->rigidbody, 1, 1);

    PhysicsOptions cylinderPS(true, ColliderType::Cylinder, PhysicsType::Static, 0);
    cylinderPS.position = glm::vec3(3, 2, 0);
    temp = new PhysicsObject(std::string("granite"), NULL, std::string("../obj/newcylinder.obj"), cylinderPS);
    objects.push_back(temp);
    dynamicsWorld->addRigidBody(temp->rigidbody, 1, 1);

    //bottom plane
    PhysicsOptions planePS(true, ColliderType::Plane, PhysicsType::Static, 0);
    temp = new PhysicsObject(std::string("granite"), NULL, std::string("../obj/newtray2.obj"), planePS);
    objects.push_back(temp);
    dynamicsWorld->addRigidBody(temp->rigidbody, 1, 1);
    
    //left plane
    planePS.planeType = 1;
    planePS.position = glm::vec3(-7, 0, 0);
    temp = new PhysicsObject(std::string("sphere"), NULL, std::string("../obj/traywall.obj"), planePS);
    objects.push_back(temp);
    dynamicsWorld->addRigidBody(temp->rigidbody, 1, 1);

    //right plane
    planePS.planeType = 2;
    planePS.position = glm::vec3(7, 0, 0);
    temp = new PhysicsObject(std::string("sphere"), NULL, std::string("../obj/traywall.obj"), planePS);
    objects.push_back(temp);
    dynamicsWorld->addRigidBody(temp->rigidbody, 1, 1);

    //back plane
    planePS.planeType = 3;
    planePS.position = glm::vec3(0, 0, -5);
    temp = new PhysicsObject(std::string("sphere"), NULL, std::string("../obj/traywall.obj"), planePS);
    objects.push_back(temp);
    dynamicsWorld->addRigidBody(temp->rigidbody, 1, 1);

    //front plane
    planePS.planeType = 4;
    planePS.position = glm::vec3(0, 0, 5);
    temp = new PhysicsObject(std::string("sphere"), NULL, std::string("../obj/traywall.obj"), planePS);
    objects.push_back(temp);
    dynamicsWorld->addRigidBody(temp->rigidbody, 1, 1);
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

    //Set up the shaders
    m_shader = new Shader();
    if(!m_shader->Initialize()){
        printf("Shader Failed to Initialize\n");
        return false;
    }

    //Add the vertex shader
    if(!m_shader->AddShader(GL_VERTEX_SHADER, vertexShader)){
        printf("Vertex Shader failed to Initialize\n");
        return false;
    }

    //Add the fragment shader
    if(!m_shader->AddShader(GL_FRAGMENT_SHADER, fragmentShader)){
        printf("Fragment Shader failed to Initialize\n");
        return false;
    }

    //Connect the program
    if(!m_shader->Finalize()){
        printf("Program to Finalize\n");
        return false;
    }

    //Locate the projection matrix in the shader
    m_projectionMatrix = m_shader->GetUniformLocation("Projection");
    if (m_projectionMatrix == INVALID_UNIFORM_LOCATION){
        printf("m_projectionMatrix not found\n");
        return false;
    }

    m_viewMatrix = m_shader->GetUniformLocation("View");
    if (m_viewMatrix == INVALID_UNIFORM_LOCATION){
        printf("m_viewMatrix not found\n");
        return false;
    }

    //Locate the model view matrix in the shader
    m_modelMatrix = m_shader->GetUniformLocation("Model");
    if (m_modelMatrix == INVALID_UNIFORM_LOCATION){
        printf("m_modelMatrix not found\n");
        return false;
    }

    m_ambientProduct = m_shader->GetUniformLocation("AmbientProduct");
    if (m_ambientProduct == INVALID_UNIFORM_LOCATION){
        printf("m_ambientProduct not found. Are you using a non-lighting shader?\n");
        return false;
    }

    m_diffuseProduct = m_shader->GetUniformLocation("DiffuseProduct");
    if (m_diffuseProduct == INVALID_UNIFORM_LOCATION){
        printf("m_diffuseProduct not found. Are you using a non-lighting shader?\n");
        return false;
    }

    m_specularProduct = m_shader->GetUniformLocation("SpecularProduct");
    if (m_specularProduct == INVALID_UNIFORM_LOCATION){
        printf("m_specularProduct not found. Are you using a non-lighting shader?\n");
        return false;
    }

    m_lightPosition = m_shader->GetUniformLocation("LightPosition");
    if (m_lightPosition == INVALID_UNIFORM_LOCATION){
        printf("m_lightPosition not found. Are you using a non-lighting shader?\n");
        return false;
    }

    m_shininess = m_shader->GetUniformLocation("Shininess");
    if (m_shininess == INVALID_UNIFORM_LOCATION){
        printf("m_shininess not found. Are you using a non-lighting shader?\n");
        return false;
    }

    m_spotPosition = m_shader->GetUniformLocation("SpotPos");
    if (m_spotPosition == INVALID_UNIFORM_LOCATION){
        printf("m_spotPosition not found. Are you using a non-lighting shader?\n");
        return false;
    }

    m_spotDirection = m_shader->GetUniformLocation("SpotDir");
    if (m_spotDirection == INVALID_UNIFORM_LOCATION){
        printf("m_spotDirection not found. Are you using a non-lighting shader?\n");
        return false;
    }

    m_spotCutoff = m_shader->GetUniformLocation("SpotCutOff");
    if (m_spotCutoff == INVALID_UNIFORM_LOCATION){
        printf("m_spotCutoff not found. Are you using a non-lighting shader?\n");
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
    for(unsigned int i = 0; i < objects.size(); ++i){
        objects[i]->Update(dt);
    }
}

//Renders a single image to the screen
void Graphics::Render(){
    //clear the screen
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Start the correct program
    m_shader->Enable();

    // Send in the projection to the shader
    glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection()));
    glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView()));
    glUniform4fv(m_ambientProduct, 1, glm::value_ptr(glm::vec4(0.25, 0.25, 0.25, 1)));
    glUniform4fv(m_diffuseProduct, 1, glm::value_ptr(glm::vec4(0.8, 0.8, 0.8, 1)));
    glUniform4fv(m_specularProduct, 1, glm::value_ptr(glm::vec4(0.75, 0.75, 0.75, 1)));
    glUniform4fv(m_lightPosition, 1, glm::value_ptr(glm::vec4(10, 10, 0, 1)));
    glUniform1f(m_shininess, 324);

    glUniform3fv(m_spotPosition, 1, glm::value_ptr(glm::vec3(0, 20, 0)));
    glUniform3fv(m_spotDirection, 1, glm::value_ptr(glm::vec3(0,-1, 0)));
    float cutOff = glm::cos(glm::radians(2.0f));
    glUniform1f(m_spotCutoff, cutOff);

    // Render the objects
    for(unsigned int i = 0; i < objects.size(); ++i){
        glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(objects[i]->GetModel()));
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
        } else{
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
