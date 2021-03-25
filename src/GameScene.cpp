#include "GameScene.h"

//-------------------------------------------------------------------------------------------------
#include <DxLib.h>
#include <fstream>
#include "Pad.h"
#include "BGM.h"
#include "SE.h"
#include "Xprintf.h"
#include "FadeEffect.h"
#include "Mouse.h"

//-------------------------------------------------------------------------------------------------
// コンストラクタ
GameScene::GameScene(InterfaceSceneChanger* aChanger) : AbstractScene(aChanger)
{
	mPlayerLeft = 2;
	mScore = 0;
	mDifficulty = 2;
	mIsStartEvent = true;
	mStartEventTimer = 0;
	mIsGameOverEvent = false;
	mGameOverEventTimer = 0;
	mIsPause = false;
	mExtendFlg = false;
	mExtendCnt = 0;
	mReturnTitleTimer = 0;
	mReturnTitleFlg = false;

	mPlayer = new Player();
	mUI = new UI(SceneList::Game);
	mCamera = new Camera();
	mNPCMgr = new NPCManager(mPlayer);

	// ハイスコア
	int fp = FileRead_open("res/file/high_score.txt");
	char c[10];
	FileRead_gets(c, 10, fp);
	mHighScore = atoi(c);
	FileRead_close(fp);
}

//-------------------------------------------------------------------------------------------------
// デストラクタ
GameScene::~GameScene()
{
	delete mPlayer;
	delete mUI;
	delete mCamera;
	delete mNPCMgr;
}

//-------------------------------------------------------------------------------------------------
// 更新
void GameScene::update()
{
	// ポーズしていない時
	if (!mIsPause)
	{
		// 各オブジェクトの更新
		{
			// マウス非表示
			Mouse::getIns()->setAnime(3);
			// カメラの更新
			{
				mCamera->update();
				// カメラの座標をセット
				float cameraY = mCamera->getY();
				mPlayer->setCameraY(cameraY);
				mBackground.setCameraY(cameraY);
				mNPCMgr->setCamera(cameraY);
			}
			// 背景更新
			{
				// 前のエリアを保持
				int tmpArea = mBackground.getArea();
				// 更新
				mBackground.update();
				// エリアが変わったら難易度+1
				if (tmpArea != mBackground.getArea() && mDifficulty < 4) {
					mDifficulty++;
				}
			}
			// 各オブジェクトの更新
			{
				mPlayer->update();
				mNPCMgr->setNowArea(mBackground.getArea());
				mNPCMgr->setDifficulty(mDifficulty);
				mNPCMgr->update();
			}
			// 残基加算
			{
				// 加算数が0以外なら
				if (mNPCMgr->getAddLeft() != 0) {
					mPlayerLeft += mNPCMgr->getAddLeft();
					// 加算数をリセット
					mNPCMgr->resetAddLeft();
				}
				// スコアが溜まったら
				if (!mExtendFlg && mNPCMgr->getScore() >= 20000 && mExtendCnt == 0) {
					mExtendFlg = true;
					mExtendCnt++;
				}
				if (!mExtendFlg && mNPCMgr->getScore() >= 60000 * mExtendCnt && mExtendCnt > 0) {
					mExtendFlg = true;
					mExtendCnt++;
				}
				if (mExtendFlg) {
					mExtendFlg = false;
					mPlayerLeft++;
					SE::getIns()->playSE(SE_Extend);
				}
				// 残基を9以上増さない
				if (mPlayerLeft > 9) {
					mPlayerLeft = 9;
				}
			}
			// ハイスコア更新
			if (mNPCMgr->getScore() > mHighScore) {
				mHighScore = mNPCMgr->getScore();
			}
			// UIの更新
			mUI->setHighScore(mHighScore);
			mUI->setScore(mNPCMgr->getScore());
			mUI->setLeft(mPlayerLeft);
			mUI->setArea(mBackground.getArea() + 1);
		}
		// イベント
		{
			startEvent();
			gameOverEvent();			
		}
	}
	// ポーズイベント
	pauseEvent();
}

