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
    IsDay = true;
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
    m_pointLight = new PointLight("light", NULL, glm::vec4(50, 100, 25, 0), 0.4);

    m_camera = new Camera(engine);
    objects.push_back(m_camera);

    //car
    BaseObject *temp;
    PhysicsOptions car(true, ColliderType::CarCube, PhysicsType::Dynamic, 0, btVector3(-7.6, 0.3f, 0), 100);
    Car *temp_car = new Car(std::string("purple"), NULL, std::string("../obj/car.obj"), car);
    temp = temp_car;
    sphere = temp_car;
    objects.push_back(temp);
    dynamicsWorld->addRigidBody(temp->rigidbody, 1, 1);

    temp->rigidbody->setLinearFactor(btVector3(1, 0, 1));
    temp->rigidbody->setAngularFactor(btVector3(0, 1, 0));

    if(!m_camera->Initialize(width, height, temp_car)){
        printf("Camera Failed to Initialize\n");
        exit(1);
    }

    //racetrack
    PhysicsOptions track_options(true, ColliderType::Mesh, PhysicsType::Static, 0, btVector3(0, 0, 0));
    temp = new PhysicsObject(std::string("granite"), NULL, std::string("../obj/track_bottom.obj"), track_options);
    objects.push_back(temp);
    dynamicsWorld->addRigidBody(temp->rigidbody, 1, 1);

    temp = new PhysicsObject(std::string("Wood"), NULL, std::string("../obj/track_fence_inside.obj"), track_options);
    objects.push_back(temp);
    //dynamicsWorld->addRigidBody(temp->rigidbody, 1, 1);

    temp = new PhysicsObject(std::string("Wood"), NULL, std::string("../obj/track_fence_outside.obj"), track_options);
    objects.push_back(temp);

    temp = new PhysicsObject(std::string("grass"), NULL, std::string("../obj/grass.obj"), track_options);
    objects.push_back(temp);
    //dynamicsWorld->addRigidBody(temp->rigidbody, 1, 1);

    temp = new PhysicsObject(std::string("granite"), NULL, std::string("../obj/inside_fence_collider.obj"), track_options);
    objects.push_back(temp);
    dynamicsWorld->addRigidBody(temp->rigidbody, 1, 1);
    temp->doNotRender = true;

    temp = new PhysicsObject(std::string("granite"), NULL, std::string("../obj/outside_fence_collider.obj"), track_options);
    objects.push_back(temp);
    dynamicsWorld->addRigidBody(temp->rigidbody, 1, 1);
    temp->doNotRender = true;

    /*btVector3 conePositions[] = {btVector3(12, 0.15, 75),
                                 btVector3(16, 0.15, 75),
                                 btVector3(20, 0.15, 75),
                                 btVector3(24, 0.15, 75),
                                 btVector3(28, 0.15, 75)};*/
    std::vector<btVector3> conePositions = {btVector3(49, 0.15, 102),
                                            btVector3(144, 0.15, 135),
                                            btVector3(318, 0.15, 133),
                                            btVector3(556, 0.15, 179),
                                            btVector3(615, 0.15, 255),
                                            btVector3(635, 0.15, 373),
                                            btVector3(626, 0.15, 525),
                                            btVector3(688, 0.15, 767),
                                            btVector3(785, 0.15, 835),
                                            btVector3(873, 0.15, 829),
                                            btVector3(1001, 0.15, 807),
                                            btVector3(1172, 0.15, 813),
                                            btVector3(1251, 0.15, 816),
                                            btVector3(1326, 0.15, 820),
                                            btVector3(1467, 0.15, 770),
                                            btVector3(1558, 0.15, 628),
                                            btVector3(1664, 0.15, 556),
                                            btVector3(1729, 0.15, 551),
                                            btVector3(1905, 0.15, 580),
                                            btVector3(2047, 0.15, 605),
                                            btVector3(2145, 0.15, 572),
                                            btVector3(2251, 0.15, 478),
                                            btVector3(2269, 0.15, 394),
                                            btVector3(2242, 0.15, 108),
                                            btVector3(2192, 0.15, -27),
                                            btVector3(2101, 0.15, -194),
                                            btVector3(1985, 0.15, -429),
                                            btVector3(1975, 0.15, -657),
                                            btVector3(2028, 0.15, -820),
                                            btVector3(2150, 0.15, -1118),
                                            btVector3(2037, 0.15, -1257),
                                            btVector3(1951, 0.15, -1393),
                                            btVector3(1882, 0.15, -1567),
                                            btVector3(1888, 0.15, -1758),
                                            btVector3(2036, 0.15, -2006),
                                            btVector3(2090, 0.15, -2250),
                                            btVector3(1764, 0.15, -2323),
                                            btVector3(1341, 0.15, -2330),
                                            btVector3(931, 0.15, -2335),
                                            btVector3(660, 0.15, -2245),
                                            btVector3(405, 0.15, -2245),
                                            btVector3(241, 0.15, -1959),
                                            btVector3(309, 0.15, -1637),
                                            btVector3(341, 0.15, -1406),
                                            btVector3(266, 0.15, -1143),
                                            btVector3(296, 0.15, -1024),
                                            btVector3(394, 0.15, -739),
                                            btVector3(397, 0.15, -600),
                                            btVector3(412, 0.15, -398),
                                            btVector3(180, 0.15, -193),
                                            btVector3(41, 0.15, -85)};
    PhysicsOptions conePhysics(true, ColliderType::Cube, PhysicsType::Dynamic, 0, conePositions[0]);
    for(unsigned int i = 0; i < conePositions.size(); i++)
    {
        conePhysics.position = conePositions[i];
        Cone *c = new Cone(std::string("orange"), NULL, std::string("../obj/cone.obj"), conePhysics);
        cones[c->rigidbody] = c;
        temp = c;
        objects.push_back(c);
        dynamicsWorld->addRigidBody(temp->rigidbody, 1, 1);
        temp->AddChildren(objects);
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

    m_lightEnabled = m_current->GetUniformLocation("PointLightEnabled");
    if (m_lightEnabled == INVALID_UNIFORM_LOCATION){
        printf("m_lightEnabled not found. Are you using a non-lighting shader?\n");
        return false;
    }

    m_nightLighting = m_current->GetUniformLocation("NightAmbient");
    if (m_nightLighting == INVALID_UNIFORM_LOCATION){
        printf("m_nightLighting not found. Are you using a non-lighting shader?\n");
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

    const bool day = IsDay;
    const glm::vec3 sky = glm::vec3(0.89, 0.16, 0.09f);
    const glm::vec3 night = glm::vec3(0.0, 0.0, 0.1f);
    glm::vec3 skyBoxColor = day ? sky : night;

    glClearColor(skyBoxColor.x, skyBoxColor.y, skyBoxColor.z, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Start the correct program
    m_current->Enable();

    // Send in the projection to the shader
    glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection()));
    glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView()));
    glUniform4fv(m_lightPosition, 1, glm::value_ptr(m_pointLight->lightPosition));
    const glm::vec3 diffuseColor = 0.9f * glm::vec3(1, 1, 1);
    const float skyBoxDiffuse = 0.3;
    const glm::vec4 skyDiffuseColor = glm::vec4((1 - skyBoxDiffuse) * diffuseColor + skyBoxDiffuse * skyBoxColor, 1.0);
    glUniform3fv(m_diffuseColor, 1, glm::value_ptr(skyDiffuseColor));
    glUniform4fv(m_nightLighting, 1, glm::value_ptr(glm::vec4(0.15, 0.15, 0.15, 1)));
    glUniform1f(m_lightEnabled, day);

    // Get the position of the ball
    glm::vec3 spherePos = sphere->GetModel()[3];
    glm::vec3 carForward = sphere->GetModel()[2];
    glm::vec3 lightRot = glm::vec3(0, -0.25f, 0);
    //glm::vec3 * printVec = &carForward;
    //std::cout << "Car pos: " << printVec->x << ", " << printVec->y << ", " << printVec->z << std::endl;
    glm::vec4 lightPos = glm::vec4(0, 0.5f, -0.3f, 0);
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
        } else if(event.key.keysym.sym == SDLK_x) {
                IsDay = !IsDay;
        }else{
            for(unsigned int i = 0; i < objects.size(); ++i){
                objects[i]->KeyDown(event);
            }
        }
	} else if (event.type == SDL_KEYUP){
        if(false)
        {
            // nothing so far
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

// Deprecated: per vertex lighting doesn't have the correct uniform variables
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
