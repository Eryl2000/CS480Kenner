#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <string>
#include "graphics_headers.h"
#include "baseobject.h"

class Object : public BaseObject
{
  public:
    Object(BaseObject *parent_, std::string objectPath);
    void DerivedUpdate(float dt);

  private:

};

#endif /* OBJECT_H */
