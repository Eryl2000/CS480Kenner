#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "graphics_headers.h"

class Object
{
  public:
    Object();
    ~Object();
    void Update(unsigned int dt);
    void Render();

    glm::mat4 GetModel();

	/*State variables*/
	//Turning rate in rev/sec
	float turningRate;

	//Orbit rate in rev/sec
    float orbitRate;

	//Orbit radius
    float radius;

	//Current angle of the cube in radians
    float angle;

	//Current angle the cube makes with its center of rotation in radians
    float angleInOrbit;

	//-1,0,1 for whether the cube is spinning counterclockwise, stoppped, or clockwise
	int rotateDirection;

	//-1,0,1 for whether the cube is orbiting counterclockwise, stoppped, or clockwise
	int orbitDirection;

  private:
    glm::mat4 model;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;
};

#endif /* OBJECT_H */
