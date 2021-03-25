#include <DxLib.h>
#include "Bozalogram.h"
#include "Image.h"
#include "Define.h"
#include "SE.h"

/*------------------------------------------------------------------------------
定数
------------------------------------------------------------------------------*/
const static int LEFT = 0; // ボザログラムの左部
const static int RIGHT = 1; // ボザログラムの右部
const static int UP = 2; // ボザログラムの上部
const static int DOWN = 3; // ボザログラムの下部

/*------------------------------------------------------------------------------
コンストラクタ
------------------------------------------------------------------------------*/
Bozalogram::Bozalogram(InterfaceNPCEvent* aNPCEvent, NPCDATA aData, Player* aPlayer) : BaseNPC(aNPCEvent)
{
	mPtn = aData.ptn;
	mX = (float)aData.x;
	mY = (float)aData.y - 37;
	mPlayer = aPlayer;

	for (int i = 0; i < 4; i++) {
		mAtkFlg[i] = 0;
		lAtkMode[i] = 0;
		mPartAnime[i] = 0;
		mSparioTimer[i] = 0;
		mBrokenAnime[i] = 0;
		mBrokenFlg[i] = false;
	}

}

/*------------------------------------------------------------------------------
デストラクタ
------------------------------------------------------------------------------*/
Bozalogram::~Bozalogram()
{

}

