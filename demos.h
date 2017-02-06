#include <irrlicht.h>
#include "castRender/CCastRender.h"
#include "castRender/BaseObject.h"
#include "castRender/objects/SphereObject.h"
#include "castRender/objects/CilinderObject.h"
#include "castRender/objects/SphereObject.h"
#include "castRender/objects/CompositObject.h"
#include "config.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using namespace cast;
using namespace volumetric_scene;

void demoHelloWorld(CCastRender* cast, IVideoDriver* driver)
{
    BaseObject* ob;/* = new BaseObject(cast );
    ob->setHeightMap(driver->getTexture("./textures/tileheight.jpg"));
    ob->setColorMap(driver->getTexture("./textures/tile.jpg"));
    ob->setScale(vector3df(8.0f, 0.25f, 8.0f));
    ob->uvMul = vector2df(4.0f,4.0f);
    cast->addObject(ob);*/

    /*CompositObject* com = new CompositObject(cast);
    com->setHeightMap(driver->getTexture("./textures/helloworld.png"));
    com->setColorMap(driver->getTexture("./textures/helloworldcolor.png"));
    cast->addObject(com);*/

    ob = new BaseObject(cast);
    ob->setHeightMap(driver->getTexture("./textures/helloworld.png"));
    ob->setColorMap(driver->getTexture("./textures/helloworldcolor.png"));
    ob->setScale(vector3df(8.0f, 0.5f, 2.0f));
    ob->setRotation(vector3df(90.0f,0.0f,180.0f));
    ob->setPosition(vector3df(0.0f , -2.0f, 0.0f));
    ob->uvMul = vector2df(4.0f,1.0f);
    //ob->uvAdd = vector2df(0.5f,0.0f);
    //com->addObject(ob);
    cast->addObject(ob);

}

void demoCube(CCastRender* cast, IVideoDriver* driver)
{
    /*BaseObject* ob = new BaseObject(cast);
    //ob->setHeightMap(driver->getTexture("./textures/test.png"));
    //ob->setHeightMap(driver->getTexture("./textures/humanbake.png"));
    //ob->setHeightMap(driver->getTexture("./textures/cube.png"));
    //ob->setColorMap(driver->getTexture("./textures/indiancolor.jpg"));
    //ob->setColorMap(driver->getTexture("./textures/plasma.png"));
    //ob->setScale(vector3df(4.0f, 4.0f, 4.0f));
    ob->setRotation(vector3df(90.0f,0.0f,180.0f));
    //ob->setPosition(vector3df(1.0f , 0.0f, 5.0f));
    cast->addObject(ob);*/

    BaseObject* ob = new BaseObject(cast );
    ob->setHeightMap(driver->getTexture("./textures/tileheight.jpg"));
    ob->setColorMap(driver->getTexture("./textures/tile.jpg"));
    ob->setScale(vector3df(32.0f, 0.25f, 32.0f));
    ob->uvMul = vector2df(16.0f,16.0f);
    cast->addObject(ob);
}

void demoComposit(CCastRender* cast, IVideoDriver* driver )
{
    CompositObject* com = new CompositObject(cast);
    com->setHeightMap(driver->getTexture("./textures/tileheight.jpg"));
    com->setColorMap(driver->getTexture("./textures/tile.jpg"));
    cast->addObject(com);

    f32 rows = floor(sqrt(f32(max_composit)));
    f32 start = -rows / 2.0f * 2;
    f32 end = -start;

    for(f32 x = start ; x < end ; x += 2.0f)
    for(f32 y = start ; y < end ; y += 2.0f)
    {
        BaseObject* ob = new BaseObject(cast);
        ob->setHeightMap(driver->getTexture("./textures/tileheight.jpg"));
        ob->setColorMap(driver->getTexture("./textures/tile.jpg"));
        ob->setScale(vector3df(1.0f, 0.125f + (f32(rand() % 1000)/1000.0f ) * 0.125f, 1.0f));
        ob->setPosition(vector3df(x , 0.0f, y));
        ob->setRotation(vector3df(
                    (f32(rand() % 1000)/1000.0f ) * 360.0f,
                    (f32(rand() % 1000)/1000.0f ) * 360.0f,
                    (f32(rand() % 1000)/1000.0f ) * 360.0f));
        com->addObject(ob);
    }
}

void demoCiliderObject(CCastRender* cast, IVideoDriver* driver )
{
    CilinderObject * cob = new CilinderObject(cast);
    cob->setHeightMap(driver->getTexture("./textures/human.png"));
    cob->setColorMap(driver->getTexture("./textures/greencube.png"));
    cob->setScale(vector3df(8.0f, 8.0f, 8.0f));
    cast->addObject(cob);
}

