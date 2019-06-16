#include "ClientLayer.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
using namespace cocostudio::timeline;

Scene* ClientLayer::createScene()
{
	auto scene = Scene::create();
	auto layer = ClientLayer::create();

	scene->addChild(layer);

	return scene;
}

bool ClientLayer::init()
{
	if ( !GameScene::init() )
	{
		return false;
	}

	log("client");
	return true;
}

void ClientLayer::initNetwork()
{
	_client = SocketClient::construct();
	_client->onRecv = CC_CALLBACK_2(ClientLayer::onRecv, this);
	_client->onDisconnect = CC_CALLBACK_0(ClientLayer::onDisconnect, this);

	if (!_client->connectServer("127.0.0.1", 8000))
	{
	
	}
}


void ClientLayer::onRecv(const char* data, int count)
{

}

void ClientLayer::onDisconnect()
{
	
}