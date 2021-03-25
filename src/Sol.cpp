#include <DxLib.h>
#include "Sol.h"
#include "Image.h"
#include "Define.h"
#include "SE.h"

/*------------------------------------------------------------------------------
コンストラクタ
------------------------------------------------------------------------------*/
Sol::Sol(InterfaceNPCEvent* aNPCEvent, NPCDATA aData, Player* aPlayer) : BaseNPC(aNPCEvent)
{
	mPtn = aData.ptn;
	mX = (float)aData.x;
	mY = (float)aData.y - 16;
	mPlayer = aPlayer;
	hitCnt = 0;

	appearFlg = false;
	strikeOn = false;

}

/*------------------------------------------------------------------------------
デストラクタ
------------------------------------------------------------------------------*/
Sol::~Sol()
{

}

/*------------------------------------------------------------------------------
更新
------------------------------------------------------------------------------*/
void Sol::update()
{
	// 倒されていない時
	if (!mIsStriken) {
		collision();
	}
	// 倒された時
	else {
		strikenEvent();
	}

	if (appearFlg) {
		if (!mIsStriken) {
			mAnime += (float)0.1;
			if (mAnime >= 7) {
				mAnime = 7;
				strikeOn = true;
			}
		}
	}


	// 画面外に出たらフラグを折る
	if (mY > mCameraY + Define::GameScreenHeight) {
		mActiveFlag = false;
	}


}

/*------------------------------------------------------------------------------
 描画
------------------------------------------------------------------------------*/
void Sol::draw() const
{
	// 通常
	if (!mIsStriken) {
		switch (mColor) {
		case 0:
			DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_EnemyBig_a,7 + (int)mAnime), true);
			break;
		case 1:
			DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_EnemyBig_b, 7 + (int)mAnime), true);
			break;
		case 2:
			DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_EnemyBig_c, 7 + (int)mAnime), true);
			break;
		case 3:
			DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_EnemyBig_d, 7 + (int)mAnime), true);
			break;
		case 4:
			DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_EnemyBig_d, 7 + (int)mAnime), true);
			break;
		}
	}
	// 破壊
	else {
		if (mAnime < 6) {
			DrawGraph((int)mX - 8, ((int)mY - 8) - (int)mCameraY, Image::getIns()->getImage(Image_EnemyStriken, 6 + (int)mAnime), true);
		}
		if (mAnime >= 6) {
			DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Destructed, (int)mAnime - 6), true);
		}
	}

	//DrawFormatString(0, 0, GetColor(255, 255, 255), "nyoki:%d so:%d", appearFlg, strikeOn);
}

/*------------------------------------------------------------------------------
倒されたとき時
------------------------------------------------------------------------------*/
void Sol::strikenEvent()
{
	mAnime += (float)0.2;
	if (mAnime >= 8) {
		mAnime = 6;
	}
}

/*------------------------------------------------------------------------------
ブラスターに当たった時
------------------------------------------------------------------------------*/
void Sol::collisionGameObject(eTag aTag)
{
	//見つけられたとき
	if (aTag == Tag_Blaster && !mIsStriken &&!appearFlg) {
		appearFlg = true;
		mNPCEvent->addScore(1000);
	}
	if (aTag == Tag_Blaster && !mIsStriken && strikeOn) {
			// 倒されたフラグを立てる
			mIsStriken = true;
			// アニメ位置を初期化
			mAnime = 0;
			// スコアを加算する
			mNPCEvent->addScore(2000);
			// 効果音再生
			SE::getIns()->playSE(SE_EnemyStrikenBlaster);	
	}
}

/*------------------------------------------------------------------------------
ブラスターとの当たり判定
------------------------------------------------------------------------------*/
void Sol::collision()
{
	// 敵の座標
	int ex = getCollisionPoint(0);
	int ey = getCollisionPoint(1);
	int ew = getCollisionPoint(2);
	int eh = getCollisionPoint(3);
	// ブラスターが敵に当たった時
	{
		Blaster* blaster = mPlayer->getBlaster();
		if (blaster->getHitFlag()) {
			int bx = blaster->getCollisionPoint(0);
			int by = blaster->getCollisionPoint(1);
			int bw = blaster->getCollisionPoint(2);
			int bh = blaster->getCollisionPoint(3);
			if (bx + bw >= ex && bx <= ex + ew &&
				by + bh >= ey && by <= ey + eh) {
				collisionGameObject(blaster->getTag());
				blaster->collisionGameObject(getTag());
			}
		}
	}
	// 照準チカチカ
	{
		// 照準座標
		Blaster* blaster = mPlayer->getBlaster();
		int bx = mPlayer->getAimngXY(0);
		int by = mPlayer->getAimngXY(1);
		int bw = blaster->getCollisionPoint(2);
		int bh = blaster->getCollisionPoint(3);
		// 照準が敵に当たった時
		if (bx + bw >= ex && bx <= ex + ew &&
			by + bh >= ey && by <= ey + eh && mPlayer->checkActiveFlag()) {
			mPlayer->setTikaTikaFlg(true);
		}
	}
}

/*------------------------------------------------------------------------------
当たり判定座標を返す
------------------------------------------------------------------------------*/
int Sol::getCollisionPoint(int aPos) const
{
	switch (aPos) {
	case 0: // x
		return (int)mX;
	case 1: // y
		return (int)mY;
	case 2: // w
		return 16;
	case 3: // h
		return 16;
	}
	return -1;
}

/*------------------------------------------------------------------------------
タグを返す
------------------------------------------------------------------------------*/
eTag Sol::getTag() const
{
	return Tag_Enemy_Ground;
}
