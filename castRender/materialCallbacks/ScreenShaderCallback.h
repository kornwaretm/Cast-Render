#ifndef SCREEN_CB
#define SCREEN_CB

#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

class ScreenShaderCallback : public IShaderConstantSetCallBack
{
public:
    dimension2d<f32> resolution;
    SColorf fogColor;

    virtual void OnSetConstants(IMaterialRendererServices * services, s32 userData)
    {
        // depth map
        f32 tex3 = 0;
        services->setPixelShaderConstant("depthBuffer", (f32*)&tex3, 1);
        // color map
        f32 tex1 = 1;
        services->setPixelShaderConstant("screenBuffer", (f32*)&tex1, 1);
        // shadow map
        f32 tex2 = 2;
        services->setPixelShaderConstant("shadowBuffer", (f32*)&tex2, 1);



        f32 sampleCount = 4.0f;
        services->setPixelShaderConstant("sampleCount", (f32*)&sampleCount, 1);

        services->setPixelShaderConstant("resolution", (f32*)&resolution, 2);
        services->setPixelShaderConstant("fogColor", (f32*)&fogColor, 4);
    }
};

#endif // BASE_DEPTH_CB