/*------------------------------------------------------------------------------
更新
------------------------------------------------------------------------------*/
void Bozalogram::update()
{
	// スパリオ発射
	if(mY <= mCameraY + Define::GameScreenHeight / 2){
		for (int i = 0; i < 4; i++) {
			if (!mBrokenFlg[i] && mSparioTimer[0] > 30 && GetRand(100) == 0 && !mAtkFlg[i]) {
				mSparioTimer[i] = 0;
				mAtkFlg[i] = true;
			}
		}
	}
	// 左右上下
	for (int i = 0; i < 4; i++) {
		// 通常
		if (!mBrokenFlg[i]) {
			if (mAtkFlg[i] == true && lAtkMode[i] == 0) {
				lAtkMode[i] = 1;
			}

			if (lAtkMode[i] == 1) {
				mPartAnime[i] += (float)0.3;
			}

			if (lAtkMode[i] == 2) {
				mPartAnime[i] -= (float)0.3;
				if (mPartAnime[i] <= 0) {
					mPartAnime[i] = 0;
					lAtkMode[i] = 0;
					mAtkFlg[i] = false;
				}
			}

			if (mPartAnime[i] > 4) {
				mPartAnime[i] = 3.5;
				switch (i) {
				case 0:
					mNPCEvent->generateEnemyBullet(0, 0, (int)mX - 4, (int)mY + 8);
					break;
				case 1:
					mNPCEvent->generateEnemyBullet(0, 0, (int)mX + 20, (int)mY + 8);
					break;
				case 2:
					mNPCEvent->generateEnemyBullet(0, 0, (int)mX + 8, (int)mY - 4);
					break;
				case 3:
					mNPCEvent->generateEnemyBullet(0, 0, (int)mX + 8, (int)mY + 20);
					break;
				}
				lAtkMode[i] = 2;
			}
			// タイマー加算
			mSparioTimer[i]++;
		}
		// 破壊
		else {
			mBrokenAnime[i] += (float)0.2;
			if (mBrokenAnime[i] >= 8) {
				mBrokenAnime[i] = 6;
			}
		}
	}
	// 倒されていない時
	if (!mIsStriken) {
		collision();
	}
	// 倒された
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
void Bozalogram::draw() const
{
	// 左
	{
		// 通常
		if (!mBrokenFlg[LEFT]) {
			switch (mColor) {
			case 0:
				DrawGraph((int)mX - 12, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_a, 36 + (int)mPartAnime[0]), true);
				break;
			case 1:
				DrawGraph((int)mX - 12, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_b, 36 + (int)mPartAnime[0]), true);
				break;
			case 2:
				DrawGraph((int)mX - 12, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_c, 36 + (int)mPartAnime[0]), true);
				break;
			case 3:
				DrawGraph((int)mX - 12, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_d, 36 + (int)mPartAnime[0]), true);
				break;
			case 4:
				DrawGraph((int)mX - 12, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_e, 36 + (int)mPartAnime[0]), true);
				break;
			}
		}
		// 破壊
		else {
			if (mBrokenAnime[LEFT] < 6) {
				DrawGraph((int)mX - 8 - 12, ((int)mY - 8) - (int)mCameraY, Image::getIns()->getImage(Image_EnemyStriken, 6 + (int)mBrokenAnime[0]), true);
			}
			if (mBrokenAnime[LEFT] >= 6) {
				DrawGraph((int)mX - 12, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Destructed, (int)mBrokenAnime[0] - 6), true);
			}
		}
	}
	// 右
	{
		// 通常
		if (!mBrokenFlg[RIGHT]) {
			switch (mColor) {
			case 0:
				DrawGraph((int)mX + 12, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_a, 36 + (int)mPartAnime[1]), true);
				break;
			case 1:
				DrawGraph((int)mX + 12, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_b, 36 + (int)mPartAnime[1]), true);
				break;
			case 2:
				DrawGraph((int)mX + 12, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_c, 36 + (int)mPartAnime[1]), true);
				break;
			case 3:
				DrawGraph((int)mX + 12, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_d, 36 + (int)mPartAnime[1]), true);
				break;
			case 4:
				DrawGraph((int)mX + 12, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_e, 36 + (int)mPartAnime[1]), true);
				break;
			}
		}
		// 破壊
		else {
			if (mBrokenAnime[RIGHT] < 6) {
				DrawGraph((int)mX - 8 + 12, ((int)mY - 8) - (int)mCameraY, Image::getIns()->getImage(Image_EnemyStriken, 6 + (int)mBrokenAnime[1]), true);
			}
			if (mBrokenAnime[RIGHT] >= 6) {
				DrawGraph((int)mX + 12, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Destructed, (int)mBrokenAnime[1] - 6), true);
			}
		}
	}
	// 上
	{
		// 通常
		if (!mBrokenFlg[UP]) {
			switch (mColor) {
			case 0:
				DrawGraph((int)mX, (int)mY - 12 - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_a, 36 + (int)mPartAnime[2]), true);
				break;
			case 1:
				DrawGraph((int)mX, (int)mY - 12 - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_b, 36 + (int)mPartAnime[2]), true);
				break;
			case 2:
				DrawGraph((int)mX, (int)mY - 12 - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_c, 36 + (int)mPartAnime[2]), true);
				break;
			case 3:
				DrawGraph((int)mX, (int)mY - 12 - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_d, 36 + (int)mPartAnime[2]), true);
				break;
			case 4:
				DrawGraph((int)mX, (int)mY - 12 - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_e, 36 + (int)mPartAnime[2]), true);
				break;
			}
		}
		// 破壊
		else {
			if (mBrokenAnime[UP] < 6) {
				DrawGraph((int)mX - 8 , ((int)mY - 8) - 12 - (int)mCameraY, Image::getIns()->getImage(Image_EnemyStriken, 6 + (int)mBrokenAnime[2]), true);
			}
			if (mBrokenAnime[UP] >= 6) {
				DrawGraph((int)mX, (int)mY - 12 - (int)mCameraY, Image::getIns()->getImage(Image_Destructed, (int)mBrokenAnime[2] - 6), true);
			}
		}
	}
	// 下
	{
		// 通常
		if (!mBrokenFlg[DOWN]) {
			switch (mColor) {
			case 0:
				DrawGraph((int)mX, (int)mY + 12 - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_a, 36 + (int)mPartAnime[3]), true);
				break;
			case 1:
				DrawGraph((int)mX, (int)mY + 12 - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_b, 36 + (int)mPartAnime[3]), true);
				break;
			case 2:
				DrawGraph((int)mX, (int)mY + 12 - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_c, 36 + (int)mPartAnime[3]), true);
				break;
			case 3:
				DrawGraph((int)mX, (int)mY + 12 - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_d, 36 + (int)mPartAnime[3]), true);
				break;
			case 4:
				DrawGraph((int)mX, (int)mY + 12 - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_e, 36 + (int)mPartAnime[3]), true);
				break;
			}
		}
		// 破壊
		else {
			if (mBrokenAnime[DOWN] < 6) {
				DrawGraph((int)mX - 8, ((int)mY - 8) + 12 - (int)mCameraY, Image::getIns()->getImage(Image_EnemyStriken, 6 + (int)mBrokenAnime[3]), true);
			}
			if (mBrokenAnime[DOWN] >= 6) {
				DrawGraph((int)mX, (int)mY + 12 - (int)mCameraY, Image::getIns()->getImage(Image_Destructed, (int)mBrokenAnime[3] - 6), true);
			}
		}
	}
	// 真ん中
	{
		// 通常
		if (!mIsStriken) {
			switch (mColor) {
			case 0:
				DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_a, 62 + (int)mAnime), true);
				break;
			case 1:
				DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_b, 62 + (int)mAnime), true);
				break;
			case 2:
				DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_c, 62 + (int)mAnime), true);
				break;
			case 3:
				DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_d, 62 + (int)mAnime), true);
				break;
			case 4:
				DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_e, 62 + (int)mAnime), true);
				break;
			}
		}
		// 破壊
		else {
			if (mAnime < 6) {
				DrawGraph((int)mX - 8, ((int)mY - 8) - (int)mCameraY, Image::getIns()->getImage(Image_EnemyStriken, 6 + (int)mAnime), true);
			}
			if (mAnime >= 6) {
				DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Destructed, (int)mAnime - 6), true);
			}
		}
	}
	// デバッグ
}

