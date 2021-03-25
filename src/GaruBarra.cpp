#include <DxLib.h>
#include "GaruBarra.h"
#include "Image.h"
#include "Define.h"
#include "SE.h"

/*------------------------------------------------------------------------------
コンストラクタ
------------------------------------------------------------------------------*/
GaruBarra::GaruBarra(InterfaceNPCEvent* aNPCEvent, NPCDATA aData, Player* aPlayer) : BaseNPC(aNPCEvent)
{
	mPtn = aData.ptn;
	mX = (float)aData.x;
	mY = (float)aData.y - 32;
	mPlayer = aPlayer;
}

/*------------------------------------------------------------------------------
デストラクタ
------------------------------------------------------------------------------*/
GaruBarra::~GaruBarra()
{

}

/*------------------------------------------------------------------------------
更新
------------------------------------------------------------------------------*/
void GaruBarra::update()
{
	// 倒されていない時
	if (!mIsStriken) {
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
void GaruBarra::draw() const
{
	// ガルバーラ
	switch (mColor) {
	case 0:
		DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_EnemyBig_a, 1), true);
		break;
	case 1:
		DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_EnemyBig_b, 1), true);
		break;
	case 2:
		DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_EnemyBig_c, 1), true);
		break;
	case 3:
		DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_EnemyBig_d, 1), true);
		break;
	case 4:
		DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_EnemyBig_d, 1), true);
		break;
	}
	// バーラ
	if (!mIsStriken) {
		switch (mColor) {
		case 0:
			DrawGraph((int)mX + 8, (int)mY + 8 - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_a, 23), true);
			break;
		case 1:
			DrawGraph((int)mX + 8, (int)mY + 8 - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_b, 23), true);
			break;
		case 2:
			DrawGraph((int)mX + 8, (int)mY + 8 - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_c, 23), true);
			break;
		case 3:
			DrawGraph((int)mX + 8, (int)mY + 8 - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_d, 23), true);
			break;
		case 4:
			DrawGraph((int)mX + 8, (int)mY + 8 - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_e, 23), true);
			break;
		}
	}
	// 破壊
	if (mIsStriken) {
		if (mAnime < 6) {
			DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_EnemyStriken, 6 + (int)mAnime), true);
		}
	}

	//DrawFormatString(0, 0, GetColor(255, 255, 255), "nyoki:%d so:%d", nyokiFlg, strikeOn);
}

/*------------------------------------------------------------------------------
倒されたとき時
------------------------------------------------------------------------------*/
void GaruBarra::strikenEvent()
{
	mAnime += (float)0.2;
	if (mAnime >= 8) {
		mAnime = 6;
	}
}

/*------------------------------------------------------------------------------
ブラスターに当たった時
------------------------------------------------------------------------------*/
void GaruBarra::collisionGameObject(eTag aTag)
{
	if (aTag == Tag_Blaster && !mIsStriken) {
		// 倒されたフラグを立てる
		mIsStriken = true;
		// アニメ位置を初期化
		mAnime = 0;
		// スコアを加算する
		mNPCEvent->addScore(300);
		// 効果音再生
		SE::getIns()->playSE(SE_EnemyStrikenBlaster);
	}
}

/*------------------------------------------------------------------------------
ブラスターとの当たり判定
------------------------------------------------------------------------------*/
void GaruBarra::collision()
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
int GaruBarra::getCollisionPoint(int aPos) const
{
	switch (aPos) {
	case 0: // x
		return (int)mX + 8;
	case 1: // y
		return (int)mY + 8;
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
eTag GaruBarra::getTag() const
{
	return Tag_Enemy_Ground;
}
