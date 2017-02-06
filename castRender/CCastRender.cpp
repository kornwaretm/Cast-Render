#include "CCastRender.h"
#include "BaseObject.h"
#include "objects/CompositObject.h"
#include "materialCallbacks/BaseDepthCB.h"
#include "materialCallbacks/DepthComposit.h"
#include "materialCallbacks/ScreenShaderCallback.h"
#include "materialCallbacks/ScreenNormalCallback.h"
#include "materialCallbacks/lightingCallBack.h"
#include "materialCallbacks/screenShadowCallback.h"

using namespace cast;
using namespace volumetric_scene;

CCastRender::CCastRender(IrrlichtDevice* device, dimension2d<u32> resolution)
{
    //ctor

    this->device = device;
    this->resolution = resolution;
    driver = device->getVideoDriver();

    // settings
    enableShadow = true;

    // back buffer for volley ops
    volleyBufferA = driver->addRenderTargetTexture(resolution, "volleyA", ECF_G32R32F  );
    volleyBufferB = driver->addRenderTargetTexture(resolution, "volleyB", ECF_G32R32F  );
    currentBuffer = volleyBufferB;
    normalPass    = driver->addRenderTargetTexture(resolution, "normal_pass", ECF_UNKNOWN);
    lightPass    = driver->addRenderTargetTexture(resolution, "light_pass", ECF_UNKNOWN);

    baseShaderCallback = new BaseDepthCB();
    baseShaderCallback->resolution = resolution;

    lineMat.Lighting = false;
    lineMat.ZWriteEnable = false;

    // tracing material setup
    IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();
    tracingShader = gpu->addHighLevelShaderMaterialFromFiles(
                        "./shaders/vert.glsl","main", EVST_VS_1_1,
                        "./shaders/cubeTracing_v1.glsl","main",EPST_PS_1_1,
                        baseShaderCallback, EMT_SOLID, 0, EGSL_DEFAULT);
    tracingMaterial.MaterialType = (E_MATERIAL_TYPE)tracingShader;
    tracingMaterial.Wireframe = false;
    tracingMaterial.Lighting = false;
    tracingMaterial.ZWriteEnable = false;
    tracingMaterial.BackfaceCulling = false;

    // setting up filtering
    tracingMaterial.setTexture(0, volleyBufferA); // reference
    tracingMaterial.setTexture(1, volleyBufferB);
    tracingMaterial.setTexture(2, normalPass);

    tracingMaterial.TextureLayer[0].AnisotropicFilter = 0;
    tracingMaterial.TextureLayer[0].BilinearFilter = false;
    tracingMaterial.TextureLayer[0].TrilinearFilter = false;

    tracingMaterial.TextureLayer[2].AnisotropicFilter = 0;
    tracingMaterial.TextureLayer[2].BilinearFilter = false;
    tracingMaterial.TextureLayer[2].TrilinearFilter = false;

    // tracing material setup
    compositCallback = new DepthComposit();
    compositCallback->resolution = resolution;
    tracingCompositShader = gpu->addHighLevelShaderMaterialFromFiles(
                                "./shaders/vert.glsl","main", EVST_VS_1_1,
                                "./shaders/compositTracing.glsl","main",EPST_PS_1_1,
                                compositCallback, EMT_SOLID, 0, EGSL_DEFAULT);
    tracingCompositMaterial.MaterialType = (E_MATERIAL_TYPE)tracingCompositShader;
    tracingCompositMaterial.Wireframe = false;
    tracingCompositMaterial.Lighting = false;
    tracingCompositMaterial.ZWriteEnable = false;
    tracingCompositMaterial.BackfaceCulling = false;

    // setting up filtering
    tracingCompositMaterial.TextureLayer[2].AnisotropicFilter = 0;
    tracingCompositMaterial.TextureLayer[2].BilinearFilter = false;
    tracingCompositMaterial.TextureLayer[2].TrilinearFilter = false;

    // tracing material setup
    tracingCilinderShader = gpu->addHighLevelShaderMaterialFromFiles(
                                "./shaders/vert.glsl","main", EVST_VS_1_1,
                                "./shaders/cilinderTracing.glsl","main",EPST_PS_1_1,
                                baseShaderCallback, EMT_SOLID, 0, EGSL_DEFAULT);
    tracingCilinderMaterial.MaterialType = (E_MATERIAL_TYPE)tracingCilinderShader;
    tracingCilinderMaterial.Wireframe = false;
    tracingCilinderMaterial.Lighting = false;
    tracingCilinderMaterial.ZWriteEnable = false;
    tracingCilinderMaterial.BackfaceCulling = false;

    // setting up filtering
    tracingCilinderMaterial.setTexture(0, volleyBufferA); // reference
    tracingCilinderMaterial.setTexture(1, volleyBufferB);
    tracingCilinderMaterial.setTexture(2, normalPass);

    tracingCilinderMaterial.TextureLayer[0].AnisotropicFilter = 0;
    tracingCilinderMaterial.TextureLayer[0].BilinearFilter = false;
    tracingCilinderMaterial.TextureLayer[0].TrilinearFilter = false;

    tracingCilinderMaterial.TextureLayer[2].AnisotropicFilter = 0;
    tracingCilinderMaterial.TextureLayer[2].BilinearFilter = false;
    tracingCilinderMaterial.TextureLayer[2].TrilinearFilter = false;

    // tracing material setup
    tracingSphereShader = gpu->addHighLevelShaderMaterialFromFiles(
                              "./shaders/vert.glsl","main", EVST_VS_1_1,
                              "./shaders/sphereTracing.glsl","main",EPST_PS_1_1,
                              baseShaderCallback, EMT_SOLID, 0, EGSL_DEFAULT);
    tracingSphereMaterial.MaterialType = (E_MATERIAL_TYPE)tracingSphereShader;
    tracingSphereMaterial.Wireframe = false;
    tracingSphereMaterial.Lighting = false;
    tracingSphereMaterial.ZWriteEnable = false;
    tracingSphereMaterial.BackfaceCulling = false;

    // setting up filtering
    tracingSphereMaterial.setTexture(0, volleyBufferA); // reference
    tracingSphereMaterial.setTexture(1, volleyBufferB);
    tracingSphereMaterial.setTexture(2, normalPass);

    tracingSphereMaterial.TextureLayer[0].AnisotropicFilter = 0;
    tracingSphereMaterial.TextureLayer[0].BilinearFilter = false;
    tracingSphereMaterial.TextureLayer[0].TrilinearFilter = false;

    tracingSphereMaterial.TextureLayer[2].AnisotropicFilter = 0;
    tracingSphereMaterial.TextureLayer[2].BilinearFilter = false;
    tracingSphereMaterial.TextureLayer[2].TrilinearFilter = false;

    // normal map material
    sncb = new ScreenNormalShaderCallback();
    sncb->resolution = resolution;
    screenNormalShader = gpu->addHighLevelShaderMaterialFromFiles(
                             "./shaders/vert.glsl","main", EVST_VS_1_1,
                             "./shaders/normal.glsl","main",EPST_PS_1_1,
                             sncb, EMT_SOLID, 0, EGSL_DEFAULT);
    screenNormalMaterial.MaterialType = (E_MATERIAL_TYPE)screenNormalShader;
    screenNormalMaterial.Wireframe = false;
    screenNormalMaterial.Lighting = false;
    screenNormalMaterial.ZWriteEnable = false;

    // lighting map material
    lcb = new LightingCallback();
    lcb->resolution = resolution;
    lightingShader = gpu->addHighLevelShaderMaterialFromFiles(
                         "./shaders/vertLight.glsl","main", EVST_VS_1_1,
                         "./shaders/lighting.glsl","main",EPST_PS_1_1,
                         lcb, EMT_SOLID, 0, EGSL_DEFAULT);
    lightingMaterial.MaterialType = (E_MATERIAL_TYPE)lightingShader;
    lightingMaterial.Wireframe = false;
    lightingMaterial.Lighting = false;
    lightingMaterial.ZWriteEnable = false;
    lightingMaterial.setTexture(0, volleyBufferA);
    lightingMaterial.setTexture(1, normalPass);

    // lighting map material
    sscb = new ScreenShadowCallback();
    sscb->resolution = resolution;
    shadowShader = gpu->addHighLevelShaderMaterialFromFiles(
                       "./shaders/vert.glsl","main", EVST_VS_1_1,
                       "./shaders/screenSpaceShadow.glsl","main",EPST_PS_1_1,
                       sscb, EMT_SOLID, 0, EGSL_DEFAULT);
    shadowMaterial.MaterialType = (E_MATERIAL_TYPE)shadowShader;
    shadowMaterial.Wireframe = false;
    shadowMaterial.Lighting = false;
    shadowMaterial.ZWriteEnable = false;
    shadowMaterial.setTexture(0, volleyBufferA);
    shadowMaterial.TextureLayer[0].AnisotropicFilter = 0;
    shadowMaterial.TextureLayer[0].BilinearFilter = false;
    shadowMaterial.TextureLayer[0].TrilinearFilter = false;

    // cleaning shader
    clearShader  = gpu->addHighLevelShaderMaterialFromFiles(
                       "./shaders/vert.glsl","main", EVST_VS_1_1,
                       "./shaders/clear.glsl","main",EPST_PS_1_1,
                       0, EMT_SOLID, 0, EGSL_DEFAULT);
    clearMaterial.MaterialType = (E_MATERIAL_TYPE)clearShader;
    clearMaterial.Wireframe = false;
    clearMaterial.Lighting = false;
    clearMaterial.ZWriteEnable = false;

    // full screen shader setup
    screenShaderCallback = new ScreenShaderCallback();
    screenShaderCallback->resolution = resolution;
    screenShader  = gpu->addHighLevelShaderMaterialFromFiles(
                        "./shaders/vert.glsl","main", EVST_VS_1_1,
                        "./shaders/screen.glsl","main",EPST_PS_1_1,
                        screenShaderCallback, EMT_SOLID, 0, EGSL_DEFAULT);

    screenMaterial.MaterialType = (E_MATERIAL_TYPE)screenShader;
    screenMaterial.Wireframe = false;
    screenMaterial.Lighting = false;
    screenMaterial.ZWriteEnable = false;


    Vertices[0] = S3DVertex(-1.0f,-1.0f,0.0f,0,0,1,SColor(0x0),-1.0f,1.0f);
    Vertices[1] = S3DVertex(-1.0f, 1.0f,0.0f,0,0,1,SColor(0x0),-1.0f,-1.0f);
    Vertices[2] = S3DVertex( 1.0f, 1.0f,0.0f,0,0,1,SColor(0x0),1.0f,-1.0f);
    Vertices[3] = S3DVertex( 1.0f,-1.0f,0.0f,0,0,1,SColor(0x0),1.0f,1.0f);
    Vertices[4] = S3DVertex(-1.0f,-1.0f,0.0f,0,0,1,SColor(0x0),-1.0f,1.0f);
    Vertices[5] = S3DVertex( 1.0f, 1.0f,0.0f,0,0,1,SColor(0x0),1.0f,-1.0f);

    ScreenVertices[0] = S3DVertex(-1.0f,-1.0f,0.0f,0,0,1,SColor(0x0),0.0f,0.0f);
    ScreenVertices[1] = S3DVertex(-1.0f, 1.0f,0.0f,0,0,1,SColor(0x0),0.0f,1.0f);
    ScreenVertices[2] = S3DVertex( 1.0f, 1.0f,0.0f,0,0,1,SColor(0x0),1.0f,1.0f);
    ScreenVertices[3] = S3DVertex( 1.0f,-1.0f,0.0f,0,0,1,SColor(0x0),1.0f,0.0f);
    ScreenVertices[4] = S3DVertex(-1.0f,-1.0f,0.0f,0,0,1,SColor(0x0),0.0f,0.0f);
    ScreenVertices[5] = S3DVertex( 1.0f, 1.0f,0.0f,0,0,1,SColor(0x0),1.0f,1.0f);

    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;
    indices[3] = 3;
    indices[4] = 4;
    indices[5] = 5;

    root = new BaseObject(this);
    cameraPosition = vector3df(0.0, -3.0f, 4.0f);
    cameraTarget = vector3df(0.0,0.0,0.0);
    cameraNear = 0.001;
    cameraFar  = 32.0f;
    cameraFoV = 60.0f / 180.0f * irr::core::PI;
}

