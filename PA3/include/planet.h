#ifndef PLANET_H
#define PLANET_H

#include <vector>
#include "graphics_headers.h"
#include "object.h"

class Planet : public Object
{
  public:
    Planet(Object *parent_, float orbitRadius_, float orbitRate_, float rotateRate_);
    void Update(unsigned int dt);
    void DerivedUpdate(float dt);

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
