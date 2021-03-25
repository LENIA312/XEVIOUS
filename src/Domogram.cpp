#include <DxLib.h>
#include "Domogram.h"
#include "Image.h"
#include "Define.h"
#include "SE.h"

/*------------------------------------------------------------------------------
コンストラクタ
------------------------------------------------------------------------------*/
Domogram::Domogram(InterfaceNPCEvent* aNPCEvent, NPCDATA aData, Player* aPlayer) : BaseNPC(aNPCEvent)
{
	mPtn = aData.ptn;
	mX = (float)aData.x;
	mY = (float)aData.y - 16;
	mPlayer = aPlayer;

	mCnt = 0;
	mAttackMode = 0;
	mSparioTimer = 0;

}

/*------------------------------------------------------------------------------
デストラクタ
------------------------------------------------------------------------------*/
Domogram::~Domogram()
{

}

/*------------------------------------------------------------------------------
更新
------------------------------------------------------------------------------*/
void Domogram::update()
{
	// 倒されていない時
	if (!mIsStriken) {
		mCnt++;

		if (mPtn >= 1) {

			if (mCnt > 40 && mAttackMode == 0 && mY <= mCameraY + Define::GameScreenHeight / 2) {
				if (GetRand(2) > 0) {
					mAttackMode = 1;
				}
				else {
					mCnt = 0;
				}
			}

			if (mAttackMode == 1) {
				mAnime += (float)0.3;
			}

			if (mAttackMode == 2) {
				mAnime -= (float)0.3;
				if (mAnime <= 0) {
					mAnime = 0;
					mAttackMode = 0;
					mCnt = 0;
				}
			}

			if (mAnime > 4) {
				mAnime = 3.5;
				mNPCEvent->generateEnemyBullet(0, 0, (int)mX + 8, (int)mY + 8);
				mAttackMode = 2;
			}
		}
		collision();
	}
	// 倒された時
	else {
		strikenEvent();
	}
	// 画面外に出たらフラグを折る
	if (mY > mCameraY + Define::GameScreenHeight) {
		mActiveFlag = false;
	}
}

/*------------------------------------------------------------------------------
 描画
------------------------------------------------------------------------------*/
void Domogram::draw() const
{
	// 通常
	if (!mIsStriken) {
		switch (mColor) {
		case 0:
			DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_a, 64 + (int)mAnime), true);
			break;
		case 1:
			DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_b, 64 + (int)mAnime), true);
			break;
		case 2:
			DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_c, 64 + (int)mAnime), true);
			break;
		case 3:
			DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_d, 64 + (int)mAnime), true);
			break;
		case 4:
			DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_e, 64 + (int)mAnime), true);
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
}

/*------------------------------------------------------------------------------
倒されたとき時
------------------------------------------------------------------------------*/
void Domogram::strikenEvent()
{
	mAnime += (float)0.2;
	if (mAnime >= 8) {
		mAnime = 6;
	}
}

/*------------------------------------------------------------------------------
ブラスターに当たった時
------------------------------------------------------------------------------*/
void Domogram::collisionGameObject(eTag aTag)
{
	if (aTag == Tag_Blaster && !mIsStriken) {
		// 倒されたフラグを立てる
		mIsStriken = true;
		// アニメ位置を初期化
		mAnime = 0;
		// スコアを加算する
		mNPCEvent->addScore(800);
		// 効果音再生
		SE::getIns()->playSE(SE_EnemyStrikenBlaster);
	}
}

/*------------------------------------------------------------------------------
ブラスターとの当たり判定
------------------------------------------------------------------------------*/
void Domogram::collision()
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
int Domogram::getCollisionPoint(int aPos) const
{
	switch (aPos) {
	case 0: // x
		return (int)mX + 1;
	case 1: // y
		return (int)mY + 1;
	case 2: // w
		return 14;
	case 3: // h
		return 14;
	}
	return -1;
}

/*------------------------------------------------------------------------------
タグを返す
------------------------------------------------------------------------------*/
eTag Domogram::getTag() const
{
	return Tag_Enemy_Ground;
}