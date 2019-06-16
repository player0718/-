#ifndef __SERVER_LAYER_H__
#define __SERVER_LAYER_H__

#include "GameScene.h"
#include "socket/SocketServer.h"

class ServerLayer : public GameScene
{
public:
    static cocos2d::Scene* createScene();
	virtual bool init();
    CREATE_FUNC(ServerLayer);


	Sprite _player;
	void update(float dt);

protected:
	void initNetwork();
	void sendData(DataType type);

private:
	SocketServer* _server;
	
};

#endif 
