#include "../BaseObject.h"
#include "SphereObject.h"
#include "objectConstants.h"

using namespace cast;
using namespace volumetric_scene;

SphereObject::SphereObject(CCastRender* manager):BaseObject(manager)
{
    //ctor
    type = EOT_SPHERE;
}

SphereObject::~SphereObject()
{
    //dtor
}
