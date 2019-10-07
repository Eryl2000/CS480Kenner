#include "object.h"

Object::Object(std::string _name, BaseObject *parent_, std::string objectPath)
    : BaseObject(_name, parent_, objectPath){

    rotationVelocity = glm::vec3(0.0, 0.0, 0.0);
    positionVelocity = glm::vec3(0.0, 0.0, 0.0);
    scaleVelocity = glm::vec3(0.0, 0.0, 0.0);

    buttonHeld_control = buttonHeld_alt = false;
}

void Object::DerivedUpdate(float dt){
    position += positionVelocity * dt;
    eulerAngle.y += dt * 2 * M_PI * rotationVelocity.y;
    scale += scaleVelocity * dt;
}


void Object::MouseDown(SDL_Event event){

}

void Object::MouseUp(SDL_Event event){

}

void Object::KeyDown(SDL_Event event){
    switch(event.key.keysym.sym){
        case SDLK_a:
            positionVelocity.x = 1.0;
            break;
        case SDLK_d:
            positionVelocity.x = -1.0;
            break;
        case SDLK_w:
            positionVelocity.z = 1.0;
            break;
        case SDLK_s:
            positionVelocity.z = -1.0;
            break;
        case SDLK_q:
            positionVelocity.y = -1.0;
            break;
        case SDLK_e:
            positionVelocity.y = 1.0;
            break;
        case SDLK_LEFT:
            if(buttonHeld_control){
                rotationVelocity.x = 0.2;
            } else if(buttonHeld_alt){
                rotationVelocity.z = 0.2;
            } else{
                rotationVelocity.y = 0.2;
            }
            break;
        case SDLK_RIGHT:
            if(buttonHeld_control){
                rotationVelocity.x = -0.2;
            } else if(buttonHeld_alt){
                rotationVelocity.z = -0.2;
            } else{
                rotationVelocity.y = -0.2;
            }
            break;
        case SDLK_UP:
            scaleVelocity.x = scaleVelocity.y = scaleVelocity.z = 0.2;
            break;
        case SDLK_DOWN:
            scaleVelocity.x = scaleVelocity.y = scaleVelocity.z = -0.2;
            break;
        case SDLK_LCTRL:
        case SDLK_RCTRL:
            buttonHeld_control = true;
            break;
        case SDLK_LALT:
        case SDLK_RALT:
            buttonHeld_alt = true;
            break;
        default:
            break;
    }
}

void Object::KeyUp(SDL_Event event){
    switch(event.key.keysym.sym){
        case SDLK_a:
            positionVelocity.x = 0.0;
            break;
        case SDLK_d:
            positionVelocity.x = 0.0;
            break;
        case SDLK_w:
            positionVelocity.z = 0.0;
            break;
        case SDLK_s:
            positionVelocity.z = 0.0;
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
            scaleVelocity.x = scaleVelocity.y = scaleVelocity.z = 0.0;
            break;
        case SDLK_DOWN:
            scaleVelocity.x = scaleVelocity.y = scaleVelocity.z = 0.0;
            break;
        case SDLK_LCTRL:
        case SDLK_RCTRL:
            buttonHeld_control = false;
            break;
        case SDLK_LALT:
        case SDLK_RALT:
            buttonHeld_alt = false;
            break;
        default:
            break;
    }
}

void Object::MouseWheel(SDL_Event event){

}
