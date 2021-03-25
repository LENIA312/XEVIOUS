#include <DxLib.h>
#include "BragSpario.h"
#include "Image.h"
#include "Define.h"
#include "SE.h"

/*------------------------------------------------------------------------------
コンストラクタ
------------------------------------------------------------------------------*/
BragSpario::BragSpario(InterfaceNPCEvent* aNPCEvent, NPCDATA aData, Player* aPlayer) : BaseNPC(aNPCEvent)
{
	mPtn = aData.ptn;
	mX = (float)aData.x;
	mY = (float)aData.y - 8;
	mPlayer = aPlayer;

	mTimer = 0;
	mSpeed = 0;
	mInitFlg = false;

	// プレイヤーとの角度
	float pai = (float)3.14;
	mTheta = 0;
	for (int i = 0; i < 4; i++) {
		if (mPtn == i) {
			mTheta = (i * 90) * (pai / 180);
			break;
		}
	}
}

/*------------------------------------------------------------------------------
デストラクタ
------------------------------------------------------------------------------*/
BragSpario::~BragSpario()
{

}

/*------------------------------------------------------------------------------
更新
------------------------------------------------------------------------------*/
void BragSpario::update()
{
	// 初期化
	if (!mInitFlg) {
		mInitFlg = true;
		mY -= mCameraY;
	}
	// 動き
	mX += cos(mTheta) * mSpeed;
	mY += sin(mTheta) * mSpeed;
	mSpeed += (float)0.15;
	// 角度変更
	if (mTimer >= 7) {
		mTimer = 0;
		float bx = cos(mTheta);
		float by = sin(mTheta);
		float ax = mPlayer->getXY(0) - mX;
		float ay = mPlayer->getXY(1) - mY;
		mTheta += (ax * by - ay * bx < 0.0) ? +(float)3.14 / 180 * 8 : -(float)3.14 / 180 * 8;
	}
	mTimer++;
	// アニメ
	if (mAnime >= 4) {
		mAnime = 0;
	}
	mAnime += (float)0.3;
	// 当たり判定
	collision();
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
void BragSpario::draw() const
{
	DrawRotaGraph((int)mX + 8, (int)mY + 8, 1, (int)mAnime * 90, Image::getIns()->getImage(Image_EnemyBullet, 28), true);
	// デバッグ
	//DrawFormatString(0, 0, GetColor(255, 255, 255), "theta:%0.1f px:%d", mTheta, mPlayerX);
}

/*------------------------------------------------------------------------------
エリアの変わり目に当たり判定を移動する
------------------------------------------------------------------------------*/
void BragSpario::areaChange()
{
	// 何もしない
}

/*------------------------------------------------------------------------------
ザッパーに当たった時
------------------------------------------------------------------------------*/
void BragSpario::collisionGameObject(eTag aTag)
{
	if (aTag == Tag_Zapper && !mIsStriken) {
		// スコアを加算する
		mNPCEvent->addScore(500);
		// 効果音再生
		SE::getIns()->playSE(SE_EnemyStrikenZapper);
	}
}

/*------------------------------------------------------------------------------
ザッパーとの当たり判定
------------------------------------------------------------------------------*/
void BragSpario::collision()
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
int BragSpario::getCollisionPoint(int aPos) const
{
	switch (aPos) {
	case 0: // x
		return (int)mX + 5;
	case 1: // y
		return (int)mY + 5 + (int)mCameraY;
	case 2: // w
		return 6;
	case 3: // h
		return 6;
	}
	return -1;
}

/*------------------------------------------------------------------------------
タグを返す
------------------------------------------------------------------------------*/
eTag BragSpario::getTag() const
{
	return Tag_Enemy_Bullet;
}