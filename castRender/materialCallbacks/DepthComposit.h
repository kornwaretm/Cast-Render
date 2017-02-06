#ifndef COMPOSIT_DEPTH_CB
#define COMPOSIT_DEPTH_CB

#include <irrlicht.h>
#include "../../config.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

class DepthComposit : public IShaderConstantSetCallBack
{
public:
    dimension2d<f32> resolution; // screen resolution
    vector3df cameraPosition; // camera position
    matrix4 cameraMatrix; // camera view matrix
    f32 cameraFoV; // camera field of view

    f32 objectCount; // the current number of object registered for drawing
    vector3df objectScale[max_composit];
    vector3df objectPosition[max_composit];
    quaternion objectQuaternion[max_composit];
    vector2df uvscale[max_composit];
    vector2df uvadd[max_composit];
    s32 types[max_composit];


    virtual void OnSetConstants(IMaterialRendererServices * services, s32 userData)
    {
        for(int i = 0 ; i < max_composit ; i++)
        {
            if(i<objectCount)
            {
                int id = i * 3;
                objectScaleArray[id    ] = objectScale[i].X;
                objectScaleArray[id + 1] = objectScale[i].Y;
                objectScaleArray[id + 2] = objectScale[i].Z;

                objectPositionArray[id   ] = objectPosition[i].X;
                objectPositionArray[id + 1] = objectPosition[i].Y;
                objectPositionArray[id + 2] = objectPosition[i].Z;

                id = i * 4;
                objectQuaternionArray[id    ] = objectQuaternion[i].X;
                objectQuaternionArray[id + 1] = objectQuaternion[i].Y;
                objectQuaternionArray[id + 2] = objectQuaternion[i].Z;
                objectQuaternionArray[id + 3] = objectQuaternion[i].W;

                id = i * 2;
                uvscaleArray[id] = uvscale[i].X;
                uvscaleArray[id + 1] = uvscale[i].Y;

                uvaddArray[id] = uvadd[i].X;
                uvaddArray[id + 1] = uvadd[i].Y;

            }
            else
            {
                types[i] = 0;
                int id = i * 3;
                objectScaleArray[id    ] = 0;
                objectScaleArray[id + 1] = 0;
                objectScaleArray[id + 2] = 0;

                objectPositionArray[id   ] = 0;
                objectPositionArray[id + 1] = 0;
                objectPositionArray[id + 2] = 0;

                id = i * 4;
                objectQuaternionArray[id    ] = 0;
                objectQuaternionArray[id + 1] = 0;
                objectQuaternionArray[id + 2] = 0;
                objectQuaternionArray[id + 3] = 0;

                id = i * 2;
                uvscaleArray[id] = 0;
                uvscaleArray[id + 1] = 0;

                uvaddArray[id] = 0;
                uvaddArray[id + 1] = 0;
            }
        }

        services->setPixelShaderConstant("resolution", (f32*)&resolution, 2);

        services->setPixelShaderConstant("cameraFoV", (f32*)&cameraFoV, 1);
        services->setPixelShaderConstant("cameraPosition", (f32*)&cameraPosition, 3);
        services->setPixelShaderConstant("cameraMatrix", (f32*)&cameraMatrix, 16);

        s32 constid;
        services->setPixelShaderConstant("objectCount", (f32*)&objectCount, 1);

        constid = services->getPixelShaderConstantID("objectType[0]");
        if(!services->setPixelShaderConstant(constid, types, max_composit))
        {
            printf("objectType failed \n");
        }

        constid = services->getPixelShaderConstantID("objectScale[0]");
        if(!services->setPixelShaderConstant(constid, objectScaleArray, 3 * max_composit))
        {
            printf("shit1");
        }


        constid = services->getPixelShaderConstantID("objectPosition[0]");
        if(!services->setPixelShaderConstant(constid, objectPositionArray, 3 * max_composit))
        {
            printf("shit2");
        }

        constid = services->getPixelShaderConstantID("uvMultiplier[0]");
        if(!services->setPixelShaderConstant(constid, uvscaleArray, 2 * max_composit))
        {
            printf("shit3");
        }

        constid = services->getPixelShaderConstantID("uvShift[0]");
        if(!services->setPixelShaderConstant(constid, uvaddArray, 2 * max_composit))
        {
            printf("shit4");
        }

        constid = services->getPixelShaderConstantID("objectQuaternion[0]");
        if(!services->setPixelShaderConstant(constid, objectQuaternionArray, 4 * max_composit))
        {
            printf("objectQuaternion failed");
        }

        f32 depthReference = 0;
        services->setPixelShaderConstant("depthReference", (f32*)&depthReference, 1);

        // height map
        f32 tex0 = 1;
        services->setPixelShaderConstant("tex0", (f32*)&tex0, 1);

        // color map
        f32 tex1 = 2;
        services->setPixelShaderConstant("tex1", (f32*)&tex1, 1);
    }
private:
    f32 objectScaleArray[max_composit * 3];
    f32 objectPositionArray[max_composit * 3];
    f32 objectQuaternionArray[max_composit * 4];
    f32 uvscaleArray[max_composit * 2];
    f32 uvaddArray[max_composit * 2];
};

#endif // BASE_DEPTH_CB
