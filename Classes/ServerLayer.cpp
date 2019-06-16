#include "ServerLayer.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
using namespace cocostudio::timeline;

Scene* ServerLayer::createScene()
{
	auto scene = Scene::create();
	auto layer = ServerLayer::create();

	scene->addChild(layer);

	return scene;
}

bool ServerLayer::init()
{
	if (!GameScene::init())
	{
		return false;
	}
	this->scheduleUpdate();

	log("server");
	return true;
}

void ServerLayer::initNetwork()
{
	_server = SocketServer::getInstance();
	_server->startServer(8000);
}

void ServerLayer::sendData(DataType type)
{
	GameData data;
	data.dataType = type;
	data.dataSize = sizeof(GameData);
	//data.position = _player->getPosition();
	_server->sendMessage((const char*)&data, sizeof(GameData));
}



void ServerLayer::update(float dt)
{
	static float curTime = 0;
	curTime += dt;
	if (curTime > dt * 2.f)
	{
	//	this->sendData(POSITION);
		curTime -= dt * 2.f;
	}
}