#include <DxLib.h>
#include "Zapper.h"
#include "Image.h"
#include "SE.h"

/*------------------------------------------------------------------------------
定義
------------------------------------------------------------------------------*/
static const float ZAPPER_SPEED = 7; // memo:7

/*------------------------------------------------------------------------------
コンストラクタ
------------------------------------------------------------------------------*/
Zapper::Zapper()
{
	mActiveFlag = false;
}

/*------------------------------------------------------------------------------
デストラクタ
------------------------------------------------------------------------------*/
Zapper::~Zapper()
{

}

/*------------------------------------------------------------------------------
更新
------------------------------------------------------------------------------*/
void Zapper::update()
{
	if (mActiveFlag) {
		mY -= ZAPPER_SPEED;
		// アニメーション
		mAnime = (float)GetRand(7);
		// 画面外に行ったらフラグをオフ
		if (mY < -16) {
			mActiveFlag = false;
		}
	}
}

/*------------------------------------------------------------------------------
描画
------------------------------------------------------------------------------*/
void Zapper::draw() const
{
	if (mActiveFlag) {
		DrawGraph((int)mX, (int)mY, Image::getIns()->getImage(Image_Zapper, (int)mAnime), TRUE);
	}
}

/*------------------------------------------------------------------------------
当たり判定座標を返す
------------------------------------------------------------------------------*/
int Zapper::getCollisionPoint(int aPos) const
{
	switch (aPos) {
	case 0: // x
		return (int)mX;
	case 1: // y
		return (int)mY + (int)mCameraY;
	case 2: // w
		return 16;
	case 3: // h
		return 16;
	}
	return -1;
}

/*------------------------------------------------------------------------------
他のゲームオブジェクトとの当たり判定
------------------------------------------------------------------------------*/
void Zapper::collisionGameObject(eTag aTag)
{
	switch (aTag) {
	case Tag_Enemy_Sky:
		mActiveFlag = false;
		break;
	}
}

/*------------------------------------------------------------------------------
タグを返す
------------------------------------------------------------------------------*/
eTag Zapper::getTag() const
{
	return Tag_Zapper;
}

/*------------------------------------------------------------------------------
ザッパーを生成する
------------------------------------------------------------------------------*/
void Zapper::generate(int aX, int aY)
{
	// 座標をセット
	mX = (float)aX;
	mY = (float)aY;
	// フラグをオン
	mActiveFlag = true;
	// 効果音再生
	SE::getIns()->playSE(SE_Zapper);
}