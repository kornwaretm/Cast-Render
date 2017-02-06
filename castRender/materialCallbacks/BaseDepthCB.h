#ifndef BASE_DEPTH_CB
#define BASE_DEPTH_CB

#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

class BaseDepthCB : public IShaderConstantSetCallBack
{
public:
    vector3df cameraPosition;
    vector3df objectScale;
    vector3df objectPosition;
    matrix4 cameraMatrix;
    matrix4 invObjectTransform;
    dimension2d<f32> resolution;
    vector2df uvscale;
    f32 cameraFoV;


    virtual void OnSetConstants(IMaterialRendererServices * services, s32 userData)
    {
        // camera position
        services->setPixelShaderConstant("cameraPosition", (f32*)&cameraPosition, 3);

        // object scale
        services->setPixelShaderConstant("objectScale", (f32*)&objectScale, 3);

        services->setPixelShaderConstant("objectPosition", (f32*)&objectPosition, 3);

        // object scale
        services->setPixelShaderConstant("uvMultiplier", (f32*)&uvscale, 2);

        // camera matrix
        services->setPixelShaderConstant("cameraMatrix", (f32*)&cameraMatrix, 16);

        // object inversed matrix
        services->setPixelShaderConstant("invObjectTransform", (f32*)&invObjectTransform, 16);

        // screen resolution
        services->setPixelShaderConstant("resolution", (f32*)&resolution, 2);

        services->setPixelShaderConstant("cameraFoV", (f32*)&cameraFoV, 1);

        f32 depthReference = 0;
        services->setPixelShaderConstant("depthReference", (f32*)&depthReference, 1);

        // height map
        f32 tex0 = 1;
        services->setPixelShaderConstant("tex0", (f32*)&tex0, 1);

        // color map
        f32 tex1 = 2;
        services->setPixelShaderConstant("tex1", (f32*)&tex1, 1);
    }
};

#endif // BASE_DEPTH_CB
