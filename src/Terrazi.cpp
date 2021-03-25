#include <DxLib.h>
#include "Terrazi.h"
#include "Image.h"
#include "Define.h"
#include "SE.h"

/*------------------------------------------------------------------------------
コンストラクタ
------------------------------------------------------------------------------*/
Terrazi::Terrazi(InterfaceNPCEvent* aNPCEvent, NPCDATA aData, Player* aPlayer) : BaseNPC(aNPCEvent)
{
	mX = (float)aData.x;
	mY = (float)aData.y - 16;
	mPtn = aData.ptn;
	mPlayer = aPlayer;

	mSpeedX = -1;
	mSpeedY = -1;
	mLRFlg = false;
	mAcceleFlg = false;
	mShotFlg = false;

	// 位置の初期化
	init();
}

/*------------------------------------------------------------------------------
デストラクタ
------------------------------------------------------------------------------*/
Terrazi::~Terrazi()
{

}

/*------------------------------------------------------------------------------
更新
------------------------------------------------------------------------------*/
void Terrazi::update()
{
	// プレイヤー座標
	int px = mPlayer->getCollisionPoint(0);
	int pw = mPlayer->getCollisionPoint(2);
	// 移動
	{
		mX += mSpeedX;
		mY += mSpeedY;
		// プレイヤーと軸が重なったら、またはザッパーに当たりそうになったら加速フラグを立てる
		if (!mAcceleFlg && !mLRFlg && mX + 16 >= px && !mIsStriken ||
			!mAcceleFlg && mLRFlg && mX <= px + pw && !mIsStriken) {

			mSpeedX *= 0.5;
			mSpeedY *= 0.5;
			if (mSpeedX <= 0.02) {
				mAcceleFlg = true;
				mShotFlg = true;
			}
			// パターン1ならスパリオ弾発射
			if (mPtn >= 1 && mShotFlg) {
				for (int i = 0; i < 3; i++) {

					mNPCEvent->generateEnemyBullet(0, 0, (int)mX + GetRand(15) + 9, (int)mY + GetRand(15) + 8);

				}
			

			}
		}
		// 加速フラグが立っている時
		if (mAcceleFlg) {
			// 左向き
			if (!mLRFlg) {
				mSpeedX -= (float)0.15;
				mSpeedY -= (float)0.2;
				// アニメ
				if (!mIsStriken) {
					mAnime += (float)0.1;
					if (mAnime >= 7) {
						mAnime = 0;
					}
				}
			}
			// 右向き
			else {
				mSpeedX += (float)0.15;
				mSpeedY -= (float)0.2;
				// アニメ
				if (!mIsStriken) {
					mAnime -= (float)0.1;
					if (mAnime <= -1) {
						mAnime = 6;
					}
				}
			}
		}
	}

	// 倒されていない時
	if (!mIsStriken) {
		collision();
	}
	// 倒された時
	else {
		strikenEvent();
	}
	// 画面外に出たらフラグを折る
	if (mX + 16 < 0 ||
		mX > Define::GameScreenWidth - 128 ||
		mY + 16 < mCameraY ||
		mY > mCameraY + Define::GameScreenHeight) {
		mActiveFlag = false;
	}

}

/*------------------------------------------------------------------------------
 描画
------------------------------------------------------------------------------*/
void Terrazi::draw() const
{
	// 通常
	if (!mIsStriken) {
		switch (mColor) {
		case 0:
			DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_a, (int)mAnime), true);
			break;
		case 1:
			DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_b, (int)mAnime), true);
			break;
		case 2:
			DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_c, (int)mAnime), true);
			break;
		case 3:
			DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_d, (int)mAnime), true);
			break;
		case 4:
			DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_e, (int)mAnime), true);
			break;
		}
	}
	// 破壊
	else {
		DrawGraph((int)mX - 8, ((int)mY - 8) - (int)mCameraY, Image::getIns()->getImage(Image_EnemyStriken, (int)mAnime), true);
	}
}

/*------------------------------------------------------------------------------
出現位置の初期化
------------------------------------------------------------------------------*/
void Terrazi::init()
{
	int pcx = mPlayer->getCollisionPoint(0) + mPlayer->getCollisionPoint(2) / 2;
	// どこから出現するかをプレイヤーの位置で決める
	{
		// 左側にセット
		if (pcx >= 208) {
			mLRFlg = false;
		}
		// 右側にセット
		else if (pcx < 144) {
			mLRFlg = true;
		}
		// 左右ランダム
		if (pcx >= 144 && pcx < 208) {
			mLRFlg = GetRand(1);
		}
	}
	// 初期位置とスピードをセット
	{
		// 左側の場合
		if (!mLRFlg) {
			mX = (float)(GetRand(64) + 48);
			mSpeedX = /*GetRand(0.3) + 2*/2;
		}
		// 右側の場合
		else {
			mX = (float)(GetRand(64) + 240);
			mSpeedX = 2;
			mSpeedX *= -1;
		}
		// 縦スピード
		mSpeedY = 22;
		mSpeedY *= (float)0.1;
	}
}

/*------------------------------------------------------------------------------
倒されたとき時
------------------------------------------------------------------------------*/
void Terrazi::strikenEvent()
{
	mAnime += (float)0.8;
	if (mAnime > 5) {
		mActiveFlag = false;
	}
}

/*------------------------------------------------------------------------------
ザッパーに当たった時
------------------------------------------------------------------------------*/
void Terrazi::collisionGameObject(eTag aTag)
{
	if (aTag == Tag_Zapper && !mIsStriken) {
		// 倒されたフラグを立てる
		mIsStriken = true;
		// アニメ位置を初期化
		mAnime = 0;
		// スコアを加算する
		mNPCEvent->addScore(700);
		// 効果音再生
		SE::getIns()->playSE(SE_EnemyStrikenZapper);
	}
}

/*------------------------------------------------------------------------------
ザッパーとの当たり判定
------------------------------------------------------------------------------*/
void Terrazi::collision()
{
	// 敵の座標
	int ex = getCollisionPoint(0);
	int ey = getCollisionPoint(1);
	int ew = getCollisionPoint(2);
	int eh = getCollisionPoint(3);
	// ザッパーが敵に当たった時
	Zapper* zapper = mPlayer->getZapper();
	for (int i = 0; i < Define::ZapperMax; i++) {
		if (zapper[i].checkActiveFlag()) {
			int zx = zapper[i].getCollisionPoint(0);
			int zy = zapper[i].getCollisionPoint(1);
			int zw = zapper[i].getCollisionPoint(2);
			int zh = zapper[i].getCollisionPoint(3);
			if (zx + zw >= ex && zx <= ex + ew &&
				zy + zh >= ey && zy <= ey + eh) {
				collisionGameObject(zapper[i].getTag());
				zapper[i].collisionGameObject(getTag());
			}
		}
	}
}

/*------------------------------------------------------------------------------
当たり判定座標を返す
------------------------------------------------------------------------------*/
int Terrazi::getCollisionPoint(int aPos) const
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
eTag Terrazi::getTag() const
{
	return Tag_Enemy_Sky;
}