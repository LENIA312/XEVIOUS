#include <DxLib.h>
#include "GaruZakato.h"
#include "Image.h"
#include "Define.h"
#include "SE.h"

/*------------------------------------------------------------------------------
定義
------------------------------------------------------------------------------*/
static const float SPEED = (float)3;

/*------------------------------------------------------------------------------
コンストラクタ
------------------------------------------------------------------------------*/
GaruZakato::GaruZakato(InterfaceNPCEvent* aNPCEvent, NPCDATA aData, Player* aPlayer) : BaseNPC(aNPCEvent)
{
	mPtn = aData.ptn;
	// 出現位置はランダム
	mX = (float)(GetRand(256) + 48);
	mY = (float)aData.y - 16;
	mPlayer = aPlayer;

	mTimer = 0;
	mBakuhatuTimer = GetRand(30) + 10;
}

/*------------------------------------------------------------------------------
デストラクタ
------------------------------------------------------------------------------*/
GaruZakato::~GaruZakato()
{

}

/*------------------------------------------------------------------------------
更新
------------------------------------------------------------------------------*/
void GaruZakato::update()
{
	mY += SPEED;
	if (mTimer >= mBakuhatuTimer) {
		// スパリオ発射
		SE::getIns()->playSE(SE_GaruZakatoExplosion);
		mNPCEvent->generateEnemyBullet(2, 0, (int)mX + 8, (int)mY + 8);
		mActiveFlag = false;
	}
	mTimer++;
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
void GaruZakato::draw() const
{
	// 通常
	if (!mIsStriken) {
		DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_EnemyBullet, 35 + (int)mColor), true);
	}
	// 破壊
	else {
		DrawGraph((int)mX - 8, (int)mY - 8 - (int)mCameraY, Image::getIns()->getImage(Image_EnemyStriken, (int)mAnime), true);
	}
	//clsDx();
	//printfDx("%0.1f %0.1f", mX, mY);
}

/*------------------------------------------------------------------------------
倒されたとき時
------------------------------------------------------------------------------*/
void GaruZakato::strikenEvent()
{
	mAnime += (float)0.8;
	if (mAnime > 5) {
		mActiveFlag = false;
	}
}

/*------------------------------------------------------------------------------
ザッパーに当たった時
------------------------------------------------------------------------------*/
void GaruZakato::collisionGameObject(eTag aTag)
{
	if (aTag == Tag_Zapper && !mIsStriken) {
		// 倒されたフラグを立てる
		mIsStriken = true;
		// アニメ位置を初期化
		mAnime = 0;
		// スコアを加算する
		mNPCEvent->addScore(1000);
		// 効果音再生
		SE::getIns()->playSE(SE_EnemyStrikenZapper);
	}
}

/*------------------------------------------------------------------------------
ザッパーとの当たり判定
------------------------------------------------------------------------------*/
void GaruZakato::collision()
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
int GaruZakato::getCollisionPoint(int aPos) const
{
	switch (aPos) {
	case 0: // x
		return (int)mX + 2;
	case 1: // y
		return (int)mY + 2;
	case 2: // w
		return 12;
	case 3: // h
		return 12;
	}
	return -1;
}

/*------------------------------------------------------------------------------
エリアが変わる瞬間に実行される
------------------------------------------------------------------------------*/
void GaruZakato::areaChange()
{
	// 何もしない
}

/*------------------------------------------------------------------------------
タグを返す
------------------------------------------------------------------------------*/
eTag GaruZakato::getTag() const
{
	return Tag_Enemy_Sky;
}