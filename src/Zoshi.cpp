#include <DxLib.h>
#include "Zoshi.h"
#include "Image.h"
#include "Define.h"
#include "SE.h"

/*------------------------------------------------------------------------------
定義
------------------------------------------------------------------------------*/
static const float SPEED = (float)1.4;

/*------------------------------------------------------------------------------
コンストラクタ
------------------------------------------------------------------------------*/
Zoshi::Zoshi(InterfaceNPCEvent* aNPCEvent, NPCDATA aData, Player* aPlayer) : BaseNPC(aNPCEvent)
{
	mPtn = aData.ptn;
	// 出現位置はランダム
	mX = (float)(GetRand(256) + 48);
	mY = (float)aData.y - 16;
	mPlayer = aPlayer;

	mAngleChangeTime = 0;
	mSparioTimer = 0;
	mInitFlg = false;
	mTheta = 0;
}

/*------------------------------------------------------------------------------
デストラクタ
------------------------------------------------------------------------------*/
Zoshi::~Zoshi()
{

}

/*------------------------------------------------------------------------------
更新
------------------------------------------------------------------------------*/
void Zoshi::update()
{
	// 初期化
	if (!mInitFlg) {
		mInitFlg = true;
		// エディター用の処理 (画面外でテストプレイしたときに生成されなくなる)
		if (mY - Define::GameScreenHeight > mCameraY) {
			mActiveFlag = false;
		} // エディターの処理終了
		// パターンが2以上なら下から出現
		if (mPtn >= 2) {
			mY += Define::GameScreenHeight + 16;
		}
		mY -= mCameraY;
		if (mY > 270) {
			mY = 270;
		}
		// 角度
		mTheta = atan2(mPlayer->getXY(2) - (mY + (int)mCameraY), mPlayer->getXY(0) - mX);
	}
	// 倒されていない時
	if (!mIsStriken) {
		// 移動
		mX += cos(mTheta) * SPEED;
		mY += sin(mTheta) * SPEED;
		// パターン1以上ならスパリオ発射
		if (mPtn >= 1) {
			int r = GetRand(60);
			// 60分の1の確率で弾を撃つ
			if (mSparioTimer >= 60 && r == 0) {
				mNPCEvent->generateEnemyBullet(0, 0, (int)mX + 8, (int)mY + 8 + (int)mCameraY);
				mSparioTimer = 0;
			}
			mSparioTimer++;
		}
		// 移動角度を変える
		{
			if (mAngleChangeTime >= 90) {
				mAngleChangeTime = 0;
				int r = 0;
				if (mPtn == 0) {
					r = GetRand(5);
				}
				else if (mPtn == 1) {
					r = GetRand(3);
				}
				else if (mPtn >= 2) {
					r = GetRand(2);
				}
				if (r == 0) {
					mTheta = atan2(mPlayer->getXY(2) - (mY + (int)mCameraY), mPlayer->getXY(0) - mX);
				}
			}
			mAngleChangeTime++;
		}
		// アニメ
		if (!mIsStriken) {
			mAnime += (float)0.8;
			if (mAnime >= 4) {
				mAnime = 0;
			}
		}
		collision();
	}
	// 倒された時
	else {
		strikenEvent();
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
void Zoshi::draw() const
{
	// 通常
	if (!mIsStriken) {
		switch (mColor) {
		case 0:
			DrawGraph((int)mX, (int)mY, Image::getIns()->getImage(Image_Enemy_a, 32 + (int)mAnime), true);
			break;
		case 1:
			DrawGraph((int)mX, (int)mY, Image::getIns()->getImage(Image_Enemy_b, 32 + (int)mAnime), true);
			break;
		case 2:
			DrawGraph((int)mX, (int)mY, Image::getIns()->getImage(Image_Enemy_c, 32 + (int)mAnime), true);
			break;
		case 3:
			DrawGraph((int)mX, (int)mY, Image::getIns()->getImage(Image_Enemy_d, 32 + (int)mAnime), true);
			break;
		case 4:
			DrawGraph((int)mX, (int)mY, Image::getIns()->getImage(Image_Enemy_e, 32 + (int)mAnime), true);
			break;
		}
	}
	// 破壊
	else {
		DrawGraph((int)mX - 8, (int)mY - 8, Image::getIns()->getImage(Image_EnemyStriken, (int)mAnime), true);
	}
	//clsDx();
	//printfDx("%0.1f %0.1f", mX, mY);
}

/*------------------------------------------------------------------------------
倒されたとき時
------------------------------------------------------------------------------*/
void Zoshi::strikenEvent()
{
	mAnime += (float)0.8;
	if (mAnime > 5) {
		mActiveFlag = false;
	}
}

/*------------------------------------------------------------------------------
ザッパーに当たった時
------------------------------------------------------------------------------*/
void Zoshi::collisionGameObject(eTag aTag)
{
	if (aTag == Tag_Zapper && !mIsStriken) {
		// 倒されたフラグを立てる
		mIsStriken = true;
		// アニメ位置を初期化
		mAnime = 0;
		// スコアを加算する
		if (mPtn == 0) {
			mNPCEvent->addScore(70);
		}
		else if (mPtn == 1) {
			mNPCEvent->addScore(80);
		}
		else if (mPtn >= 2) {
			mNPCEvent->addScore(100);
		}
		// 効果音再生
		SE::getIns()->playSE(SE_EnemyStrikenZapper);
	}
}

/*------------------------------------------------------------------------------
ザッパーとの当たり判定
------------------------------------------------------------------------------*/
void Zoshi::collision()
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
int Zoshi::getCollisionPoint(int aPos) const
{
	switch (aPos) {
	case 0: // x
		return (int)mX + 1;
	case 1: // y
		return (int)mY + 1 + (int)mCameraY;
	case 2: // w
		return 14;
	case 3: // h
		return 14;
	}
	return -1;
}

/*------------------------------------------------------------------------------
エリアが変わる瞬間に実行される
------------------------------------------------------------------------------*/
void Zoshi::areaChange()
{
	// 何もしない
}

/*------------------------------------------------------------------------------
タグを返す
------------------------------------------------------------------------------*/
eTag Zoshi::getTag() const
{
	return Tag_Enemy_Sky;
}