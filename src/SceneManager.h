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
	AbstractScene* mScene; // �V�[���Ǘ��ϐ�
	SceneList mNowScene;  // ���̃V�[���Ǘ��ϐ�
	SceneList mNextScene; // ���̃V�[���Ǘ��ϐ�

};