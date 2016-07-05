#include "MyScene.h"


string MyScene::id = "";

Scene* MyScene::createScene(string Id)
{
	auto scene = Scene::create();

	auto layer = MyScene::create();

	scene->addChild(layer);
	Id = id;
	return scene;
}

bool MyScene::init()
{
	if (!LayerColor::initWithColor(Color4B(0, 0, 0, 255)))
	{
		return false;
	}

	log("%s", id.c_str());
	return true;
}

void MyScene::update(float dt)
{

}