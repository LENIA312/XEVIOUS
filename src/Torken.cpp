#include <DxLib.h>
#include "Torken.h"
#include "Image.h"
#include "Define.h"
#include "SE.h"

/*------------------------------------------------------------------------------
コンストラクタ
------------------------------------------------------------------------------*/
Torken::Torken(InterfaceNPCEvent* aNPCEvent, NPCDATA aData, Player* aPlayer) : BaseNPC(aNPCEvent)
{
	mPtn = aData.ptn;
	mX = (float)aData.x;
	mY = (float)aData.y - 16;
	mPlayer = aPlayer;

	mAcceleFlg = false;
	mWaitFlg = false;

	mTt = 0;
	mStopTime = GetRand(40) + 10;
	mTimer = 0;


	// 位置初期化
	init();
}

/*------------------------------------------------------------------------------
デストラクタ
------------------------------------------------------------------------------*/
Torken::~Torken()
{

}

/*------------------------------------------------------------------------------
更新
------------------------------------------------------------------------------*/
void Torken::update()
{
	// 移動

	mVx = cos(mTheta);
	mVy = sin(mTheta);

	mX = mX - mVx * mSpeed;
	mY = mY - mVy * mSpeed;

	//一定範囲で停止 ,射撃可能状態に
	if (mTimer >= mStopTime) {
		mSpeed = 0;			        //プレイヤーの近く、または横のラインで停止
		mWaitFlg = true;

	}
	mTimer++;

	if (mWaitFlg) {
		
		mAnime += 0.2;
		if (mAnime >= 6) {

		    mAcceleFlg = true;

			mWaitFlg = false;

			mAnime = 6;

			mTheta = atan2(mY - mPlayer->getXY(2), mX - mPlayer->getXY(0));
		}


	}
	

	if (mAcceleFlg) {
		mTt++;


		switch (mPtn) {
		case 0:

			break;
		case 1:
			if (mTt == 1) {
				mNPCEvent->generateEnemyBullet(0, 0, (int)mX + 8, (int)mY + 8);
			}
			break;
		case 2:
			if (mTt != true) {
				mNPCEvent->generateEnemyBullet(0, 0, (int)mX + 8, (int)mY + 8);

			}
			break;
		}

		mX = mX - mVx * mSpeedR;
		mY = mY - mVy * mSpeedR;

	}




		// 倒されていない時
		if (!mIsStriken) {
			collision();
		}
		// 倒されたら
		else {
			strikenEvent();
		}
		// 画面外に出たらフラグを折る
		if (mX + 16 < 0 ||
			mX > Define::GameScreenWidth - 128 ||
			mY > mCameraY + Define::GameScreenHeight) {
			mActiveFlag = false;
		
	}
}

/*------------------------------------------------------------------------------
 描画
------------------------------------------------------------------------------*/
void Torken::draw() const
{
	// 通常
	if (!mIsStriken) {
		switch (mColor) {
		case 0:
			DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_a, 16 + (int)mAnime), true);
			break;
		case 1:
			DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_b, 16 + (int)mAnime), true);
			break;
		case 2:
			DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_c, 16 + (int)mAnime), true);
			break;
		case 3:
			DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_d, 16 + (int)mAnime), true);
			break;
		case 4:
			DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_e, 16 + (int)mAnime), true);
			break;
		}
	}
	// 破壊
	else {
		DrawGraph((int)mX - 8, ((int)mY - 8) - (int)mCameraY, Image::getIns()->getImage(Image_EnemyStriken, (int)mAnime), true);
	}
	// デバッグ
	//clsDx();
	//printfDx("x%0.1f y%0.1f",mSpeedX, mSpeedY);
	//DrawLine(mPlayerX, 0, mPlayerX, 300, GetColor(0, 0, 0));
}

/*------------------------------------------------------------------------------
出現位置の初期化
------------------------------------------------------------------------------*/
void Torken::init()
{
	mX = GetRand(240) + 64;		//敵のx座標
	mSpeed = 3;
	mSpeedR = mSpeed * -1;
	mTheta = atan2(mY - mPlayer->getXY(2), mX - mPlayer->getXY(0));
}

/*------------------------------------------------------------------------------
倒されたとき時
------------------------------------------------------------------------------*/
void Torken::strikenEvent()
{
	mAnime += (float)0.8;
	if (mAnime > 5) {
		mActiveFlag = false;
	}
}

/*------------------------------------------------------------------------------
ザッパーに当たった時
------------------------------------------------------------------------------*/
void Torken::collisionGameObject(eTag aTag)
{
	// 当たったオブジェクトがザッパーなら
	if (aTag == Tag_Zapper && !mIsStriken) {
		// 倒されたフラグを立てる
		mIsStriken = true;
		// アニメ位置を初期化
		mAnime = 0;
		// スコアを加算する
		mNPCEvent->addScore(30);
		// 効果音再生
		SE::getIns()->playSE(SE_EnemyStrikenZapper);
	}
}

/*------------------------------------------------------------------------------
ザッパーとの当たり判定
------------------------------------------------------------------------------*/
void Torken::collision()
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
int Torken::getCollisionPoint(int aPos) const
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
eTag Torken::getTag() const
{
	return Tag_Enemy_Sky;
}
