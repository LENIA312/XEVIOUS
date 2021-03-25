#include "TitleScene.h"

//-------------------------------------------------------------------------------------------------
#include <DxLib.h>
#include "Pad.h"
#include "Image.h"
#include "SE.h"
#include "Xprintf.h"
#include "FadeEffect.h"
#include "Mouse.h"

//-------------------------------------------------------------------------------------------------
// コンストラクタ
TitleScene::TitleScene(InterfaceSceneChanger* aChanger) : AbstractScene(aChanger)
{
	mButtonAnime = 0;
	mTenmetuTimer = 0;
	mIsGameStart = false;
	mIsEditorStart = false;

	mPlayer = new Player(168, 270 - 74);
	mUI = new UI(SceneList::Title);
	mCamera = new Camera();

	// ハイスコア
	int fp = FileRead_open("res/file/high_score.txt");
	FileRead_gets(mHighScore, 10, fp);
	FileRead_close(fp);
}

//-------------------------------------------------------------------------------------------------
// デストラクタ
TitleScene::~TitleScene()
{
	delete mPlayer;
	delete mUI;
	delete mCamera;
}

//-------------------------------------------------------------------------------------------------
// 更新
void TitleScene::update()
{
	// カメラ
	mCamera->update();
	float cameraY = mCamera->getY();
	mPlayer->setCameraY(cameraY);
	mBackground.setCameraY(cameraY);

	// 各オブジェクトの更新
	mBackground.update();
	mPlayer->update();
	mUI->setHighScore(atoi(mHighScore));

	// PRESS SPACEの点滅
	mTenmetuTimer++;
	if (mTenmetuTimer > 120) {
		mTenmetuTimer = 0;
	}

	// スペースキーでシーン変更
	{
		if (Pad::getIns()->get(Pad_Space) == 1 && !mIsGameStart && !mIsEditorStart && FadeEfect_GetState() == FadeState_Stop) {
			// 効果音
			SE::getIns()->playSE(SE_Credit);
			SE::getIns()->setCanPlaySE(false);
			// スタートフラグをtrueに
			mIsGameStart = true;
			// フェードイン
			FadeEfect_Mode(FadeMode_Up);
			FadeEfect_Play();
		}
		// スタートフラグが立っていたら
		if (mIsGameStart && FadeEfect_GetState() == FadeState_End) {
			// フェードアウト
			FadeEfect_Mode(FadeMode_Down);
			FadeEfect_Play();
			// ゲーム画面へ
			mSceneChanger->changeScene(SceneList::Game);
		}
	}

	// エディターボタンを押したらシーン変更
	{
		int mx = Mouse::getIns()->getXY(0);
		int my = Mouse::getIns()->getXY(1);
		mButtonAnime = 0;
		// マウスの位置がボタンに触れていたら
		if (mx >= 368 && mx <= 464 && my >= 236 && my <= 252) {
			// ボタンアニメ変更
			mButtonAnime = 1;
			// ボタンを左クリックしたら
			if (Mouse::getIns()->getClickLR(0) == 1 && !mIsGameStart && FadeEfect_GetState() == FadeState_Stop) {
				// 効果音
				SE::getIns()->playSE(SE_Credit);
				// スタートフラグをtrueに
				mIsEditorStart = true;
				// フェードイン
				FadeEfect_Mode(FadeMode_Right);
				FadeEfect_Play();
			}
		}
		// スタートフラグが立っていたら
		if (mIsEditorStart) {
			// ボタンアニメ変更
			mButtonAnime = 2;
			// シーン変更
			if (FadeEfect_GetState() == FadeState_End) {
				// フェードアウト
				FadeEfect_Mode(FadeMode_Left);
				FadeEfect_Play();
				// エディター画面へ
				mSceneChanger->changeScene(SceneList::Editor);
			}
		}
	}
}

//-------------------------------------------------------------------------------------------------
// 描画
void TitleScene::draw() const
{
	// 各オブジェクトの描画
	mBackground.draw();
	mPlayer->draw();

	// タイトルロゴ & NAMCO商標
	DrawGraph(84, 32, Image::getIns()->getImage(Image_TitleRogoEditor), TRUE);
	Xprintf_Color(0);
	Xprintf_Position(2);
	Xprintf("@ ^ 1982 NAMCO LTD.", 176, 232);
	DrawGraph(144, 248, Image::getIns()->getImage(Image_Namco), TRUE);

	// PRESS_SPACE文字
	if (mTenmetuTimer < 90 && !mIsGameStart) {
		Xprintf_Position(2);
		Xprintf("PRESS SPACE", 176, 144);
		Xprintf("ONE PLAYER ONLY", 176, 144 + 16);
	}

	// UI
	mUI->draw();

	// エディターボタン
	DrawGraph(368, 236, Image::getIns()->getImage(Image_LevelEditorButton, mButtonAnime), true);
}
