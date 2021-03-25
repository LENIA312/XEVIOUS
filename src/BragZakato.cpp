#include <DxLib.h>
#include "BragZakato.h"
#include "Image.h"
#include "Define.h"
#include "SE.h"

/*------------------------------------------------------------------------------
コンストラクタ
------------------------------------------------------------------------------*/
BragZakato::BragZakato(InterfaceNPCEvent* aNPCEvent, NPCDATA aData, Player* aPlayer) : BaseNPC(aNPCEvent)
{
	mPtn = aData.ptn;
	mPlayer = aPlayer;
	// 出現位置はランダム
	mX = (float)(GetRand(256) + 48);
	mY = (float)aData.y;
	mY += GetRand(80);
	// タイマー
	mTimer = 0;
	mBakuhatuTimer = GetRand(60) + 10;
	// 角度
	mTheta = atan2(mPlayer->getXY(2) - mY, mPlayer->getXY(0) - mX);
	// 爆発フラグ
	mBakuhatuFlg = false;
	// 効果音
	SE::getIns()->playSE(SE_ZakatoDethout);
}

/*------------------------------------------------------------------------------
デストラクタ
------------------------------------------------------------------------------*/
BragZakato::~BragZakato()
{

}

/*------------------------------------------------------------------------------
更新
------------------------------------------------------------------------------*/
void BragZakato::update()
{
	// 移動
	{
		// 爆発していないとき
		if (!mBakuhatuFlg) {
			if (mAnime <= 5) {
				mAnime += (float)0.3;
			}
			else {
				mX += cos(mTheta) * (float)1.3;
				mY += sin(mTheta) * (float)1.3;
				if (mTimer >= mBakuhatuTimer) {
					// スパリオ発射
					mNPCEvent->generateEnemyBullet(1, 0, (int)mX + 8, (int)mY + 8);
					mBakuhatuFlg = true;
					mAnime = 5;
				}
				mTimer++;
			}
		}
		// 爆発したとき
		else {
			if (mAnime <= 10) {
				mAnime += (float)0.3;
			}
			else {
				mActiveFlag = false;
			}
		}
	}
	// 倒されていない時
	if (!mIsStriken) {
		if (!mBakuhatuFlg) {
			collision();
		}
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
void BragZakato::draw() const
{
	// 通常
	if (!mIsStriken) {
		// 爆発していないとき
		if (!mBakuhatuFlg) {
			if (mAnime <= 5) {
				DrawGraph((int)mX - 8, (int)mY - 8 - (int)mCameraY, Image::getIns()->getImage(Image_ZakatoEfect, (int)mAnime), true);
			}
			else {
				DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_EnemyBullet, 30 + mColor), true);
			}
		}
		// 爆発したとき
		else {
			if (mAnime <= 10) {
				DrawGraph((int)mX - 8, (int)mY - 8 - (int)mCameraY, Image::getIns()->getImage(Image_ZakatoEfect, (int)mAnime), true);
			}
		}
	}
	// 破壊
	else {
		DrawGraph((int)mX - 8, ((int)mY - 8) - (int)mCameraY, Image::getIns()->getImage(Image_EnemyStriken, (int)mAnime), true);
	}
	// デバッグ
	//clsDx();
	//printfDx("%0.1f", mAnime);
	//DrawLine(mPlayerX, 0, mPlayerX, 300, GetColor(0, 0, 0));
}

/*------------------------------------------------------------------------------
倒されたとき時
------------------------------------------------------------------------------*/
void BragZakato::strikenEvent()
{
	mAnime += (float)0.8;
	if (mAnime > 5) {
		mActiveFlag = false;
	}
}

/*------------------------------------------------------------------------------
ザッパーに当たった時
------------------------------------------------------------------------------*/
void BragZakato::collisionGameObject(eTag aTag)
{
	// 当たったオブジェクトがザッパーなら
	if (aTag == Tag_Zapper && !mIsStriken) {
		// 倒されたフラグを立てる
		mIsStriken = true;
		// アニメ位置を初期化
		mAnime = 0;
		// スコアを加算する
		mNPCEvent->addScore((GetRand(9) + 6) * 100);
		// 効果音再生
		SE::getIns()->playSE(SE_EnemyStrikenZapper);
	}
}

/*------------------------------------------------------------------------------
ザッパーとの当たり判定
------------------------------------------------------------------------------*/
void BragZakato::collision()
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
int BragZakato::getCollisionPoint(int aPos) const
{
	switch (aPos) {
	case 0: // x
		return (int)mX + 4;
	case 1: // y
		return (int)mY + 4;
	case 2: // w
		return 8;
	case 3: // h
		return 8;
	}
	return -1;
}

/*------------------------------------------------------------------------------
タグを返す
------------------------------------------------------------------------------*/
eTag BragZakato::getTag() const
{
	return Tag_Enemy_Sky;
}
