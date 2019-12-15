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

        //btTransform tran;
        //rigidbody->getMotionState()->getWorldTransform(tran);
        //btVector3 pos = tran.getOrigin();
        //std::cout << pos[0] << ", " << pos[1] << ", " << pos[2] << std::endl;

        float angleRadians = M_PI / 180.0f * angle;
        float diff = turn * dt;
        angle += diff;
        speed += speedChange * dt;
        vel = glm::vec3(0, 0, speed);
        vel = glm::vec3(cos(angleRadians) * vel.x - sin(angleRadians) * vel.z,
                        0.0f,
                        sin(angleRadians) * vel.x + cos(angleRadians) * vel.z);
        rigidbody->setLinearVelocity(btVector3(vel.x, vel.y, vel.z));
        rigidbody->setAngularVelocity(btVector3(0.0, -1.05 * diff, 0.0f));


    } else
    {
        std::cerr << "Object \"" << name << "\" does not have a rigidbody (Movable::DerivedUpdate)" << std::endl;
        exit(1);
    }
}

void Car::KeyDown(SDL_Event event)
{
    if(event.key.keysym.sym == SDLK_UP)
    {
        speedChange = 2;
    }
    if(event.key.keysym.sym == SDLK_DOWN)
    {
        speedChange = -2;
    }
    if(event.key.keysym.sym == SDLK_LEFT)
    {
        turn = -90;
    }
    if(event.key.keysym.sym == SDLK_RIGHT)
    {
        turn = 90;
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
