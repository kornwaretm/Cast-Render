#ifndef LIGHTING_CB
#define LIGHTING_CB

#define max_light 10

#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

class LightingCallback : public IShaderConstantSetCallBack
{
public:
    vector3df cameraPosition;
    SColorf ambientColor;

    dimension2d<f32> resolution;
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

                lightColor[id    ] = f32(colors[i].getRed())   / 255.0f;
                lightColor[id + 1] = f32(colors[i].getGreen()) / 255.0f;
                lightColor[id + 2] = f32(colors[i].getBlue())  / 255.0f;
            }
            else
            {
                int id = i * 3;
                lightPos[id    ] = 0.0f;
                lightPos[id + 1] = 0.0f;
                lightPos[id + 2] = 0.0f;

                lightColor[id    ] = 0.0f;
                lightColor[id + 1] = 0.0f;
                lightColor[id + 2] = 0.0f;

                radius[i] = 0.0f;
            }
        }
        s32 constid = services->getPixelShaderConstantID("lightColor[0]");
        if(!services->setPixelShaderConstant(constid, lightColor, max_light * 3))
        {
            printf("failed light color");
        }
        constid = services->getPixelShaderConstantID("radius[0]");
        if(!services->setPixelShaderConstant(constid, radius, max_light))
        {
            printf("failed light radius");
        }

        constid = services->getPixelShaderConstantID("lightPos[0]");
        if(!services->setPixelShaderConstant(constid, lightPos, max_light * 3))
        {
            printf("failed light radius");
        }

        // camera position
        services->setPixelShaderConstant("cameraPosition", (f32*)&cameraPosition, 3);

        services->setPixelShaderConstant("resolution", (f32*)&resolution, 2);
        f32 tex0 = 0;
        services->setPixelShaderConstant("colorDepth", (f32*)&tex0, 0);

        f32 tex1 = 1;
        services->setPixelShaderConstant("normalTex", (f32*)&tex1, 0);

        constid = services->getPixelShaderConstantID("lightCount");
        vector2d<f32> lct(lightCount, 0.0);
        if(!services->setPixelShaderConstant(constid, (f32*)&lct, 2))
        {
            printf("failed to set light count at id : %d, value %d \n", constid,  lightCount);
        }

        f32 debugBuffers = 0;
        services->setPixelShaderConstant("debugBuffers", (f32*)&debugBuffers, 1);
        services->setPixelShaderConstant("ambientColor", (f32*)&ambientColor, 4);

    }

    void addLight(vector3df position, SColor color, f32 rad)
    {
        if(lightCount >= max_light) return;

        lamps.push_back(position);
        colors.push_back(color);
        radiuses.push_back(rad);
        lightCount = lamps.size();
    }

    void reset()
    {
        lightCount = 0;
        lamps.clear();
        colors.clear();
        radiuses.clear();
    }

private :
    f32 lightPos[max_light * 3];
    f32 lightColor[max_light * 3];
    f32 radius[max_light];
    int lightCount;

    array<vector3df> lamps;
    array<SColor> colors;
    array<f32> radiuses;
};

#endif // BASE_DEPTH_CB
