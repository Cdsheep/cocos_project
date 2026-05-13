#include "HelloWorldScene.h"
#include "view/GameBoardLayer.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

bool HelloWorld::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto boardLayer = smallgame::GameBoardLayer::create();
    this->addChild(boardLayer);
    return true;
}
