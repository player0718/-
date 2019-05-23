#include "scenemanager.h"
#include "gamemenu.h"

scenemanager * scenemanager::s_SceneManager = NULL;

scenemanager* scenemanager::getInstance()
{
	if (s_SceneManager == NULL)
	{
		s_SceneManager = new scenemanager();
		if (s_SceneManager && s_SceneManager->init())
		{
			s_SceneManager->autorelease();
		}
		else
		{
			CC_SAFE_DELETE(s_SceneManager);
			s_SceneManager = NULL;
		}
	}
	return s_SceneManager;
}


bool scenemanager::init() 
{
	return true;
}



void scenemanager::changeScene(SceneType enSceneType)
{
	Scene * scene = NULL;
	TransitionScene * ccts = NULL;

	switch (enSceneType)
	{
	
	case en_MenuScene:
		scene = gamemenu::createScene();
		break;
	}

	if (scene == NULL)
		return;

	auto pDirector = Director::getInstance();
	auto curScene = pDirector->getRunningScene();

	if (ccts == NULL)
		ccts = CCTransitionFadeTR::create(1.0f, scene);

	if (curScene == NULL)
		pDirector->runWithScene(scene);
	else
		pDirector->replaceScene(scene);
}