void demoWarrior(CCastRender* cast, IVideoDriver* driver)
{
    BaseObject* ob = new BaseObject(cast );
    ob->setHeightMap(driver->getTexture("./textures/tileheight.jpg"));
    ob->setColorMap(driver->getTexture("./textures/tile.jpg"));
    ob->setScale(vector3df(32.0f, 0.25f, 32.0f));
    ob->uvMul = vector2df(16.0f,16.0f);
    cast->addObject(ob);

    CompositObject* com = new CompositObject(cast);
    com->setHeightMap(driver->getTexture("./textures/warriorheight.png"));
    com->setColorMap(driver->getTexture("./textures/warriorcolor.png"));
    cast->addObject(com);
    // head
    CilinderObject *cob = new CilinderObject(cast);
    cob->setHeightMap(driver->getTexture("./textures/warriorheight.png"));
    cob->setColorMap(driver->getTexture("./textures/warriorcolor.png"));
    cob->setScale(vector3df(2.0f, 1.0f, 1.5f));
    cob->setPosition(vector3df(0.0f , -6.0f, 0.25f));
    cob->uvMul = vector2df(0.25f,0.25f);
    cob->uvAdd = vector2df(0.5f,0.0f);
    cob->setRotation(vector3df(0.0f,0.0f, 0.0f));
    com->addObject(cob);
    // body
    cob = new CilinderObject(cast);
    cob->setHeightMap(driver->getTexture("./textures/warriorheight.png"));
    cob->setColorMap(driver->getTexture("./textures/warriorcolor.png"));
    cob->setScale(vector3df(2.5f, 1.5f, 2.8f));
    cob->setPosition(vector3df(0.0f , -3.5f, 0.0f));
    cob->uvMul = vector2df(0.25f,0.25f);
    cob->uvAdd = vector2df(0.0f,0.0f);
    cob->setRotation(vector3df(0.0f,0.0f, 0.0f));
    com->addObject(cob);

    // hips
    cob = new CilinderObject(cast);
    cob->setHeightMap(driver->getTexture("./textures/warriorheight.png"));
    cob->setColorMap(driver->getTexture("./textures/warriorcolor.png"));
    cob->setScale(vector3df(1.75f, 0.65f, 1.75f));
    cob->setPosition(vector3df(0.0f , -1.5f, 0.0f));
    cob->uvMul = vector2df(0.25f,0.25f);
    cob->uvAdd = vector2df(0.75f,0.0f);
    cob->setRotation(vector3df(0.0f,0.0f, 0.0f));
    com->addObject(cob);

    //arm
    cob = new CilinderObject(cast);
    cob->setHeightMap(driver->getTexture("./textures/warriorheight.png"));
    cob->setColorMap(driver->getTexture("./textures/warriorcolor.png"));
    cob->setScale(vector3df(1.0f, 1.5f, 1.0f));
    cob->setPosition(vector3df(1.6f , -3.5f, 0.0f));
    cob->uvMul = vector2df(0.25f,0.25f);
    cob->uvAdd = vector2df(0.0f,0.25f);
    cob->setRotation(vector3df(0.0f,90.0f, -30.0f));
    com->addObject(cob);

    //arm
    cob = new CilinderObject(cast);
    cob->setHeightMap(driver->getTexture("./textures/warriorheight.png"));
    cob->setColorMap(driver->getTexture("./textures/warriorcolor.png"));
    cob->setScale(vector3df(-1.0f, 1.5f, 1.0f));
    cob->setPosition(vector3df(-1.6f , -3.5f, 0.0f));
    cob->uvMul = vector2df(0.25f,0.25f);
    cob->uvAdd = vector2df(0.0f,0.25f);
    cob->setRotation(vector3df(0.0f,-90.0f, 30.0f));
    com->addObject(cob);

    //lower arm
    cob = new CilinderObject(cast);
    cob->setHeightMap(driver->getTexture("./textures/warriorheight.png"));
    cob->setColorMap(driver->getTexture("./textures/warriorcolor.png"));
    cob->setScale(vector3df(-1.0f, 1.5f, 1.0f));
    cob->setPosition(vector3df(-2.6f , -1.5f, 0.0f));
    cob->uvMul = vector2df(0.25f,0.25f);
    cob->uvAdd = vector2df(0.25f,0.25f);
    cob->setRotation(vector3df(0.0f,-90.0f, 30.0f));
    com->addObject(cob);

    //lower arm
    cob = new CilinderObject(cast);
    cob->setHeightMap(driver->getTexture("./textures/warriorheight.png"));
    cob->setColorMap(driver->getTexture("./textures/warriorcolor.png"));
    cob->setScale(vector3df(-1.0f, 1.5f, 1.0f));
    cob->setPosition(vector3df(2.6f , -1.5f, 0.0f));
    cob->uvMul = vector2df(0.25f,0.25f);
    cob->uvAdd = vector2df(0.25f,0.25f);
    cob->setRotation(vector3df(0.0f,90.0f, -30.0f));
    com->addObject(cob);

    //legg
    cob = new CilinderObject(cast);
    cob->setHeightMap(driver->getTexture("./textures/warriorheight.png"));
    cob->setColorMap(driver->getTexture("./textures/warriorcolor.png"));
    cob->setScale(vector3df(-1.1f, 1.6f, 1.1f));
    cob->setPosition(vector3df(-0.6f , 0.0f, 0.0f));
    cob->uvMul = vector2df(0.25f,0.25f);
    cob->uvAdd = vector2df(0.0f,0.25f);
    cob->setRotation(vector3df(0.0f,-90.0f, 10.0f));
    com->addObject(cob);

    //legg
    cob = new CilinderObject(cast);
    cob->setHeightMap(driver->getTexture("./textures/warriorheight.png"));
    cob->setColorMap(driver->getTexture("./textures/warriorcolor.png"));
    cob->setScale(vector3df(1.1f, 1.6f, 1.1f));
    cob->setPosition(vector3df(0.6f , 0.0f, 0.0f));
    cob->uvMul = vector2df(0.25f,0.25f);
    cob->uvAdd = vector2df(0.0f,0.25f);
    cob->setRotation(vector3df(0.0f,90.0f, -10.0f));
    com->addObject(cob);

}

