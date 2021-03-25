#include <DxLib.h>
#include "Background.h"
#include "Image.h"
#include "Define.h"

/*------------------------------------------------------------------------------
コンストラクタ
------------------------------------------------------------------------------*/
Background::Background()
{
	// モード
	mMode = 0;
	// y座標
	mCameraY = (float)(Define::MapHeight + Define::ForestHeight - Define::GameScreenHeight);
	mTmpCameraY = mCameraY;
	mBackgroundY[0] = 0;
	mBackgroundY[1] = Define::MapHeight;
	mForestY		= Define::MapHeight;
	// 森表示フラグ
	mIsForest = true;
	// エリア毎のx座標
	mAreaPoint[ 0] = 480;
	mAreaPoint[ 1] = 736;
	mAreaPoint[ 2] =  80;
	mAreaPoint[ 3] = 608;
	mAreaPoint[ 4] = 240;
	mAreaPoint[ 5] = 528;
	mAreaPoint[ 6] =  16;
	mAreaPoint[ 7] = 720;
	mAreaPoint[ 8] = 432;
	mAreaPoint[ 9] =   0;
	mAreaPoint[10] = 608;
	mAreaPoint[11] = 720;
	mAreaPoint[12] = 304;
	mAreaPoint[13] = 80;
	mAreaPoint[14] = 560;
	mAreaPoint[15] = 736;
	// 現在のエリア
	mNowArea[0] = 0;
	mNowArea[1] = 0;
	// 再開するエリア
	mRestartArea = 0;
	mCanAddRestarArea = true;
}

/*------------------------------------------------------------------------------
デストラクタ
------------------------------------------------------------------------------*/
Background::~Background()
{
}

/*------------------------------------------------------------------------------
更新
------------------------------------------------------------------------------*/
void Background::update()
{
	// 通常モード
	if (mMode == 0) {
		// 前フレームのカメラの位置と比較してエリアを変更させる
		if (mTmpCameraY < mCameraY) {
			mNowArea[0]++;
			mNowArea[1] = mNowArea[0] - 1;
			mCanAddRestarArea = true;
			mRestartArea = mNowArea[0];
		}
		// エリア進行度が7割以上なら再開エリアを更新
		if (mCameraY <= Define::MapHeight * 0.3 && mCanAddRestarArea) {
			mRestartArea++;
			mCanAddRestarArea = false;
		}
		if (mRestartArea > 15) {
			mRestartArea = 6;
		}
		// エリア16まで行ったらエリア7へ
		if (mNowArea[0] > 15) {
			mNowArea[0] = 6;
			mNowArea[1] = 15;
		}
		// 森を超えたかどうかの判定
		if (mIsForest && mCameraY < Define::MapHeight - Define::GameScreenHeight) {
			mIsForest = false;
		}
		mTmpCameraY = mCameraY;
	}
	// エディターシーン
	else {

	}
}

/*------------------------------------------------------------------------------
描画
------------------------------------------------------------------------------*/
void Background::draw() const
{
	// 通常モード
	if (mMode == 0) {
		// 森
		if (mIsForest) {
			DrawGraph(0, mForestY - (int)mCameraY,
				Image::getIns()->getImage(Image_Forest), TRUE);
		}
		// 背景1
		DrawGraph(-mAreaPoint[mNowArea[0]], mBackgroundY[0] - (int)mCameraY,
			Image::getIns()->getImage(Image_Background), TRUE);
		// 背景2 : 画面外の時は表示しない
		if (mBackgroundY[1] - (int)mCameraY < Define::GameScreenHeight && !mIsForest) {
			DrawGraph(-mAreaPoint[mNowArea[1]], mBackgroundY[1] - (int)mCameraY,
				Image::getIns()->getImage(Image_Background), TRUE);
		}
	}
	// エディターシーン
	else {
		// 森
		if (mForestY - (int)mCameraY < Define::GameScreenHeight) {
			DrawGraph(0, mForestY - (int)mCameraY,
				Image::getIns()->getImage(Image_Forest), TRUE);
		}
		
		// 背景1
		DrawGraph(-mAreaPoint[mNowArea[0]], mBackgroundY[0] - (int)mCameraY,
			Image::getIns()->getImage(Image_Background), TRUE);
	}
	// デバッグ
	//DrawFormatString(0, 0, GetColor(255, 255, 255), "Forest:%d Map1:%d Map2:%d\n %d",
	//	mForestY - (int)mCameraY, mBackgroundY[0], mBackgroundY[1], mRestartArea);
}

/*------------------------------------------------------------------------------
リセット
------------------------------------------------------------------------------*/
void Background::reset()
{
	// y座標
	mCameraY = float(Define::MapHeight + Define::ForestHeight - Define::GameScreenHeight);
	mTmpCameraY = mCameraY;
	mBackgroundY[0] = 0;
	mBackgroundY[1] = Define::MapHeight;
	mForestY = Define::MapHeight;
	// 森表示フラグ
	mIsForest = true;
	// エリア
	mNowArea[0] = mRestartArea;
}

/*------------------------------------------------------------------------------
現在のエリアを返す
------------------------------------------------------------------------------*/
int Background::getArea()
{
	return mNowArea[0];
}

/*------------------------------------------------------------------------------
カメラの座標をセット
------------------------------------------------------------------------------*/
void Background::setCameraY(float aY)
{
	mCameraY = aY;
}

/*------------------------------------------------------------------------------
モードをセット
------------------------------------------------------------------------------*/
void Background::setMode(int aMode) {
	mMode = aMode;
	mTmpCameraY = mCameraY;
}

/*------------------------------------------------------------------------------
エリア加算
------------------------------------------------------------------------------*/
void Background::addArea(int aAdd) {
	mNowArea[0] += aAdd;
	if (mNowArea[0] < 0) {
		mNowArea[0] = 15;
	}
	if (mNowArea[0] > 15) {
		mNowArea[0] = 0;
	}
}