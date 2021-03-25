#pragma once

#include "Background.h"
#include "Camera.h"

class EditorUI
{
public:
	EditorUI(Camera* aCamera, Background* aBackground);
	~EditorUI();
	void update();
	void draw() const;
	int getEnemyNum();
	int getEditorButtonState(int a);
	int getDifficulty();
	void setEnemyCount(int aCount);
	void resetEditorButton();

private:
	// 変数
	int mMouseX;
	int mMouseY;
	int mNowArea;
	int mDifficulty;
	int mBlend;
	bool mBlendFlg;
	bool mIsPushAreaChangeButton[2];
	int mEnemyButtonNum;
	int mEnemyNum;
	int mEditorButtonState[3];
	float mEditorButtonAnime[3];
	int mEnemyCount;
	float mSaveMeter;

	// オブジェクト
	Camera* mCamera;
	Background* mBackground;

	// 関数
	void drawEnemyName() const;

};