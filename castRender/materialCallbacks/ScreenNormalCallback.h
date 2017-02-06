#ifndef SCREEN_NORMAL_CB
#define SCREEN_NORMAL_CB

#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

class ScreenNormalShaderCallback : public IShaderConstantSetCallBack
{
public:
    dimension2d<f32> resolution;
    virtual void OnSetConstants(IMaterialRendererServices * services, s32 userData)
    {
        // color map
        f32 tex1 = 0;
        services->setPixelShaderConstant("screenBuffer", (f32*)&tex1, 1);
        services->setPixelShaderConstant("resolution", (f32*)&resolution, 2);
        f32 sampleCount = 1.0f;
        services->setPixelShaderConstant("sampleCount", (f32*)&sampleCount, 1);
    }
};

#endif // BASE_DEPTH_CB
