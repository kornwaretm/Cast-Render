#include <irrlicht.h>
#include "castRender/CCastRender.h"
#include "castRender/BaseObject.h"
#include "demos.h"

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

int main(int argc, char** argv)
{
    s32 W = 1024;
    s32 H = 512;
    IrrlichtDevice *device =
        createDevice(EDT_OPENGL, dimension2d<u32>(W, H), 32,
            false, false, false, 0);
    IVideoDriver* driver = device->getVideoDriver();
    ISceneManager* smgr = device->getSceneManager();
    IGUIEnvironment* guienv = device->getGUIEnvironment();

    // turbo
    // we do not need mip maps
    driver->setTextureCreationFlag(ETCF_CREATE_MIP_MAPS, false);

    //driver->setTextureCreationFlag(ETCF_OPTIMIZED_FOR_SPEED, true);

    device->setWindowCaption(L"Korn's cast rendering");
    ICameraSceneNode* cam = smgr->addCameraSceneNode();//FPS(0, 100.0f, 0.01f);
    //ICameraSceneNode* cam = smgr->addCameraSceneNodeFPS(0, 100.0f, 0.01f);
    cam->setPosition(vector3df(0.0, -2.0, 16.0));
    cam->setTarget(vector3df(0.0, 0.0, 0.0));

    CCastRender* cast = new CCastRender(device, dimension2d<u32>(W,H));
    cast->fogColor = SColor(0xff884422);
    cast->ambientColor = SColorf(0xff884422);
    cast->setCameraFoV(60.0f);

    // uncomment one
    //demoCube(cast, driver);
    //demoIndianTemple(cast, driver);
    //demoTilingBricks(cast, driver);
    //demoTransparentSurface(cast, driver);
    //demoCiliderObject(cast, driver);
    //demoSphereObject(cast, driver);
    //demoComposit(cast,driver); // maximum object per composite
    //demoWarrior(cast,driver);
    demoHelloWorld(cast,driver);
    //demoRtsWorld(cast, driver);

    IGUIStaticText* fpsText = guienv->addStaticText(L"log",rect<s32>(0,0,100,30),true);
    fpsText->setDrawBackground(true);
    fpsText->setDrawBorder(true);

    dinamicLight* light = new dinamicLight();
    light->position.X = 1.0f;
    light->position.Y = -1.0f;
    light->position.Z = 0.0f;
    light->color.setRed(255);
    light->color.setGreen(64);
    light->color.setBlue(64);
    light->radius = 1.1f;
    light->castShadows = true;
    cast->addLamp(light);

    dinamicLight* light2 = new dinamicLight();
    light2->position.X = -1.0f;
    light2->position.Y = -1.0f;
    light2->position.Z = 0.0f;
    light2->color.setRed(64);
    light2->color.setGreen(255);
    light2->color.setBlue(64);
    light2->radius = 0.1f;
    light2->castShadows = true;
    cast->addLamp(light2);

    dinamicLight* light3 = new dinamicLight();
    light3->position.X = 0.0f;
    light3->position.Y = -5.0f;
    light3->position.Z = 0.0f;
    light3->color.setRed(64);
    light3->color.setGreen(64);
    light3->color.setBlue(255);
    light3->radius = 0.1f;
    light3->castShadows = true;
    cast->addLamp(light3);

    dinamicLight* sun = new dinamicLight();
    sun->position.X = 0.0f;
    sun->position.Y = -20.0f;
    sun->position.Z = 10.0f;
    sun->color.setRed(255);
    sun->color.setGreen(255);
    sun->color.setBlue(255);
    sun->radius = 5.0f;
    sun->castShadows = true;
    cast->addLamp(sun);


    while(device->run())
    {
		core::stringw txt = L"FPS: ";
		txt += driver->getFPS();
		txt += "\n draw count : ";
		txt += cast->drawCallCount;
		txt += "\n object count : ";
		txt += cast->objectCount;

		fpsText->setText(txt.c_str());

		f32 time = f32(device->getTimer()->getTime() %5000) / 5000 * 6.28;
        light->position.X = sin(time) * 5.0f;
        light->position.Z = cos(time) * 5.0f;
        light->position.Y = -2.5f;

        light2->position.X = sin(-time) * 5.0f;
        light2->position.Z = cos(-time) * 5.0f;
        light2->position.Y = -2.5f;

        light3->position.X = sin(time) * 6.0f;
        light3->position.Y = cos(time) * 6.0f;
        light3->position.Z = -0.0;

        driver->beginScene(true, true, SColor(0,200,200,200));
        cast->update();
        smgr->drawAll();
        guienv->drawAll();
        driver->endScene();
        //device->sleep(30);
    }

    device->drop();

    return 0;
}

