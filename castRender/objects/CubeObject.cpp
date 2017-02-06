#include "../BaseObject.h"
#include "CubeObject.h"
#include "objectConstants.h"

using namespace cast;
using namespace volumetric_scene;

CubeObject::CubeObject(CCastRender* manager):BaseObject(manager)
{
    //ctor
    type = EOT_CUBE;
}

CubeObject::~CubeObject()
{
    //dtor
}
