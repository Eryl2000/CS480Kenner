#ifndef PLANET_H
#define PLANET_H

#include <vector>
#include "graphics_headers.h"
#include "baseobject.h"

class Planet : public BaseObject{
public:
    Planet(BaseObject *_parent, float _orbitRadius, float _orbitRate, float _rotateRate,
            glm::vec3 _scale = glm::vec3(1.0, 1.0, 1.0));
    void DerivedUpdate(float dt);

    virtual void MouseDown(SDL_Event event);
    virtual void MouseUp(SDL_Event event);
    virtual void KeyDown(SDL_Event event);
    virtual void KeyUp(SDL_Event event);
    virtual void MouseWheel(SDL_Event event);

    //Turning rate in rev/sec
	float rotateRate;

	//Orbit rate in rev/sec
    float orbitRate;

	//Orbit radius
    float orbitRadius;

	//Current angle the cube makes with its center of rotation in radians
    float angleInOrbit;

	//-1,0,1 for whether the cube is spinning counterclockwise, stoppped, or clockwise
	int rotateDirection;

	//-1,0,1 for whether the cube is orbiting counterclockwise, stoppped, or clockwise
	int orbitDirection;

private:

};

#endif /* PLANET_H */
