#ifndef PLANET_H
#define PLANET_H

#include <vector>
#include "graphics_headers.h"
#include "baseobject.h"

const int simSpeed = 1e2;
const long scaleMod = 1e7;
const long distMod = 1e10;

class Planet : public BaseObject{
public:
    Planet(std::string _name,
           BaseObject *_parent,
           Planet * sun,
           Planet * previousPlanet,
           float _orbitRadius,
           bool _orbitScaled,
           float _orbitRate,
           float _rotateRate,
           bool _hasRings,
           glm::vec3 _scale = glm::vec3(1.0, 1.0, 1.0));
    void DerivedUpdate(float dt);

    virtual void MouseDown(SDL_Event event);
    virtual void MouseUp(SDL_Event event);
    virtual void KeyDown(SDL_Event event);
    virtual void KeyUp(SDL_Event event);
    virtual void MouseWheel(SDL_Event event);

    float GetRadius();

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

    bool hasRings;

    float scaledRadius;
    float normalizedRadius;

    // normalized = 0, scaled = 1
    float radiusParam;
    float orbitParamVel;
    bool orbitScaled;

};

struct PlanetInfo
{
    std::string name;
    // meters
    long diameter;

    // hours
    long rotationalPeriod;

    // meters
    long distFromSun;

    // hours
    long orbitPeriod;

    int numMoon;
    bool hasRings;

    Planet * FromInfo(BaseObject * parent, Planet * sun, Planet * previousPlanet);
};

#endif /* PLANET_H */
