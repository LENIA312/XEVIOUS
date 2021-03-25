#pragma once

//-------------------------------------------------------------------------------------------------
#include "AbstractScene.h"
#include "Background.h"
#include "UI.h"
#include "Player.h"
#include "Camera.h"

//-------------------------------------------------------------------------------------------------
// ƒ^ƒCƒgƒ‹‰æ–Ê
class TitleScene : public AbstractScene
{
public:
	TitleScene(InterfaceSceneChanger* aChanger);
	~TitleScene();

	void update() override;
	void draw() const override;

private:
	Background mBackground;
	UI* mUI;
	Player* mPlayer;
	Camera* mCamera;

	char mHighScore[10];
	int mButtonAnime;
	int mTenmetuTimer;
	bool mIsGameStart;
	bool mIsEditorStart;

};
