#ifndef BASEOBJECT_H
#define BASEOBJECT_H

#include <irrlicht.h>
#include "objects/objectConstants.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;



namespace cast
{
class CCastRender;
namespace volumetric_scene
{


class BaseObject
{
public:
    vector2df uvMul;
    vector2df uvAdd;

    BaseObject(CCastRender* manager);
    virtual ~BaseObject();

    virtual void update();
    virtual void update(bool force);
    virtual void updateTransformation();
    void addChild(BaseObject* object);
    virtual void drawBox();

    void setScale(const vector3df &scale);
    void setRotation(const vector3df &rotation);
    void setPosition(const vector3df &position);

    vector3df getScale()
    {
        return objectScale;
    }
    vector3df getRotation()
    {
        return objectRotation;
    }
    vector3df getPosition()
    {
        return objectPosition;
    }

    void setHeightMap(ITexture* tex)
    {
        heightMap = tex;
    }
    void setColorMap(ITexture* tex)
    {
        colorMap = tex;
    }
    ITexture* getHeightMap()
    {
        return heightMap;
    }
    ITexture* getColorMap()
    {
        return colorMap;
    }

    matrix4 getObjectInverseTransform()
    {
        return invObjectTransform;
    }
    matrix4 getObjectTransform()
    {
        return objectTransform;
    }
    vector3df getObjectScale()
    {
        return objectScale;
    }
    rect<f32> getBound()
    {
        return boundRect;
    }

    ObjectTypes getType()
    {
        return type;
    }

protected:
    ObjectTypes type;

private:
    // managerial
    CCastRender* manager;

    // textures
    ITexture* heightMap;
    ITexture* colorMap;

    // properties
    BaseObject* parent;
    irr::core::array<BaseObject*> childs;


    // transformation
    array<vector3df> points;
    rect<f32> boundRect;
    aabbox3d<f32> bbox;
    vector3df objectScale;
    vector3df objectRotation;
    vector3df objectPosition;
    bool updateTransform;
    matrix4 objectTransform;
    matrix4 invObjectTransform;

    //
    bool boundRectInsideRenderingBound();
};

}

}

#endif // BASEOBJECT_H
