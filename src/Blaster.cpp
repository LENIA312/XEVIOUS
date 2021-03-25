#include <DxLib.h>
#include "Blaster.h"
#include "Image.h"
#include "SE.h"
#include "Define.h"

/*
// エリアの変わり目に当たり判定を移動
if (by <= Define::GAME_H && mCameraY > Define::MAP_HEIGHT - Define::GAME_H) {
	by += Define::MAP_HEIGHT;
}
*/

/*------------------------------------------------------------------------------
定義
------------------------------------------------------------------------------*/
const static float BLASTER_SPEED = (float)0.15; // memo:0.15
const static float BLASTER_ANIME_SPEED = (float)0.26;

/*------------------------------------------------------------------------------
コンストラクタ
------------------------------------------------------------------------------*/
Blaster::Blaster()
{
	mBlasterSpeed = 0;
	mTargetY = 0;

	mIsHit = false;
	mActiveFlag = false;
}

/*------------------------------------------------------------------------------
デストラクタ
------------------------------------------------------------------------------*/
Blaster::~Blaster()
{

}

/*------------------------------------------------------------------------------
更新
------------------------------------------------------------------------------*/
void Blaster::update()
{
	// ヒットしたらフラグをオフに
	if (mIsHit) {
		mIsHit = false;
		mActiveFlag = false;
	}
	if (mActiveFlag) {
		// ターゲットを超えるまで
		if (mY > mTargetY) {
			// 移動
			{
				mY -= mBlasterSpeed;
				mBlasterSpeed += BLASTER_SPEED;
				// 補正
				if (mY < mTargetY) {
					mY = mTargetY;
				}
			}
			// アニメ
			{
				mAnime += BLASTER_ANIME_SPEED;
				// 補正
				if (mAnime > 8) {
					mAnime = 8;
				}
			}
			// エリアの変わり目に座標を補正する
			if (mTargetY <= Define::GameScreenHeight && mCameraY > Define::MapHeight - Define::GameScreenHeight) {
				mTargetY += Define::MapHeight;
			}
			if (mY <= Define::GameScreenHeight && mCameraY > Define::MapHeight - Define::GameScreenHeight) {
				mY += Define::MapHeight;
			}
		}
		// ターゲットを超えたら
		else {
			mY = mTargetY;
			mIsHit = true;
		}
	}
}

/*------------------------------------------------------------------------------
描画
------------------------------------------------------------------------------*/
void Blaster::draw() const
{
	if (mActiveFlag) {
		// ターゲット照準
		DrawGraph((int)mX, (int)mTargetY - (int)mCameraY,
			Image::getIns()->getImage(Image_Aiming, 4), TRUE);
		// ブラスター
		DrawGraph((int)mX, (int)mY - (int)mCameraY,
			Image::getIns()->getImage(Image_Blaster, (int)mAnime), TRUE);
	}
}

/*------------------------------------------------------------------------------
当たり判定座標を返す
------------------------------------------------------------------------------*/
int Blaster::getCollisionPoint(int aPos) const
{
	switch (aPos) {
	case 0: // x
		return (int)mX;
	case 1: // y
		return (int)mY;
	case 2: // w
		return 12;
	case 3: // h
		return 12;
	}
	return -1;
}

/*------------------------------------------------------------------------------
他のゲームオブジェクトとの当たり判定
------------------------------------------------------------------------------*/
void Blaster::collisionGameObject(eTag aTag)
{

}

/*------------------------------------------------------------------------------
タグを返す
------------------------------------------------------------------------------*/
eTag Blaster::getTag() const
{
	return Tag_Blaster;
}

/*------------------------------------------------------------------------------
ブラスターを生成する
------------------------------------------------------------------------------*/
void Blaster::generate(int aX, int aY)
{
	// 座標をセット
	mX = (float)aX;
	mY = (float)aY + mCameraY;
	mTargetY = mY - 96;
	// フラグをオン
	mBlasterSpeed = 0;
	mActiveFlag = true;
	mIsHit = false;
	mAnime = 0;
	// 効果音再生
	SE::getIns()->playSE(SE_Blaster);
}

/*------------------------------------------------------------------------------
ヒットフラグを返す
------------------------------------------------------------------------------*/
bool Blaster::getHitFlag()
{
	return mIsHit;
}