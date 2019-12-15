#include "car.h"

Car::Car(std::string _name, BaseObject *parent_, std::string objectPath, struct PhysicsOptions physics)
: PhysicsObject(_name, parent_, objectPath, physics),
  speed(10.0f),
  turn(0.0f),
  angle(0.0f),
  speedChange(0.0f)
{
}

void Car::DerivedUpdate(float dt)
{
    if(rigidbody != NULL && !rigidbody->isStaticObject())
    {
        rigidbody->activate(true);

        float angleRadians = M_PI / 180.0f * angle;
        float diff = turn * dt;
        angle += diff;
        //std::cout << speed << std::endl;
        glm::vec3 forward = glm::vec3(model * glm::vec4(0, 0, 1, 0));
        speed = btDot(rigidbody->getLinearVelocity(), btVector3(forward.x, 0, forward.z));

        if(speed == 0 && speedChange * dt >= 0)
        {
            speed = 0.000001f;
        }

        speed += speedChange * dt;
        //vel = glm::vec3(0, 0, speed);
        /*vel = glm::vec3(cos(angleRadians) * vel.x - sin(angleRadians) * vel.z,
                        0.0f,
                        sin(angleRadians) * vel.x + cos(angleRadians) * vel.z);

        */

        vel = glm::vec3(model * glm::vec4(0, 0, speed, 0));
        btVector3 applyVec = btVector3(0, 0, 0);
        if(vel.x != 0)
        {
            applyVec.setX(vel.x);
        }

        if(vel.z != 0)
        {
            applyVec.setZ(vel.z);
        }
        rigidbody->setLinearVelocity(btVector3(vel.x, 0, vel.z));
        rigidbody->setAngularVelocity(btVector3(0.0, -1.05 * diff, 0.0f));

        //std::cout << std::to_string(getPosition().x) << ", " << std::to_string(getPosition().y) << ", " << std::to_string(getPosition().z) << std::endl;


    } else
    {
        std::cerr << "Object \"" << name << "\" does not have a rigidbody (Movable::DerivedUpdate)" << std::endl;
        exit(1);
    }
}

void Car::KeyDown(SDL_Event event)
{
    const int acceleration = 5;
    if(event.key.keysym.sym == SDLK_UP)
    {
        speedChange = acceleration;
    }
    if(event.key.keysym.sym == SDLK_DOWN)
    {
        speedChange = -acceleration;
    }
    if(event.key.keysym.sym == SDLK_LEFT)
    {
        turn = -90;
    }
    if(event.key.keysym.sym == SDLK_RIGHT)
    {
        turn = 90;
    }
    if(event.key.keysym.sym == SDLK_SPACE)
    {
        btTransform tran;
        rigidbody->getMotionState()->getWorldTransform(tran);
        btVector3 pos = tran.getOrigin();
        std::cout << pos[0] << ", " << pos[1] << ", " << pos[2] << std::endl;
    }
}

void Car::KeyUp(SDL_Event event)
{
    if(event.key.keysym.sym == SDLK_LEFT)
    {
        turn = 0;
    }
    if(event.key.keysym.sym == SDLK_RIGHT)
    {
        turn = 0;
    }
    if(event.key.keysym.sym == SDLK_UP)
    {
        speedChange = 0;
    }
    if(event.key.keysym.sym == SDLK_DOWN)
    {
        speedChange = 0;
    }
}
