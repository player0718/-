#ifndef __CLIENT_LAYER_H__
#define __CLIENT_LAYER_H__

#include "GameScene.h"
#include "socket/SocketClient.h"

class ClientLayer : public GameScene
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(ClientLayer);


	void onRecv(const char* data, int count);
	void onDisconnect();



protected:
	void initNetwork();

	SocketClient* _client;
};

#endif 
