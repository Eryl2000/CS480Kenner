#include "object.h"

Object::Object(BaseObject *parent_, std::string objectPath)
    : BaseObject(parent_, objectPath){
}

void Object::DerivedUpdate(float dt){
    static float angle = 0;
    angle += dt * 2 * M_PI * 0.2;
    model = glm::rotate(model, angle, glm::vec3(1.0, 1.0, 0.2));
}