CCastRender::~CCastRender()
{
    //dtor
}

void CCastRender::volleyBuffer()
{
    if(currentBuffer == volleyBufferA)
    {
        currentBuffer = volleyBufferB;
    }
    else
    {
        currentBuffer = volleyBufferA;
    }
}

ITexture* CCastRender::getCurrentBuffer()
{
    return currentBuffer;
}

ITexture* CCastRender::getCurrentDepthReference()
{
    return (currentBuffer == volleyBufferA) ? volleyBufferB : volleyBufferA;
}

void CCastRender::addObject(BaseObject* newObject, BaseObject* parent)
{
    if(parent)
    {
        parent->addChild(newObject);
    }
    else
    {
        root->addChild(newObject);
    }
}

void CCastRender::draw(BaseObject* object)
{
    // repair position
    rect<f32> r = object->getBound();
    f32 x1 = max_(r.LowerRightCorner.X, resolution.Width);
    f32 y1 = max_(r.LowerRightCorner.Y, resolution.Height);
    f32 x2 = min_(r.UpperLeftCorner.X, 0.0f);
    f32 y2 = min_(r.UpperLeftCorner.Y, 0.0f);
    x1 = (x1 * 2.0f / resolution.Width ) - 1.0f;
    y1 = (y1 * 2.0f / resolution.Height ) - 1.0f;
    x2 = (x2 * 2.0f / resolution.Width ) - 1.0f;
    y2 = (y2 * 2.0f / resolution.Height ) - 1.0f;

    Vertices[0].Pos.X = x1;
    Vertices[0].Pos.Y = -y1;
    Vertices[1].Pos.X = x1;
    Vertices[1].Pos.Y = -y2;
    Vertices[2].Pos.X = x2;
    Vertices[2].Pos.Y = -y2;
    Vertices[3].Pos.X = x2;
    Vertices[3].Pos.Y = -y1;
    Vertices[4].Pos.X = x1;
    Vertices[4].Pos.Y = -y1;
    Vertices[5].Pos.X = x2;
    Vertices[5].Pos.Y = -y2;

    Vertices[0].TCoords.X = x1;
    Vertices[0].TCoords.Y = y1;
    Vertices[1].TCoords.X = x1;
    Vertices[1].TCoords.Y = y2;
    Vertices[2].TCoords.X = x2;
    Vertices[2].TCoords.Y = y2;
    Vertices[3].TCoords.X = x2;
    Vertices[3].TCoords.Y = y1;
    Vertices[4].TCoords.X = x1;
    Vertices[4].TCoords.Y = y1;
    Vertices[5].TCoords.X = x2;
    Vertices[5].TCoords.Y = y2;

    switch(object->getType())
    {
    case(EOT_SPHERE):
    {
        driver->setMaterial(tracingSphereMaterial);
        tracingSphereMaterial.setTexture(0, getCurrentDepthReference());
        tracingSphereMaterial.setTexture(1, object->getHeightMap());
        tracingSphereMaterial.setTexture(2, object->getColorMap());
        baseShaderCallback->objectScale = object->getObjectScale();
        baseShaderCallback->invObjectTransform = object->getObjectInverseTransform();
        baseShaderCallback->uvscale = object->uvMul;
        baseShaderCallback->objectPosition = object->getPosition();
        break;
    }
    case(EOT_CILINDER):
    {
        driver->setMaterial(tracingCilinderMaterial);
        tracingCilinderMaterial.setTexture(0, getCurrentDepthReference());
        tracingCilinderMaterial.setTexture(1, object->getHeightMap());
        tracingCilinderMaterial.setTexture(2, object->getColorMap());
        baseShaderCallback->objectScale = object->getObjectScale();
        baseShaderCallback->invObjectTransform = object->getObjectInverseTransform();
        baseShaderCallback->uvscale = object->uvMul;
        baseShaderCallback->objectPosition = object->getPosition();
        break;
    }
    case(EOT_CUBE):
    {
        driver->setMaterial(tracingMaterial);
        tracingMaterial.setTexture(0, getCurrentDepthReference());
        tracingMaterial.setTexture(1, object->getHeightMap());
        tracingMaterial.setTexture(2, object->getColorMap());
        baseShaderCallback->objectScale = object->getObjectScale();
        baseShaderCallback->invObjectTransform = object->getObjectInverseTransform();
        baseShaderCallback->uvscale = object->uvMul;
        baseShaderCallback->objectPosition = object->getPosition();
        break;
    }
    case(EOT_COMPOSITE):
    {
        driver->setMaterial(tracingCompositMaterial);
        tracingCompositMaterial.setTexture(0, getCurrentDepthReference());
        tracingCompositMaterial.setTexture(1, object->getHeightMap());
        tracingCompositMaterial.setTexture(2, object->getColorMap());

        ((CompositObject*)object)->updateCallback(compositCallback);
        break;
    }
    default:
    {
        driver->setMaterial(tracingMaterial);
        tracingMaterial.setTexture(0, getCurrentDepthReference());
        tracingMaterial.setTexture(1, object->getHeightMap());
        tracingMaterial.setTexture(2, object->getColorMap());
        baseShaderCallback->objectScale = object->getObjectScale();
        baseShaderCallback->invObjectTransform = object->getObjectInverseTransform();
        baseShaderCallback->uvscale = object->uvMul;
        baseShaderCallback->objectPosition = object->getPosition();
        break;
    }
    }
    driver->drawIndexedTriangleList(&Vertices[0], 6, &indices[0], 2);
}