void demoSphereObject(CCastRender* cast, IVideoDriver* driver )
{
    BaseObject* ob = new BaseObject(cast );
    ob->setHeightMap(driver->getTexture("./textures/tileheight.jpg"));
    ob->setColorMap(driver->getTexture("./textures/tile.jpg"));
    ob->setScale(vector3df(8.0f, 0.25f, 8.0f));
    ob->uvMul = vector2df(4.0f,4.0f);
    cast->addObject(ob);

    /*CilinderObject *cob = new CilinderObject(cast);
    cob->setHeightMap(driver->getTexture("./textures/kotakkotak.jpg"));
    cob->setColorMap(driver->getTexture("./textures/kotakkotakdif.jpg"));
    //cob->setScale(vector3df(1.0f, 2.0f, 1.0f));
    cob->setPosition(vector3df(3.0f , -2.0f, -0.0f));
    cast->addObject(cob);*/

    SphereObject * sob = new SphereObject(cast);
    sob->setHeightMap(driver->getTexture("./textures/kotakkotak.jpg"));
    sob->setColorMap(driver->getTexture("./textures/kotakkotakdif.jpg"));
    sob->setScale(vector3df(1.0f, 2.0f, 1.0f));
    sob->setPosition(vector3df(0.0f , -2.0f, -0.0f));
    //sob->setRotation(vector3df(0.0f,0.0f, 90.0f));
    cast->addObject(sob);
}

void demoTilingBricks(CCastRender* cast, IVideoDriver* driver)
{
    CompositObject* com = new CompositObject(cast);
    com->setHeightMap(driver->getTexture("./textures/tileheight.jpg"));
    com->setColorMap(driver->getTexture("./textures/tile.jpg"));
    cast->addObject(com);

    for(f32 x = -16.0f ; x < 16.0f ; x += 4.0f)
    for(f32 y = -16.0f ; y < 16.0f ; y += 4.0f)
    {
        BaseObject* ob = new BaseObject(cast);
        ob->setHeightMap(driver->getTexture("./textures/tileheight.jpg"));
        ob->setColorMap(driver->getTexture("./textures/tile.jpg"));
        ob->setScale(vector3df(2.0f, 0.25f, 2.0f));
        ob->setPosition(vector3df(x , 0.0f, y));
        ob->uvMul = vector2df(1.0f,1.0f);
        com->addObject(ob);
    }
}

