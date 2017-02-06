#ifndef CONEOBJECT_H
#define CONEOBJECT_H

#include "../BaseObject.h"

namespace cast
{

namespace volumetric_scene
{

class ConeObject : public BaseObject
{
    public:
        ConeObject(CCastRender* manager);
        virtual ~ConeObject();

    protected:

    private:
};

}

}

#endif // SPHEREOBJECT_H