void CCastRender::registerObject(BaseObject* object)
{
    registeredObject.push_back(object);
}

void CCastRender::update()
{
    // build camera matrix
    ISceneManager* smgr = device->getSceneManager();
    ICameraSceneNode* cam = smgr->getActiveCamera();
    cameraPosition = cam->getPosition();
    cameraTarget = cam->getTarget();
    f32 time = f32(device->getTimer()->getTime() %10000) / 10000 * 6.28;
    cameraPosition.X = sin(time) * 16.0f;
    cameraPosition.Z = cos(time) * 16.0f;
    cameraPosition.Y = -4;

    //cameraTarget.X = 5 + sin(time) * 6.0f;
    //cameraTarget.Z = 5 + cos(time) * 6.0f;
    cameraTarget.Y = 0;

    projectionMatrix.makeIdentity();
    projectionMatrix.buildProjectionMatrixPerspectiveFovLH(cameraFoV, resolution.Width / resolution.Height, cameraNear, cameraFar);

    cameraMatrix.makeIdentity();
    cameraMatrix.buildCameraLookAtMatrixLH(cameraPosition, cameraTarget, vector3df(0.0f, 1.0f, 0.0f));
    invCameraMatrix = cameraMatrix;
    invCameraMatrix.makeInverse();

    baseShaderCallback->cameraMatrix = invCameraMatrix;
    baseShaderCallback->cameraPosition = cameraPosition;
    baseShaderCallback->cameraFoV = cameraFoV * 1.5f;

    compositCallback->cameraMatrix = invCameraMatrix;
    compositCallback->cameraPosition = cameraPosition;
    compositCallback->cameraFoV = cameraFoV * 1.5f;

    // register objects for rendering
    registeredObject.clear();
    root->update();

    // clear buffers
    driver->setRenderTarget(volleyBufferA, true,true, SColor(0x00000000));
    driver->setMaterial(clearMaterial);
    driver->drawIndexedTriangleList(&ScreenVertices[0], 6, &indices[0], 2);

    driver->setRenderTarget(volleyBufferB, true,true, SColor(0x00000000));
    driver->setMaterial(clearMaterial);
    driver->drawIndexedTriangleList(&ScreenVertices[0], 6, &indices[0], 2);

    // draw all object
    driver->setTransform(video::ETS_VIEW, core::matrix4());
    driver->setTransform(video::ETS_PROJECTION, core::matrix4());
    driver->setTransform(video::ETS_WORLD, core::matrix4());

    drawCallCount = 0;
    objectCount = 0;
    for(u32 i = 0 ; i < registeredObject.size() ; i++)
    {
        volleyBuffer();
        driver->setRenderTarget(currentBuffer, false,false);
        draw(registeredObject[i]);
        drawCallCount++;
        objectCount++;
        if(registeredObject[i]->getType() == EOT_COMPOSITE)
        {
            objectCount += ((CompositObject*)(registeredObject[i]))->size();
        }
    }

    // create normal map
    driver->setRenderTarget(normalPass, false,false);
    screenNormalMaterial.setTexture(0, currentBuffer);
    driver->setMaterial(screenNormalMaterial);
    driver->drawIndexedTriangleList(&ScreenVertices[0], 6, &indices[0], 2);

    // lighting pass
    lcb->reset();
    sscb->reset();
    for(int i = 0 ; i < lights.size() ; i++)
    {
        vector3df position = lights[i]->position;
        cameraMatrix.transformVect(position);
        projectionMatrix.transformVect(position);
        position.X /= position.Z;
        position.Y /= position.Z;
        position.X = (position.X + 1.0f);// * 1.01f;
        position.Y = 0.5 - position.Y;
        position.Z = 1.0f -  max_(min_(position.Z / 32.0f,1.0f),0.0f);
        lcb->addLight(position, lights[i]->color, lights[i]->radius);

        if(lights[i]->castShadows)sscb->addLight(position,lights[i]->radius );
    }
    lcb->cameraPosition = cameraPosition;
    lcb->ambientColor = ambientColor;

    driver->setRenderTarget(lightPass);
    lightingMaterial.setTexture(0, currentBuffer);
    driver->setMaterial(lightingMaterial);
    driver->drawIndexedTriangleList(&ScreenVertices[0], 6, &indices[0], 2);

    if(enableShadow)
    {
        driver->setRenderTarget(normalPass, false,false);
        shadowMaterial.setTexture(0, currentBuffer);

        sscb->cameraMatrix = invCameraMatrix;
        sscb->cameraFoV = cameraFoV;
        sscb->cameraPosition = cameraPosition;
        driver->setMaterial(shadowMaterial);
        driver->drawIndexedTriangleList(&ScreenVertices[0], 6, &indices[0], 2);

        // multi sampling shadow
        driver->setRenderTarget(0);
        screenMaterial.setTexture(0, currentBuffer);
        screenMaterial.setTexture(1, lightPass);
        screenMaterial.setTexture(2, normalPass);
        screenShaderCallback->fogColor = fogColor;
        driver->setMaterial(screenMaterial);
        driver->drawIndexedTriangleList(&ScreenVertices[0], 6, &indices[0], 2);

    }
    else
    {
        driver->setRenderTarget(0);
        screenMaterial.setTexture(0, currentBuffer);
        screenMaterial.setTexture(1, lightPass);
        screenShaderCallback->fogColor = fogColor;
        driver->setMaterial(screenMaterial);
        driver->drawIndexedTriangleList(&ScreenVertices[0], 6, &indices[0], 2);
    }
}
