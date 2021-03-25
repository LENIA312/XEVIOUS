#include <DxLib.h>
#include "Bacura.h"
#include "Image.h"
#include "Define.h"
#include "SE.h"

/*------------------------------------------------------------------------------
定義
------------------------------------------------------------------------------*/
static const int MAX = 3; // エフェクト最大同時再生数

/*------------------------------------------------------------------------------
コンストラクタ
------------------------------------------------------------------------------*/
Bacura::Bacura(InterfaceNPCEvent* aNPCEvent, NPCDATA aData, Player* aPlayer) : BaseNPC(aNPCEvent)
{
	mPtn = aData.ptn;
	mX = (float)(GetRand(256) + 32);
	mY = (float)aData.y - 32;
	mPlayer = aPlayer;

	// ザッパー粉々エフェクト
	mEfectNum = 0;
	mEfectX = new int[MAX];
	mEfectShift = new int[MAX];
	mEfectFlg = new bool[MAX];
	mEfectAnime = new float[MAX];
	for (int i = 0; i < MAX; i++) {
		mEfectX[i] = 0;
		mEfectShift[i] = 0;
		mEfectFlg[i] = false;
		mEfectAnime[i] = 0;
	}
}

/*------------------------------------------------------------------------------
デストラクタ
------------------------------------------------------------------------------*/
Bacura::~Bacura()
{
	delete[] mEfectX;
	delete[] mEfectShift;
	delete[] mEfectFlg;
	delete[] mEfectAnime;
}

/*------------------------------------------------------------------------------
更新
------------------------------------------------------------------------------*/
void Bacura::update()
{
	mY += 0.5;
	mAnime += (float)0.2;
	if (mAnime >= 8) {
		mAnime = 0;
	}
	// 画面外に出たらフラグを折る
	if (mY > mCameraY + Define::GameScreenHeight) {
		mActiveFlag = false;
	}
	// ザッパー粉々エフェクト
	for (int i = 0; i < MAX; i++) {
		if (mEfectFlg[i]) {
			mEfectAnime[i] += (float)0.3;
			// 色変え
			if (GetRand(1) == 0) {
				mEfectShift[i] = 0;
			}
			else {
				mEfectShift[i] = 4;
			}
			if (mEfectAnime[i] >= 4) {
				mEfectFlg[i] = false;
			}
		}
	}
	// 当たり判定
	collision();
	
}

/*------------------------------------------------------------------------------
 描画
------------------------------------------------------------------------------*/
void Bacura::draw() const
{
	// 通常
	if (!mIsStriken) {
		switch (mColor) {
		case 0:
			DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_EnemyBig_a, 16 + (int)mAnime), true);
			break;
		case 1:
			DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_EnemyBig_b, 16 + (int)mAnime), true);
			break;
		case 2:
			DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_EnemyBig_c, 16 + (int)mAnime), true);
			break;
		case 3:
			DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_EnemyBig_d, 16 + (int)mAnime), true);
			break;
		case 4:
			DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_EnemyBig_d, 16 + (int)mAnime), true);
			break;
		}
	}
	// ザッパー粉々エフェクト
	//clsDx();
	for (int i = 0; i < MAX; i++) {
		if (mEfectFlg[i]) {
			DrawGraph((int)mEfectX[i], (int)mY + 10 - (int)mCameraY,
				Image::getIns()->getImage(Image_Zapper, 9 + mEfectShift[i] + (int)mEfectAnime[i]), true);
		}
		//printfDx("%d:%d", i, mEfectFlg[i]);
		//printfDx("\n");
	}
}

/*------------------------------------------------------------------------------
ザッパーに当たった時
------------------------------------------------------------------------------*/
void Bacura::collisionGameObject(eTag aTag)
{
	if (aTag == Tag_Zapper) {
		// 効果音再生
		SE::getIns()->playSE(SE_BacuraHitZapper);
		// ザッパー粉々エフェクト
		for (int i = 0; i < MAX; i++) {
			if (!mEfectFlg[i]) {
				mEfectFlg[i] = true;
				Zapper* zapper = mPlayer->getZapper();
				mEfectX[i] = zapper[mEfectNum].getCollisionPoint(0);
				mEfectAnime[i] = 0;
				break;
			}
		}
	}
}

/*------------------------------------------------------------------------------
ザッパーとの当たり判定
------------------------------------------------------------------------------*/
void Bacura::collision()
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
				mEfectNum = i;
				collisionGameObject(zapper[i].getTag());
				zapper[i].collisionGameObject(getTag());
			}
		}
	}
}

/*------------------------------------------------------------------------------
当たり判定座標を返す
------------------------------------------------------------------------------*/
int Bacura::getCollisionPoint(int aPos) const
{
	switch (aPos) {
	case 0: // x
		return (int)mX + 8;
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
eTag Bacura::getTag() const
{
	return Tag_Enemy_Sky;
}
