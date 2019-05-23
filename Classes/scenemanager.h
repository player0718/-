#ifndef _scenemanager_H_
#define _scenemanager_H_

#include "cocos2d.h"
USING_NS_CC;
class scenemanager : public Ref
{
  public:
	enum SceneType
	{
		en_MenuScene,		//´óÌü³¡¾°
	};
  static scenemanager * getInstance();
  virtual bool init();

  void changeScene(SceneType enSceneType);
  private:
	static scenemanager * s_SceneManager;

};
#endif
