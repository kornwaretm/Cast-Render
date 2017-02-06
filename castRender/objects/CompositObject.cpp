#include "../BaseObject.h"
#include "CompositObject.h"
#include "objectConstants.h"

using namespace cast;
using namespace volumetric_scene;

CompositObject::CompositObject(CCastRender* manager):BaseObject(manager)
{
    //ctor
    type = EOT_COMPOSITE;
    count  = 0;
}

CompositObject::~CompositObject()
{
    //dtor
}

void CompositObject::addObject(BaseObject* object)
{
    if(count < max_composit)
    {
        objects[count] = object;
        count ++;
    }
}

void CompositObject::drawBox()
{
    BaseObject::drawBox();
    for(int i = 0 ; i < count ; i++)
    {
        objects[i]->drawBox();
    }
}

void CompositObject::updateTransformation()
{
    BaseObject::updateTransformation();
    for(int i = 0 ; i < count ; i++)
    {
        objects[i]->updateTransformation();
    }
}

void CompositObject::updateCallback(DepthComposit* callback)
{
    callback->objectCount = count;
    for(int i = 0 ; i < count ; i++)
    {
        callback->objectPosition[i] = objects[i]->getPosition();
        callback->objectScale[i] = objects[i]->getScale();
        callback->objectQuaternion[i] = quaternion(objects[i]->getRotation() * irr::core::DEGTORAD);
        callback->uvscale[i] = objects[i]->uvMul;
        callback->uvadd[i] = objects[i]->uvAdd;
        callback->types[i] = (s32)objects[i]->getType();
    }
}

