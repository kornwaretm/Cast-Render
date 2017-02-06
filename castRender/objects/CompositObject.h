#ifndef COMPOSITOBJECT_H
#define COMPOSITOBJECT_H

#include "../BaseObject.h"
#include "../materialCallbacks/DepthComposit.h"
#include "../../config.h"

namespace cast
{

namespace volumetric_scene
{

class CompositObject : public BaseObject
{
public:
    CompositObject(CCastRender* manager);
    virtual ~CompositObject();
    virtual void drawBox();
    virtual void updateTransformation();
    void addObject(BaseObject* object);
    void updateCallback(DepthComposit* callback);
    u32 size(){return count;}
protected:

private:
    u32 count;
    BaseObject* objects[max_composit];
};

}

}

#endif // SPHEREOBJECT_H
