#include "../BaseObject.h"
#include "ConeObject.h"
#include "objectConstants.h"

using namespace cast;
using namespace volumetric_scene;

ConeObject::ConeObject(CCastRender* manager):BaseObject(manager)
{
    //ctor
    type = EOT_CONE;
}

ConeObject::~ConeObject()
{
    //dtor
}
