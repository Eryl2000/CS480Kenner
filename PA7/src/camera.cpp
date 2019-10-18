#include "camera.h"
#include <iostream>

Camera::Camera(Engine* _engine) : BaseObject(std::string("Camera"), NULL, std::string(""), true){
    rotationVelocity = glm::vec3(0.0, 0.0, 0.0);
    positionVelocity = glm::vec3(0.0, 0.0, 0.0);

    position = glm::vec3(0.0, 10.0, -200.0);
    forward = glm::vec3(0.0, 0.0, 1.0);
    speedUp = 1;
    engine = _engine;
}

bool Camera::Initialize(int w, int h){
    width = w;
    height = h;
    view = glm::lookAt( position, //Eye Position
                        position + glm::vec3(0.0, 0.0, 10.0), //Focus point
                        glm::vec3(0.0, 1.0, 0.0)); //Positive Y is up

    projection = glm::perspective(45.0f, //the FoV
                                 float(w)/float(h), //Aspect Ratio, so Circles stay Circular
                                 0.01f, //Distance to the near plane, normally a small value like this
                                 1000.0f); //Distance to the far plane,
    eulerAngle.x = -M_PI / 180 * 20.0;
    eulerAngle.y = -M_PI / 180 * 34.0;
    return true;
}

glm::mat4 Camera::GetProjection(){
    return projection;
}

glm::mat4 Camera::GetView(){
    return view;
}

void Camera::DerivedUpdate(float dt){
    dt = engine->dt;
    eulerAngle += rotationVelocity * dt;
    const float epsilon = 0.01;
    if(eulerAngle.x > M_PI / 2.0 - epsilon){
        eulerAngle.x = M_PI / 2.0 - epsilon;
    } else if(eulerAngle.x < -M_PI / 2.0 + epsilon){
        eulerAngle.x = -M_PI / 2.0 + epsilon;
    }

    //Look in the correct direction
    glm::vec2 rotateForward = rotateVector(eulerAngle.y, glm::vec2(0.0, 1.0));
    forward = glm::vec3(rotateForward.x, sin(eulerAngle.x), rotateForward.y);

    //Updates the position based on the velocity
    glm::vec2 rotatedPositionVelocity = rotateVector(eulerAngle.y, glm::vec2(positionVelocity.x, positionVelocity.z));
    position += glm::vec3(rotatedPositionVelocity.x, positionVelocity.y, rotatedPositionVelocity.y) * speedUp * dt;

    //Looks in the correct direction from the correct location
    view = glm::lookAt(position, //Eye Position
                       position + forward, //Focus point
                       glm::vec3(0.0, 1, 0.0)); //Positive Y is up
    rotationVelocity.y = 0;
    rotationVelocity.x = 0;
}

/*
 * Rotates a vector by a given angle
 */
glm::vec2 Camera::rotateVector(float angleRadians, glm::vec2 original) const{
    return glm::vec2(cos(angleRadians) * original.x - sin(angleRadians) * original.y,
                     sin(angleRadians) * original.x + cos(angleRadians) * original.y);
}

void Camera::MouseDown(SDL_Event event){

}

void Camera::MouseUp(SDL_Event event){

}

void Camera::KeyDown(SDL_Event event){
    const int velocity = 50;
    const int speedIncrease = 10;
    switch(event.key.keysym.sym){
        case SDLK_w:
            positionVelocity.z = velocity;
            break;
        case SDLK_s:
            positionVelocity.z = -velocity;
            break;
        case SDLK_a:
            positionVelocity.x = velocity;
            break;
        case SDLK_d:
            positionVelocity.x = -velocity;
            break;
        case SDLK_q:
            positionVelocity.y = -velocity;
            break;
        case SDLK_e:
            positionVelocity.y = velocity;
            break;
        case SDLK_LEFT:
            rotationVelocity.y = -120 / 180.0 * M_PI;
            break;
        case SDLK_RIGHT:
            rotationVelocity.y = 120 / 180.0 * M_PI;
            break;
        case SDLK_UP:
            rotationVelocity.x = 120 / 180.0 * M_PI;
            break;
        case SDLK_DOWN:
            rotationVelocity.x = -120 / 180.0 * M_PI;
            break;
        case SDLK_LSHIFT:
            speedUp = speedIncrease;
            break;
        default:
            break;
    }
}

void Camera::KeyUp(SDL_Event event){
    switch(event.key.keysym.sym){
        case SDLK_w:
            positionVelocity.z = 0.0;
            break;
        case SDLK_s:
            positionVelocity.z = 0.0;
            break;
        case SDLK_a:
            positionVelocity.x = 0.0;
            break;
        case SDLK_d:
            positionVelocity.x = 0.0;
            break;
        case SDLK_q:
            positionVelocity.y = 0.0;
            break;
        case SDLK_e:
            positionVelocity.y = 0.0;
            break;
        case SDLK_LEFT:
            rotationVelocity.y = 0.0;
            break;
        case SDLK_RIGHT:
            rotationVelocity.y = 0.0;
            break;
        case SDLK_UP:
            rotationVelocity.x = 0.0;
            break;
        case SDLK_DOWN:
            rotationVelocity.x = 0.0;
            break;
        case SDLK_LSHIFT:
            speedUp = 1;
            break;
        default:
            break;
    }
}

void Camera::MouseWheel(SDL_Event event){

}

void Camera::MouseMotion(SDL_Event event){
    motionEvent = true;
    rotationVelocity.y = -120 / 1800.0 * event.motion.xrel;
    rotationVelocity.x = 120 / 1800.0 * event.motion.yrel;
}