void demoIndianTemple(CCastRender* cast, IVideoDriver* driver )
{
    BaseObject* ob;

    ob = new BaseObject(cast );
    ob->setHeightMap(driver->getTexture("./textures/tileheight.jpg"));
    ob->setColorMap(driver->getTexture("./textures/tile.jpg"));
    ob->setScale(vector3df(32.0f, 0.25f, 32.0f));
    ob->uvMul = vector2df(16.0f,16.0f);
    cast->addObject(ob);


    // the composit
    CompositObject* com = new CompositObject(cast);
    com->setHeightMap(driver->getTexture("./textures/indianAtlasHeight.jpg"));
    com->setColorMap(driver->getTexture("./textures/indianAtlasColor.jpg"));
    com->setPosition(vector3df(0.0f , 0.0f, 0.0f));
    cast->addObject(com);

    // front
    ob = new BaseObject(cast);
    ob->setHeightMap(driver->getTexture("./textures/indianheight.jpg"));
    ob->setColorMap(driver->getTexture("./textures/indiancolor.jpg"));
    ob->setScale(vector3df(2.0f, 1.0f, 2.0f));
    ob->setRotation(vector3df(90.0f,0.0f,180.0f));
    ob->setPosition(vector3df(0.0f , -2.0f, 2.0f));
    ob->uvMul = vector2df(0.25f,1.0f);
    ob->uvAdd = vector2df(0.5f,0.0f);
    com->addObject(ob);

   // back
    ob = new BaseObject(cast);
    ob->setHeightMap(driver->getTexture("./textures/indianheight.jpg"));
    ob->setColorMap(driver->getTexture("./textures/indiancolor.jpg"));
    ob->setScale(vector3df(2.0f, 1.0f, 2.0f));
    ob->setRotation(vector3df(-90.0f,180.0f, 0.0f));
    ob->setPosition(vector3df(0.0f , -2.0f, -2.0f));
    ob->uvMul = vector2df(0.25f,1.0f);
    ob->uvAdd = vector2df(0.5f,0.0f);
    com->addObject(ob);

    // left
    ob = new BaseObject(cast);
    ob->setHeightMap(driver->getTexture("./textures/indianheight.jpg"));
    ob->setColorMap(driver->getTexture("./textures/indiancolor.jpg"));
    ob->setScale(vector3df(2.0f, 1.0f, 2.0f));
    ob->setRotation(vector3df(90.0f,90.0f,180.0f));
    ob->setPosition(vector3df(2.0f , -2.0f, 0.0f));
    ob->uvMul = vector2df(0.25f,1.0f);
    ob->uvAdd = vector2df(0.5f,0.0f);
    com->addObject(ob);

    // right
    ob = new BaseObject(cast);
    ob->setHeightMap(driver->getTexture("./textures/indianheight.jpg"));
    ob->setColorMap(driver->getTexture("./textures/indiancolor.jpg"));
    ob->setScale(vector3df(2.0f, 1.0f, 2.0f));
    ob->setRotation(vector3df(90.0f,90.0f, 180.0f));
    ob->setPosition(vector3df(-2.0f , -2.0f, 0.0f));
    ob->uvMul = vector2df(0.25f,1.0f);
    ob->uvAdd = vector2df(0.5f,0.0f);
    com->addObject(ob);

    for(f32 i = -2 ; i <= 2 ; i+=4)
    for(f32 j = -2 ; j <= 2 ; j+=4)
    {
        CilinderObject *cob = new CilinderObject(cast);
        cob->setHeightMap(driver->getTexture("./textures/kotakkotak.jpg"));
        cob->setColorMap(driver->getTexture("./textures/kotakkotakdif.jpg"));
        cob->setScale(vector3df(0.5f, 3.0f, 0.5f));
        cob->setPosition(vector3df(i , -3.0f, j));
        cob->uvMul = vector2df(0.25f,3.0f);
        cob->uvAdd = vector2df(0.25f,0.0f);
        com->addObject(cob);
    }

    SphereObject * sob = new SphereObject(cast);
    sob->setHeightMap(driver->getTexture("./textures/kotakkotak.jpg"));
    sob->setColorMap(driver->getTexture("./textures/kotakkotakdif.jpg"));
    sob->setScale(vector3df(2.0f, 2.0f, 2.0f));
    sob->setPosition(vector3df(0.0f , -4.0f, -0.0f));
    sob->setRotation(vector3df(90.0f, 0.0f, 0.0f));
    sob->uvMul = vector2df(0.25f,1.0f);
    sob->uvAdd = vector2df(0.75f,0.0f);
    com->addObject(sob);
}

void demoTransparentSurface(CCastRender* cast, IVideoDriver* driver )
{
    BaseObject* ob = new BaseObject(cast );
    ob->setHeightMap(driver->getTexture("./textures/tileheight.jpg"));
    ob->setColorMap(driver->getTexture("./textures/tile.jpg"));
    ob->setScale(vector3df(32.0f, 0.25f, 32.0f));
    ob->uvMul = vector2df(16.0f,16.0f);
    cast->addObject(ob);

    ob = new BaseObject(cast);
    ob->setHeightMap(driver->getTexture("./textures/halfball.jpg"));
    ob->setColorMap(driver->getTexture("./textures/halfballcolor.png"));
    ob->setScale(vector3df(2.0f, 1.0f, 2.0f));
    ob->setPosition(vector3df(0.0f , -2.0f, -0.0f));
    ob->setRotation(vector3df(90.0f,0.0f, 0.0f));
    //ob->uv = vector2df(1.0f,8.0f);
    cast->addObject(ob);
}

void demoRtsWorld(CCastRender* cast, IVideoDriver* driver )
{
}