//-------------------------------------------------------------------------------------------------
// 描画
void GameScene::draw() const
{
	// 各オブジェクトの描画
	mBackground.draw();
	mNPCMgr->draw();
	mPlayer->draw();
	// スタートイベント中の文字
	if (mIsStartEvent && mStartEventTimer < 190 && !mIsGameOverEvent) {
		Xprintf_Position(2);
		Xprintf("PLAYER ONE", 176, 144);
		Xprintf("READY!", 176, 144 + 16);
		Xprintf_Position(0);
		XNprintf(mPlayerLeft, 116, 144 + 16 * 2);
		Xprintf_Position(2);
		Xprintf("  SOLVALOU LEFT", 176, 144 + 16 * 2);
	}
	// ゲームオーバー時の文字
	if (mIsGameOverEvent && mPlayerLeft < 0 && mGameOverEventTimer > 120) {
		Xprintf_Position(2);
		Xprintf("GAME OVER", 176, 135);
	}
	// ポーズ中の文字
	if (mIsPause) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
		DrawBox(0, 270, 352, 270 - 24, GetColor(0, 0, 0), TRUE);
		DrawBox(0, 270, mReturnTitleTimer, 270 - 24, GetColor(231, 81, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		Xprintf_Position(2);
		Xprintf("[ SHIFT ] : RETURN TO TITLE", 176, 270 - 16);
	}
	// UI
	mUI->draw();

	// デバッグ
	//clsDx();
	//printfDx("%d", mDifficulty);
}

//-------------------------------------------------------------------------------------------------
// スタートイベント
void GameScene::startEvent()
{
	if (mIsStartEvent && !mIsGameOverEvent) {
		// 初めにBGMを流してSE再生フラグを折る
		if (mStartEventTimer == 0) {
			BGM::getIns()->playBGM(BGM_Start, false, true);
			SE::getIns()->setCanPlaySE(false);
		}
		// BGMが鳴り終わったらイベント終了
		if (!BGM::getIns()->checkIsPlay(BGM_Start)) {
			mIsStartEvent = false;
			// SE再生フラグを立てる
			SE::getIns()->setCanPlaySE(true);
			// BGMを流す
			BGM::getIns()->playBGM(BGM_Game, true, true);
		}
		mStartEventTimer++;
	}
	else {
		mStartEventTimer = 0;
	}
}

//-------------------------------------------------------------------------------------------------
// ゲームオーバー
void GameScene::gameOverEvent()
{
	// プレイヤーが撃墜されたら
	if (!mPlayer->checkActiveFlag() && !mIsGameOverEvent) {
		// フラグを立てる
		mIsGameOverEvent = true;
		// BGMを止める
		{
			// スタートイベント中なら
			if (mIsStartEvent) {
				BGM::getIns()->stopBGM(BGM_Start);
				static int tmpSE = LoadSoundMem("res/se/miss.wav");
				PlaySoundMem(tmpSE, DX_PLAYTYPE_BACK, TRUE);
			}
			// ゲームプレイ中なら
			else {
				BGM::getIns()->stopBGM(BGM_Game);
				BGM::getIns()->stopBGM(BGM_Boss);
			}
		}
		// 残機を減らす
		mPlayerLeft--;
	}
	// イベント処理
	if (mIsGameOverEvent) {
		// 残機が残っている時
		if (mPlayerLeft >= 0) {
			if (mGameOverEventTimer == 60) {
				FadeEfect_Mode(FadeMode_Up);
				FadeEfect_Play();
			}
			if (FadeEfect_GetState() == FadeState_End) {
				// 前のエリアを保持
				int tmpArea = mBackground.getArea();
				// リスタート
				mCamera->reset();
				mPlayer->reset();
				mBackground.reset();
				mNPCMgr->clearList();
				mIsGameOverEvent = false;
				mIsStartEvent = true;
				FadeEfect_Mode(FadeMode_Down);
				FadeEfect_Play();
				// エリアが変わっていなかったら難易度 -1
				if (tmpArea == mBackground.getArea() && mDifficulty > 0) {
					mDifficulty -= 1;
				}
			}
		}
		// 残機が残っていない時
		else {
			if (mGameOverEventTimer == 240) {
				FadeEfect_Mode(FadeMode_Down);
				FadeEfect_Play();
			}
			if (FadeEfect_GetState() == FadeState_End) {
				// ハイスコアセーブ
				std::ofstream ofs("res/file/high_score.txt");
				ofs << mHighScore << std::endl;
				// タイトルシーンへ
				mSceneChanger->changeScene(SceneList::Title);
				FadeEfect_Mode(FadeMode_Up);
				FadeEfect_Play();
			}
		}
		mGameOverEventTimer++;
	}
	else {
		mGameOverEventTimer = 0;
	}
}

//-------------------------------------------------------------------------------------------------
// ポーズイベント
void GameScene::pauseEvent()
{
	// プレイ中
	if (!mIsPause) {
		// スペースキーでポーズ
		if (Pad::getIns()->get(Pad_Space) == 1 && !mIsGameOverEvent) {
			mIsPause = true;
			// スタートイベント中なら
			if (mIsStartEvent) {
				SE::getIns()->setCanPlaySE(true);
				BGM::getIns()->stopBGM(BGM_Start);
			}
			// ゲームプレイ中なら
			else {
				BGM::getIns()->stopBGM(BGM_Game);
				BGM::getIns()->stopBGM(BGM_Boss);
			}
			// SE
			SE::getIns()->playSE(SE_Credit);
		}
	}
	// ポーズ中
	else {
		// タイトル画面へ戻るフラグがfalseの時
		if (!mReturnTitleFlg) {
			// スペースキーでポーズ解除
			if (Pad::getIns()->get(Pad_Space) == 1) {
				mIsPause = false;
				// 先にSEを鳴らす
				SE::getIns()->playSE(SE_Credit);
				// スタートイベント中なら
				if (mIsStartEvent) {
					SE::getIns()->setCanPlaySE(false);
					BGM::getIns()->playBGM(BGM_Start, false, false);
				}
				// ゲームプレイ中なら
				else {
					BGM::getIns()->playBGM(BGM_Game, true, false);
				}
			}
			// シフトキーでタイトルへ戻る
			if (Pad::getIns()->get(Pad_LShift) > 0 || Pad::getIns()->get(Pad_RShift) > 0) {
				mReturnTitleTimer += 8;
			}
			else if (mReturnTitleTimer > 0) {
				mReturnTitleTimer = 0;
			}
			if (mReturnTitleTimer >= 352) {
				mReturnTitleFlg = true;
				SE::getIns()->setCanPlaySE(true);
				SE::getIns()->playSE(SE_Credit);
			}
		}
		// タイトル画面へ戻る処理
		else {
			if (FadeEfect_GetState() == FadeState_Stop) {
				FadeEfect_Mode(FadeMode_Down);
				FadeEfect_Play();
			}
			if (FadeEfect_GetState() == FadeState_End) {
				// タイトルシーンへ
				mSceneChanger->changeScene(SceneList::Title);
				FadeEfect_Mode(FadeMode_Up);
				FadeEfect_Play();
			}
		}
	}
}
