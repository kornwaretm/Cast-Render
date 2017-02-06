#ifndef SCREEN_SHADOW_CB
#define SCREEN_SHADOW_CB

#include <irrlicht.h>
#define max_light 10


using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

class ScreenShadowCallback : public IShaderConstantSetCallBack
{
public:
    dimension2d<f32> resolution;
    matrix4 cameraMatrix;
    f32 cameraFoV;
    vector3df cameraPosition;
    virtual void OnSetConstants(IMaterialRendererServices * services, s32 userData)
    {

        for(int i = 0 ; i < max_light ; i++)
        {
            if(i < lightCount)
            {
                radius[i] = radiuses[i];

                int id = i * 3;
                lightPos[id    ] = lamps[i].X;
                lightPos[id + 1] = lamps[i].Y;
                lightPos[id + 2] = lamps[i].Z;
            }
            else
            {
                int id = i * 3;
                lightPos[id    ] = 0.0f;
                lightPos[id + 1] = 0.0f;
                lightPos[id + 2] = 0.0f;

                radius[i] = 0.0f;
            }
        }
        s32 constid;
        constid = services->getPixelShaderConstantID("radius[0]");
        if(!services->setPixelShaderConstant(constid, radius, max_light))
        {
            printf("failed light radius");
        }

        constid = services->getPixelShaderConstantID("lightPos[0]");
        if(!services->setPixelShaderConstant(constid, lightPos, max_light * 3))
        {
            printf("failed light position");
        }

        services->setPixelShaderConstant("resolution", (f32*)&resolution, 2);
        f32 tex0 = 0;
        services->setPixelShaderConstant("depthReference", (f32*)&tex0, 0);

        services->setPixelShaderConstant("lightCount", (f32*)&lightCount, 0);
        /*constid = services->getPixelShaderConstantID("cameraMatrix");
        if(!services->setPixelShaderConstant(constid, (f32*)&cameraMatrix, 16))
        {
            printf("failed cameraMatrix");
        }*/
        position2d<f32> hurah(cameraFoV, lightCount);
        services->setPixelShaderConstant("count", (f32*)&hurah, 2);/*
        services->setPixelShaderConstant("cameraPosition", (f32*)&cameraPosition, 3);*/
    }

    void addLight(vector3df position, f32 rad)
    {
        if(lightCount >= max_light) return;

        lamps.push_back(position);
        radiuses.push_back(rad);
        lightCount = lamps.size();
    }

    void reset()
    {
        lightCount = 0;
        lamps.clear();
        radiuses.clear();
    }

private :
    f32 lightPos[max_light * 3];
    f32 radius[max_light];
    f32 lightCount;

    array<vector3df> lamps;
    array<f32> radiuses;
};


#endif // BASE_DEPTH_CB
