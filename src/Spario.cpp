#include <DxLib.h>
#include "Spario.h"
#include "Image.h"
#include "Define.h"
#include "SE.h"

/*------------------------------------------------------------------------------
コンストラクタ
------------------------------------------------------------------------------*/
Spario::Spario(InterfaceNPCEvent* aNPCEvent, NPCDATA aData, Player* aPlayer) : BaseNPC(aNPCEvent)
{
	mPtn = aData.ptn;
	mX = (float)aData.x - 8;
	mY = (float)aData.y - 8;
	mPlayer = aPlayer;

	mInitFlg = false;
	mSpeed = 0;

	// プレイヤーとの角度
	float pai = (float)3.14;
	mTheta = 0;
	if (mPtn <= 5) {
		mTheta = atan2(mPlayer->getXY(2) - mY, mPlayer->getXY(0) - mX);
		switch (mPtn) {
		case 1:
			mTheta -= 60 * (pai / 180);
			break;
		case 2:
			mTheta -= 30 * (pai / 180);
			break;
		case 3:

			break;
		case 4:
			mTheta += 60 * (pai / 180);
			break;
		case 5:
			mTheta += 30 * (pai / 180);
			break;
		}
	}
	else {
		for (int i = 0; i < 16; i++) {
			if (mPtn - 6 == i) {
				mTheta = (i * (float)22.5) * (pai / 180);
				break;
			}
		}
	}
	if (mPtn == 0) {
		mSpeed = (float)1.9;
	}
	else if (mPtn >= 1) {
		mSpeed = (float)2.5;
	}
	else {
		mSpeed = 4;
	}
}

/*------------------------------------------------------------------------------
デストラクタ
------------------------------------------------------------------------------*/
Spario::~Spario()
{

}

/*------------------------------------------------------------------------------
更新
------------------------------------------------------------------------------*/
void Spario::update()
{
	// 初期化
	if (!mInitFlg) {
		mInitFlg = true;
		mY -= mCameraY;
	}
	// 動き
	mX += cos(mTheta) * mSpeed;
	mY += sin(mTheta) * mSpeed;
	// アニメーション
	mAnime += (float)0.2;
	if (mAnime >= 4) {
		mAnime = 0;
	}
	// 画面外に出たらフラグを折る
	if (mX + 16 < 0 ||
		mX > Define::GameScreenWidth - 128 ||
		mY + 16 < 0 ||
		mY > Define::GameScreenHeight) {
		mActiveFlag = false;
	}
}

/*------------------------------------------------------------------------------
 描画
------------------------------------------------------------------------------*/
void Spario::draw() const
{
	DrawGraph((int)mX, (int)mY, Image::getIns()->getImage(Image_EnemyBullet, (int)mAnime), true);
	// デバッグ
	//DrawFormatString(0, 0, GetColor(255, 255, 255), "theta:%0.1f px:%d", mTheta, mPlayerX);
}

/*------------------------------------------------------------------------------
エリアの変わり目に当たり判定を移動する
------------------------------------------------------------------------------*/
void Spario::areaChange()
{
	// 何もしない
}

/*------------------------------------------------------------------------------
当たり判定座標を返す
------------------------------------------------------------------------------*/
int Spario::getCollisionPoint(int aPos) const
{
	switch (aPos) {
	case 0: // x
		return (int)mX + 6;
	case 1: // y
		return (int)mY + 6 + (int)mCameraY;
	case 2: // w
		return 4;
	case 3: // h
		return 4;
	}
	return -1;
}

/*------------------------------------------------------------------------------
タグを返す
------------------------------------------------------------------------------*/
eTag Spario::getTag() const
{
	return Tag_Enemy_Bullet;
}