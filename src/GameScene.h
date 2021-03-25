#pragma once

//-------------------------------------------------------------------------------------------------
#include "AbstractScene.h"
#include "Background.h"
#include "UI.h"
#include "Player.h"
#include "Camera.h"
#include "NPCManager.h"

//-------------------------------------------------------------------------------------------------
// ÉQÅ[ÉÄâÊñ 
class GameScene : public AbstractScene
{
public:
	GameScene(InterfaceSceneChanger* aChanger);
	~GameScene();
	void update() override;
	void draw() const override;

private:
	void startEvent();
	void gameOverEvent();
	void pauseEvent();

	Background mBackground;
	UI* mUI;
	Player* mPlayer;
	Camera* mCamera;
	NPCManager* mNPCMgr;

	int mPlayerLeft;
	int mHighScore;
	int mScore;
	int mDifficulty;

	bool mIsStartEvent;
	int mStartEventTimer;
	bool mIsGameOverEvent;
	int mGameOverEventTimer;
	bool mIsPause;
	bool mExtendFlg;
	int mExtendCnt;
	int mReturnTitleTimer;
	bool mReturnTitleFlg;

};