#pragma once

#include "InterfaceSceneChanger.h"
#include "AbstractScene.h"

class SceneManager : public InterfaceSceneChanger
{
public:
	SceneManager();
	~SceneManager();
	void update();
	void draw();
	void changeScene(SceneList aNextScene) override;
	SceneList getNowScene();

private:
	AbstractScene* mScene; // シーン管理変数
	SceneList mNowScene;  // 今のシーン管理変数
	SceneList mNextScene; // 次のシーン管理変数

};