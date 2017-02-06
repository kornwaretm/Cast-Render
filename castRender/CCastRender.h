#ifndef CCASTRENDER_H
#define CCASTRENDER_H

#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;


class BaseDepthCB;
class ScreenShaderCallback;
class ScreenNormalShaderCallback;
class LightingCallback;
class DepthComposit;
class ScreenShadowCallback;

namespace cast
{
namespace volumetric_scene
{
class BaseObject;

class dinamicLight
{
    public:
    SColor color;
    vector3df position;
    f32 radius;
    bool castShadows;

    dinamicLight()
    {
        color.setRed(255);
        color.setGreen(255);
        color.setBlue(255);

        radius = 1.0f;
        castShadows = false;
    }
};
}


class CCastRender
{
public:
    // options
    bool enableShadow;

    // stats
    u32 drawCallCount;
    u32 objectCount;

    SColorf fogColor;
    SColorf ambientColor;

    CCastRender(IrrlichtDevice* device, dimension2d<u32> resolution);
    virtual ~CCastRender();

    void registerObject(volumetric_scene::BaseObject* object);
    void update();
    void addObject(volumetric_scene::BaseObject* newObject)
    {
        addObject(newObject, 0);
    }
    void addObject(volumetric_scene::BaseObject* newObject, volumetric_scene::BaseObject* parent);
    void addLamp(volumetric_scene::dinamicLight *light)
    {
        lights.push_back(light);
    }
    matrix4 getCameraMatrix()
    {
        return cameraMatrix;
    }
    matrix4 getInvCameraMatrix()
    {
        return invCameraMatrix;
    }
    matrix4 getProjectionMatrix()
    {
        return projectionMatrix;
    }
    vector3df getCameraPosition()
    {
        return cameraPosition;
    }
    vector3df getCameraTarget()
    {
        return cameraTarget;
    }
    vector3df getEyeVector()
    {
        vector3df dist = cameraTarget - cameraPosition;
        return dist.normalize();
    }
    f32 getCameraNear()
    {
        return cameraNear;
    }
    f32 getCameraFar()
    {
        return cameraFar;
    }
    void setCameraFoV(f32 FoV)
    {
        cameraFoV = FoV / 180.0f * irr::core::PI;
    }
    void setCameraFoVFloat(f32 FoV)
    {
        cameraFoV = FoV;
    }
    IVideoDriver* getDriver()
    {
        return driver;
    }
    dimension2d<f32> getResolution()
    {
        return resolution;
    }

protected:

private:
    IrrlichtDevice *device;
    IVideoDriver *driver;
    BaseDepthCB *baseShaderCallback;
    DepthComposit* compositCallback;
    ScreenShaderCallback *screenShaderCallback;
    ScreenNormalShaderCallback* sncb;
    LightingCallback* lcb;
    ScreenShadowCallback* sscb;

    // cameras
    f32 cameraNear;
    f32 cameraFar;
    f32 cameraFoV;
    vector3df cameraPosition;
    vector3df cameraTarget;
    matrix4 cameraMatrix;
    matrix4 projectionMatrix;
    matrix4 invCameraMatrix;
    dimension2d<f32> resolution;

    // screen full primitive
    S3DVertex Vertices[6];
    S3DVertex ScreenVertices[6];
    u16 indices[6];

    // material
    SMaterial lineMat;
    SMaterial screenMaterial;
    SMaterial screenNormalMaterial;
    SMaterial tracingMaterial;
    SMaterial tracingCilinderMaterial;
    SMaterial tracingSphereMaterial;
    SMaterial tracingCompositMaterial;
    SMaterial clearMaterial;
    SMaterial lightingMaterial;
    SMaterial shadowMaterial;
    s32 tracingSphereShader;
    s32 tracingShader;
    s32 tracingCilinderShader;
    s32 screenShader;
    s32 screenNormalShader;
    s32 clearShader;
    s32 lightingShader;
    s32 tracingCompositShader;
    s32 shadowShader;

    // volley buffer and rendering passes
    ITexture* volleyBufferA;
    ITexture* volleyBufferB;
    ITexture* currentBuffer;
    ITexture* normalPass;
    ITexture* lightPass;
    ITexture* ssaoPass;

    // objects
    volumetric_scene::BaseObject* root;
    array<volumetric_scene::BaseObject*> registeredObject;
    array<volumetric_scene::dinamicLight*> lights;

    void draw(volumetric_scene::BaseObject* object);
    void volleyBuffer();
    ITexture* getCurrentBuffer();
    ITexture* getCurrentDepthReference();
};

}

#endif // CCASTRENDER_H
