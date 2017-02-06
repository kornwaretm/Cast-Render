#ifndef CUBEOBJECT_H
#define CUBEOBJECT_H

#include "../BaseObject.h"

namespace cast
{

namespace volumetric_scene
{

class CubeObject : public BaseObject
{
    public:
        CubeObject(CCastRender* manager);
        virtual ~CubeObject();

    protected:

    private:
};

}

}

#endif // SPHEREOBJECT_H
