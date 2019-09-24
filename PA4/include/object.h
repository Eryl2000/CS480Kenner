#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "graphics_headers.h"
#include "baseobject.h"

class Object : public BaseObject
{
  public:
    Object(BaseObject *parent_);
    void Update(unsigned int dt);
    void DerivedUpdate(float dt);

  private:

};

#endif /* OBJECT_H */
