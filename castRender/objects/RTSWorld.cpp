#include "../BaseObject.h"
#include "SphereObject.h"
#include "objectConstants.h"

using namespace cast;
using namespace volumetric_scene;

RTSWorld::RTSWorld(CCastRender* manager):BaseObject(manager)
{
    //ctor
    type = EOT_SPHERE;
}

RTSWorld::~RTSWorld()
{
    //dtor
}
