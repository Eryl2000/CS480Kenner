#include "object.h"

Object::Object(BaseObject *parent_, std::string objectPath)
    : BaseObject(parent_, objectPath){

    rotateRate = 0.0;
}

void Object::DerivedUpdate(float dt){
    angle += dt * 2 * M_PI * rotateRate;
    model = glm::rotate(model, angle, glm::vec3(0.0, 1.0, 0.0));
}


void Object::MouseDown(SDL_Event event){

}

void Object::MouseUp(SDL_Event event){

}

void Object::KeyDown(SDL_Event event){
    switch(event.key.keysym.sym){
        case SDLK_a:
            rotateRate = 0.5;
            break;
        case SDLK_d:
            rotateRate = -0.5;
            break;
        case SDLK_w:
            break;
        case SDLK_s:
            break;
        default:
            break;
    }
}

void Object::KeyUp(SDL_Event event){

}

void Object::MouseWheel(SDL_Event event){

}
