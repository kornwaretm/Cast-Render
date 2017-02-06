#ifndef SPHEREOBJECT_H
#define SPHEREOBJECT_H

#include "../BaseObject.h"

namespace cast
{

namespace volumetric_scene
{

class SphereObject : public BaseObject
{
public:
    SphereObject(CCastRender* manager);
    virtual ~SphereObject();

protected:

private:
};

}

}

#endif // SPHEREOBJECT_H
