#ifndef SPHEREOBJECT_H
#define SPHEREOBJECT_H

#include "../BaseObject.h"

namespace cast
{

namespace volumetric_scene
{

class RTSWorld : public BaseObject
{
public:
    RTSWorld(CCastRender* manager);
    virtual ~RTSWorld();

protected:

private:
};

}

}

#endif // SPHEREOBJECT_H
