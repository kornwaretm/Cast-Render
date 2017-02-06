#include "../BaseObject.h"
#include "CilinderObject.h"
#include "objectConstants.h"

using namespace cast;
using namespace volumetric_scene;

CilinderObject::CilinderObject(CCastRender* manager):BaseObject(manager)
{
    //ctor
    type = EOT_CILINDER;
}

CilinderObject::~CilinderObject()
{
    //dtor
}
