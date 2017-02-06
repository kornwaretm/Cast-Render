#ifndef CILINDEROBJECT_H
#define CILINDEROBJECT_H

#include "../BaseObject.h"

namespace cast
{

namespace volumetric_scene
{

class CilinderObject : public BaseObject
{
    public:
        CilinderObject(CCastRender* manager);
        virtual ~CilinderObject();

    protected:

    private:
};
}
}

#endif // SPHEREOBJECT_H
