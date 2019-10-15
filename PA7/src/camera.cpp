#include "camera.h"
#include <iostream>

Camera::Camera() : BaseObject(std::string("Camera"), NULL, std::string(""), true){
    rotationVelocity = glm::vec3(0.0, 0.0, 0.0);
    positionVelocity = glm::vec3(0.0, 0.0, 0.0);

    position = glm::vec3(0.0, 10.0, -16.0);
    forward = glm::vec3(0.0, 0.0, 1.0);
}

bool Camera::Initialize(int w, int h){
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
    //Make the camera work forward in time when time is reversed
    dt = dt < 0 ? -dt : dt;
    eulerAngle += rotationVelocity * dt;
    const float epsilon = 0.01;
    if(eulerAngle.x > M_PI / 2.0 - epsilon){
        eulerAngle.x = M_PI / 2.0 - epsilon;
    } else if(eulerAngle.x < -M_PI / 2.0 + epsilon){
        eulerAngle.x = -M_PI / 2.0 + epsilon;
    }

    glm::vec2 rotateForward = rotateVector(eulerAngle.y, glm::vec2(0.0, 1.0));
    forward = glm::vec3(rotateForward.x, sin(eulerAngle.x), rotateForward.y);

    glm::vec2 rotatedY = rotateVector(eulerAngle.y, glm::vec2(positionVelocity.x, positionVelocity.z));
    position += glm::vec3(rotatedY.x, positionVelocity.y, rotatedY.y) * dt;

    view = glm::lookAt( position, //Eye Position
                        position + forward, //Focus point
                        glm::vec3(0.0, 1, 0.0)); //Positive Y is up
    //std::cout << forward[0] << ", " << forward[1] << ", " << forward[2] << std::endl;
}

glm::vec2 Camera::rotateVector(float angleRadians, glm::vec2 original) const{
    return glm::vec2(cos(angleRadians) * original.x - sin(angleRadians) * original.y,
                     sin(angleRadians) * original.x + cos(angleRadians) * original.y);
}

void Camera::MouseDown(SDL_Event event){

}

void Camera::MouseUp(SDL_Event event){

}

void Camera::KeyDown(SDL_Event event){
    switch(event.key.keysym.sym){
        case SDLK_w:
            positionVelocity.z = 3;
            break;
        case SDLK_s:
            positionVelocity.z = -3;
            break;
        case SDLK_a:
            positionVelocity.x = 3;
            break;
        case SDLK_d:
            positionVelocity.x = -3;
            break;
        case SDLK_q:
            positionVelocity.y = -3;
            break;
        case SDLK_e:
            positionVelocity.y = 3;
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
        default:
            break;
    }
}

void Camera::MouseWheel(SDL_Event event){

}
