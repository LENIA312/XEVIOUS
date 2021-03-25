#include "Game.h"

//-------------------------------------------------------------------------------------------------
#include "Image.h"
#include "BGM.h"
#include "SE.h"
#include "Keyboard.h"
#include "Pad.h"
#include "Mouse.h"
#include "FadeEffect.h"

//-------------------------------------------------------------------------------------------------
// コンストラクタ
Game::Game()
	: mFps()
	, mSceneManager()
{
}

//-------------------------------------------------------------------------------------------------
// メインループ
bool Game::mainLoop()
{
	// 入力処理
	Keyboard::getIns()->update();
	Pad::getIns()->update();
	Mouse::getIns()->update();

	// シーン処理
	mSceneManager.update();
	mSceneManager.draw();

	// フェードイン/アウト
	FadeEfect_Update();
	FadeEfect_Draw();

	// マウス描画
	Mouse::getIns()->draw();

	// FPS処理
	mFps.update();
	mFps.wait();
	//mFps.draw();

	// ESCキーで終了
	if (Pad::getIns()->get(Pad_ESC) == 1) {
		return false;
	}

	return true;
}