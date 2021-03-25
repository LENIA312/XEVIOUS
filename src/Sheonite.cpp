#include <DxLib.h>
#include<math.h>
#include "Sheonite.h"
#include "Image.h"
#include "Define.h"
#include "SE.h"
/*------------------------------------------------------------------------------
コンストラクタ
------------------------------------------------------------------------------*/
Sheonite::Sheonite(InterfaceNPCEvent* aNPCEvent, NPCDATA aData, Player* aPlayer) : BaseNPC(aNPCEvent)
{
	mPtn = aData.ptn;
	mX[0] = (float)aData.x;
	mY[0] = (float)aData.y - 16;
	mPlayer = aPlayer;

	mSpeed = -1; // 移動スピード
	mMotionNum = 0; // 現在のモーション番号
	mTT = 0; // タイマー
	mAnimSpeed = 0.3; // アニメーション再生速度
	mTransFlg[0] = mTransFlg[1] = false;
	mFlyCnt = 0;

	// 位置初期化
	init();
}

/*------------------------------------------------------------------------------
デストラクタ
------------------------------------------------------------------------------*/
Sheonite::~Sheonite()
{

}

/*------------------------------------------------------------------------------
更新
------------------------------------------------------------------------------*/
void Sheonite::update()
{
	// プレイヤー座標
	int px = mPlayer->getCollisionPoint(0);
	int py = mPlayer->getCollisionPoint(1);
	int pw = mPlayer->getCollisionPoint(2);
	int ph = mPlayer->getCollisionPoint(3);

	//***************************移動***************************************

	switch (mMotionNum)
	{
	case 0:
		// 1号機の処理
		if (py - ph > mY[0]) {
			// 自機とのベクトルを計算
			mTheta[0] = atan2((py - ph) - mY[0], (px + pw) - mX[0]);
			mVx[0] = cos(mTheta[0]);
			mVy[0] = sin(mTheta[0]);
			//自機の方向へ移動
			mX[0] = mX[0] + mVx[0] * mSpeed;
			mY[0] = mY[0] + mVy[0] * mSpeed;
		}else {
			mTransFlg[0] = true; //くっついたフラグをオン
			mX[0] = px + pw; mY[0] = py - ph; // 座標をセット
		}

		// 2号機の処理
		if (py - ph > mY[1]) {
			// 自機とのベクトルを計算
			mTheta[1] = atan2((py - ph) - mY[1], (px - pw) - mX[1]);
			mVx[1] = cos(mTheta[1]);
			mVy[1] = sin(mTheta[1]);
			//自機の方向へ移動
			mX[1] = mX[1] + mVx[1] * mSpeed;
			mY[1] = mY[1] + mVy[1] * mSpeed;
		}else {
			mTransFlg[1] = true; //くっついたフラグをオン
			mX[1] = px - pw;mY[1] = py - ph;
		}

		// 2つとも時期に合体したら
		if (mTransFlg[0] && mTransFlg[1]) {
			mMotionNum = 1; // 次のモーションへ
		}
		break;
	case 1:
		// タイマーを起動
		mTT++;

		// 座標をセット
		mX[0] = px + pw;mY[0] = py - ph;
		mX[1] = px - pw;mY[1] = py - ph;

		// 600フレーム後に
		if (mTT == 600) {
			mAnimMax = 4; // アニメーションの最大値
			mAnimSpeed = 0.1; // アニメーションの再生速度
			mAnime = 0; 
			mAnimP[0] = 10;mAnimP[1] = 18; // 再生するアニメーション番号
		}

		if (40 + mAnimP[0] + mAnime == 54) {
			mMotionNum = 2; // 次のモーションへ
			mTT = 0; // タイマーの初期化
			mAnimSpeed = 0; // アニメーションの再生速度
		}
		break;
	case 2:
		mBetX++;
		mY[0] = mY[1]= py - ph;
		mX[0] = px + pw - mBetX;
		mX[1] = px - pw + mBetX;

		if (mX[0] == px) {
			mMotionNum = 3;// 次のモーションへ
			mAnime = 0;
		}
		break;
	case 3:
		mAnimP[0] = 8;mAnimP[1] = 16; // 再生するアニメーション番号
		mAnimMax = 2; // アニメーションの最大値
		mAnimSpeed = 0.5; // アニメーションの再生速度
		// 飛んでいく
		mY[0] -= mSpeed;
		mY[1] = mSpeed;
		mFlyCnt++;
		break;
	default:
		break;
	}

	// 撤退のSEを再生する
	if (mFlyCnt == 1)SE::getIns()->playSE(SE_ShioniteDocking);

	//アニメ再生
	mAnime += mAnimSpeed;
	if (mAnime >= mAnimMax) {
		mAnime = 0;
	}

	// 画面外に出たらフラグを折る
	if (mY[0] + 16 < mCameraY) {
		mActiveFlag = false;
	}
}

/*------------------------------------------------------------------------------
 描画
------------------------------------------------------------------------------*/
void Sheonite::draw() const
{
	// 色によって表示する画像を変える
	if (!mIsStriken) {
		for (int i = 0; i < 2; i++) {
				switch (mColor) {
				case 0:
					DrawGraph((int)mX[i], (int)mY[i] - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_a, (40 + mAnimP[i]) + (int)mAnime), true);
					break;
				case 1:
					DrawGraph((int)mX[i], (int)mY[i] - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_b, (40 + mAnimP[i]) + (int)mAnime), true);
					break;
				case 2:
					DrawGraph((int)mX[i], (int)mY[i] - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_c, (40 + mAnimP[i]) + (int)mAnime), true);
					break;
				case 3:
					DrawGraph((int)mX[i], (int)mY[i] - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_d, (40 + mAnimP[i]) + (int)mAnime), true);
					break;
				case 4:
					DrawGraph((int)mX[i], (int)mY[i] - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_e, (40 + mAnimP[i]) + (int)mAnime), true);
					break;
				}
		}
	}
}

/*------------------------------------------------------------------------------
初期化
------------------------------------------------------------------------------*/
void Sheonite::init()
{
	// 縦スピードをセット
	mSpeed = 7;
	mAnimMax = 4;
	//2号をセット
	mX[1] = mX[0] + 100;
	mY[1] = mY[0];
	mBetX = 0;
	//アニメ再生位置をセット
	mAnimP[0] = 0;mAnimP[1] = 4;
}

/*------------------------------------------------------------------------------
当たり判定座標を返す
------------------------------------------------------------------------------*/
int Sheonite::getCollisionPoint(int aPos) const
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
eTag Sheonite::getTag() const
{
	return Tag_None;
}