/*------------------------------------------------------------------------------
当たり判定描画
------------------------------------------------------------------------------*/
void Bozalogram::drawCollision(int r, int g, int b) const
{
	int a = 12;
	DrawBox((int)mX, (int)mY - (int)mCameraY, (int)mX + 16, (int)mY + 16 - (int)mCameraY, GetColor(r, g, b), FALSE);
	DrawBox((int)mX - a, (int)mY - (int)mCameraY, (int)mX - a + 16, (int)mY + 16 - (int)mCameraY, GetColor(r, g, b), FALSE);
	DrawBox((int)mX + a, (int)mY - (int)mCameraY, (int)mX + a + 16, (int)mY + 16 - (int)mCameraY, GetColor(r, g, b), FALSE);
	DrawBox((int)mX, (int)mY - a - (int)mCameraY, (int)mX + 16, (int)mY - a + 16 - (int)mCameraY, GetColor(r, g, b), FALSE);
	DrawBox((int)mX, (int)mY + a - (int)mCameraY, (int)mX + 16, (int)mY + a + 16 - (int)mCameraY, GetColor(r, g, b), FALSE);
}

/*------------------------------------------------------------------------------
倒されたとき時
------------------------------------------------------------------------------*/
void Bozalogram::strikenEvent()
{
	mAnime += (float)0.2;
	if (mAnime >= 8) {
		mAnime = 6;
	}
}

/*------------------------------------------------------------------------------
ブラスターに当たった時
------------------------------------------------------------------------------*/
void Bozalogram::collisionGameObject(eTag aTag)
{
	if (aTag == Tag_Blaster && !mIsStriken) {
		// ブラスター座標
		Blaster* blaster = mPlayer->getBlaster();
		int bx = blaster->getCollisionPoint(0);
		int by = blaster->getCollisionPoint(1);
		int bw = blaster->getCollisionPoint(2);
		int bh = blaster->getCollisionPoint(3);
		// 当たり判定 幅 / 高さ
		int w = 16;
		int h = 16;
		int x, y;
		// 真ん中に当たったら
		{
			x = (int)mX;
			y = (int)mY;
			if (bx + bw >= x && bx <= x + w &&
				by + bh >= y && by <= y + h) {
				// パーツの倒されたフラグを立てる
				mIsStriken = true;
				for (int i = 0; i < 4; i++) {
					mBrokenFlg[i] = true;
				}
				// スコアを加算する
				mNPCEvent->addScore(10);
				// 効果音再生
				SE::getIns()->playSE(SE_EnemyStrikenBlaster);
			}
		}
		// 左に当たったら
		{
			x = (int)mX - 12;
			y = (int)mY;
			if (!mBrokenFlg[0] &&
				bx + bw >= x && bx <= x + w &&
				by + bh >= y && by <= y + h) {
				mBrokenFlg[0] = true;
				// スコアを加算する
				mNPCEvent->addScore(1);
				// 効果音再生
				SE::getIns()->playSE(SE_EnemyStrikenBlaster);
			}
		}
		// 右に当たったら
		{
			x = (int)mX + 12;
			y = (int)mY;
			if (!mBrokenFlg[1] &&
				bx + bw >= x && bx <= x + w &&
				by + bh >= y && by <= y + h) {
				mBrokenFlg[1] = true;
				// スコアを加算する
				mNPCEvent->addScore(1);
				// 効果音再生
				SE::getIns()->playSE(SE_EnemyStrikenBlaster);
			}
		}
		// 上に当たったら
		{
			x = (int)mX;
			y = (int)mY - 12;
			if (!mBrokenFlg[2] &&
				bx + bw >= x && bx <= x + w &&
				by + bh >= y && by <= y + h) {
				mBrokenFlg[2] = true;
				// スコアを加算する
				mNPCEvent->addScore(1);
				// 効果音再生
				SE::getIns()->playSE(SE_EnemyStrikenBlaster);
			}
		}
		// 下に当たったら
		{
			x = (int)mX;
			y = (int)mY + 12;
			if (!mBrokenFlg[3] &&
				bx + bw >= x && bx <= x + w &&
				by + bh >= y && by <= y + h) {
				mBrokenFlg[3] = true;
				// スコアを加算する
				mNPCEvent->addScore(1);
				// 効果音再生
				SE::getIns()->playSE(SE_EnemyStrikenBlaster);
			}
		}
	}
}

/*------------------------------------------------------------------------------
ブラスターとの当たり判定
------------------------------------------------------------------------------*/
void Bozalogram::collision()
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
int Bozalogram::getCollisionPoint(int aPos) const
{
	switch (aPos) {
	case 0: // x
		return (int)mX - 10 + 2;
	case 1: // y
		return (int)mY - 10 + 2;
	case 2: // w
		return 37 - 3;
	case 3: // h
		return 37 - 3;
	}
	return -1;
}

/*------------------------------------------------------------------------------
タグを返す
------------------------------------------------------------------------------*/
eTag Bozalogram::getTag() const
{
	return Tag_Enemy_Ground;
}