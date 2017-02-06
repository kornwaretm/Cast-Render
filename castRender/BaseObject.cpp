#include "CCastRender.h"
#include "BaseObject.h"

using namespace cast;
using namespace volumetric_scene;

BaseObject::BaseObject(CCastRender* manager)
{
    //ctor
    this->manager = manager;
    type = EOT_CUBE;
    parent = 0;
    objectScale = vector3df(1.0, 1.0, 1.0);
    uvMul = vector2df(1.0, 1.0);
    uvAdd = vector2df(0.0, 0.0);

    heightMap = 0;
    colorMap = 0;

    updateTransform = false;

    points.push_back(vector3df(-1, -1, 1));
    points.push_back(vector3df(1, -1, 1));
    points.push_back(vector3df(1, 1, 1));
    points.push_back(vector3df(-1, 1, 1));
    points.push_back(vector3df(-1, -1, -1));
    points.push_back(vector3df(1, -1, -1));
    points.push_back(vector3df(1, 1, -1));
    points.push_back(vector3df(-1, 1, -1));
}

BaseObject::~BaseObject()
{
    //dtor
}

void BaseObject::addChild(BaseObject* object)
{
    object->parent = this;
    childs.push_back(object);
}

void BaseObject::setScale(const vector3df &scale)
{
    this->objectScale = scale;
    updateTransform = true;
}
void BaseObject::setRotation(const vector3df &rotation)
{
    this->objectRotation = rotation;
    updateTransform = true;
}
void BaseObject::setPosition(const vector3df &position)
{
    this->objectPosition = position;
    updateTransform = true;
}

void BaseObject::update()
{
    update(false);
}

void BaseObject::updateTransformation()
{
    objectTransform.makeIdentity();
    objectTransform.setRotationDegrees(objectRotation);
    objectTransform.setTranslation(objectPosition);
    matrix4 scale; scale.setScale(objectScale);
    objectTransform *= scale;

    invObjectTransform.makeIdentity();
    invObjectTransform.setRotationDegrees(objectRotation);
}

void BaseObject::update(bool force)
{
    bool forceChilds = false;
    if(updateTransform || force)
    {
        updateTransformation();
        updateTransform = false;
        forceChilds = true;
    }
    // no rendering if no texture
    if(heightMap && colorMap)
    {
        // check frustrum clipping
        matrix4 view = manager->getCameraMatrix();
        matrix4 proj = manager->getProjectionMatrix();
        matrix4 mod  = objectTransform;

        vector3df eyeVec = manager->getEyeVector();
        vector3df nearPlanePos = manager->getCameraPosition() + eyeVec * manager->getCameraNear();
        plane3d<f32> cameraNear(nearPlanePos, eyeVec);
        vector3df farPlanePos = manager->getCameraPosition() + eyeVec * manager->getCameraFar();

        bool invisible = true;
        for(int i = 0 ; i < 8 ; i++)
        {
            vector3df vec = points[i];

            mod.transformVect(vec);
            if(invisible)
            {
                vector3df tp = vec;
                if(cameraNear.classifyPointRelation(tp) == ISREL3D_FRONT )invisible = false;
            }
            view.transformVect(vec);
            proj.transformVect(vec);

            vec.X /= vec.Z;
            vec.Y /= vec.Z;
            vec.X = (vec.X + 1.0f) * manager->getResolution().Width / 2.0f;
            vec.Y = (vec.Y + 1.0f) * manager->getResolution().Height / 2.0f;
            if(i==0)
            {
                boundRect.LowerRightCorner.X = vec.X;
                boundRect.LowerRightCorner.Y = vec.Y;
                boundRect.UpperLeftCorner.X = vec.X;
                boundRect.UpperLeftCorner.Y = vec.Y;
            }
            else
            {
                boundRect.addInternalPoint(vec.X, vec.Y);
            }
        }
        if(!invisible)
        if((boundRect.getWidth() >= 4.0f) && (boundRect.getHeight() >= 4.0f) && boundRectInsideRenderingBound())
            manager->registerObject(this);
    }

    for(int i = 0 ; i < childs.size() ; i++)
    {
        childs[i]->update(updateTransform || force);
    }
}

bool BaseObject::boundRectInsideRenderingBound()
{
    f32 minX = boundRect.UpperLeftCorner.X;
    f32 minY = boundRect.UpperLeftCorner.Y;
    f32 maxX = boundRect.LowerRightCorner.X;
    f32 maxY = boundRect.LowerRightCorner.Y;

    f32 rx = manager->getResolution().Width;
    f32 ry = manager->getResolution().Height;

    f32 x_1 = (minX > 0) ? minX : 0;
	f32 y_1 = (minY > 0) ? minY : 0;

	f32 x_2 = (maxX < rx) ? maxX : rx;
	f32 y_2 = (maxY < ry) ? maxY : ry;

	if (((x_2 - x_1) > 0) && ((y_2 - y_1) > 0))
	{
		return true;
	}
	return false;
}
void BaseObject::drawBox()
{
    IVideoDriver* driver = manager->getDriver();
    f32 resX = manager->getResolution().Width;
    f32 resY = manager->getResolution().Height;

    irr::core::array<vector3df> pos;
    pos.push_back(vector3df(-1, -1, 1));
    pos.push_back(vector3df( 1, -1, 1));
    pos.push_back(vector3df( 1,  1, 1));
    pos.push_back(vector3df(-1,  1, 1));

    pos.push_back(vector3df(-1, -1, -1));
    pos.push_back(vector3df( 1, -1, -1));
    pos.push_back(vector3df( 1,  1, -1));
    pos.push_back(vector3df(-1,  1, -1));

    matrix4 view = manager->getCameraMatrix();
    matrix4 proj = manager->getProjectionMatrix();
    u32 i;
    for(i = 0 ; i < pos.size() ; i++)
    {
        objectTransform.transformVect(pos[i]);
        view.transformVect(pos[i]);
        proj.transformVect(pos[i]);
        pos[i].X /= pos[i].Z;
        pos[i].Y /= pos[i].Z;
        pos[i].X = (pos[i].X + 1.0f) * resX / 2.0f;
        pos[i].Y = (pos[i].Y + 1.0f) * resY / 2.0f;
    }

    // draw volume cube
    for(i = 0 ; i < 4 ; i ++)
    {
        u32 id1 = i;
        u32 id2 = (i + 1) % 4;
        driver->draw2DLine(
            position2d<s32>(pos[id1].X,pos[id1].Y),
            position2d<s32>(pos[id2].X,pos[id2].Y),
            SColor(0xff0000ff));
    }

    for(i = 0 ; i < 4 ; i ++)
    {
        u32 id1 = i + 4;
        u32 id2 = (i + 1) % 4 + 4;
        driver->draw2DLine(
            position2d<s32>(pos[id1].X,pos[id1].Y),
            position2d<s32>(pos[id2].X,pos[id2].Y),
            SColor(0xff0000ff));
    }

    for(i = 0 ; i < 4 ; i ++)
    {
        u32 id2 = i + 4;
        driver->draw2DLine(
            position2d<s32>(pos[id2].X,pos[id2].Y),
            position2d<s32>(pos[i].X,pos[i].Y),
            SColor(0xff0000ff));
    }

    driver->draw2DRectangleOutline(
            rect<s32>(
                min_(boundRect.LowerRightCorner.X, resX),
                min_(boundRect.LowerRightCorner.Y, resY) - 1,
                max_(boundRect.UpperLeftCorner.X, 0.0f),
                max_(boundRect.UpperLeftCorner.Y, 0.0f)),
            SColor(0xffffffff));
}
