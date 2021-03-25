#include <DxLib.h>
#include "AddorGuileness.h"
#include "Image.h"
#include "Define.h"
#include "SE.h"
#include "BGM.h"

/*------------------------------------------------------------------------------
コンストラクタ
------------------------------------------------------------------------------*/
AddorGuileness::AddorGuileness(InterfaceNPCEvent* aNPCEvent, NPCDATA aData, Player* aPlayer) : BaseNPC(aNPCEvent)
{
	mPtn = aData.ptn;
	mX = (float)aData.x;
	mY = (float)aData.y - 96;
	mPlayer = aPlayer;

	mSayonaraTimer = 0;
	mSparioTimer[0] = 0;
	mSparioTimer[1] = 0;
	mSparioTimer[2] = 0;
	mSparioTimer[3] = 0;
	mEscapeY = 0;
	mEscapeAnime = 0;
	mBrokenAnime[0] = 0;   // 左上
	mBrokenAnime[1] = 0;   // 右上
	mBrokenAnime[2] = 0;   // 左下
	mBrokenAnime[3] = 0;   // 右下
	mBrokenFlg[0] = false; // 左上
	mBrokenFlg[1] = false; // 右上
	mBrokenFlg[2] = false; // 左下
	mBrokenFlg[3] = false; // 右下
}

/*------------------------------------------------------------------------------
デストラクタ
------------------------------------------------------------------------------*/
AddorGuileness::~AddorGuileness()
{

}

/*------------------------------------------------------------------------------
更新
------------------------------------------------------------------------------*/
void AddorGuileness::update()
{
	// 倒されていない時
	if (!mIsStriken) {
		// 初期化
		if (mSayonaraTimer == 0) {
			// エディター用の処理 (画面外でテストプレイしたときに生成されなくなる)
			if (mY > mCameraY) {
				// BGM停止
				BGM::getIns()->stopBGM(BGM_Boss);
				mActiveFlag = false;
			} // エディターの処理終了
			mY -= mCameraY;
		}
		// BGM
		if (!BGM::getIns()->checkIsPlay(BGM_Boss) &&
			!mIsStriken && !BGM::getIns()->checkIsPlay(BGM_Start) &&
			mPlayer->checkActiveFlag() && mActiveFlag) {
			BGM::getIns()->playBGM(BGM_Boss, true, false);
		}
		// 移動イベント
		{
			// 規定位置まで下がる
			if (mY <= 48 && mSayonaraTimer < 660) {
				mY++;
			}
			if (mY > 48) {
				mY = 48;
			}
			// 11秒したら帰る
			if (mSayonaraTimer >= 660) {
				mY--;
			}
			mSayonaraTimer++;
		}
		// パーツ破壊
		for (int i = 0; i < 4; i++) {
			if (mBrokenFlg[i] && mBrokenAnime[i] < 6) {
				mBrokenAnime[i] += (float)0.2;
			}
		}
		// スパリオ発射
		{
			if (!mBrokenFlg[0] && mSparioTimer[0] > 30 && GetRand(100) == 0) {
				mSparioTimer[0] = 0;
				mNPCEvent->generateEnemyBullet(0, 0, (int)mX + 32, (int)mY + 32 + (int)mCameraY);
			}
			if (!mBrokenFlg[1] && mSparioTimer[1] > 30 && GetRand(100) == 0) {
				mSparioTimer[1] = 0;
				mNPCEvent->generateEnemyBullet(0, 0, (int)mX + 64, (int)mY + 32 + (int)mCameraY);
			}
			if (!mBrokenFlg[2] && mSparioTimer[2] > 30 && GetRand(100) == 0) {
				mSparioTimer[2] = 0;
				mNPCEvent->generateEnemyBullet(0, 0, (int)mX + 32, (int)mY + 64 + (int)mCameraY);
			}
			if (!mBrokenFlg[3] && mSparioTimer[3] > 30 && GetRand(100) == 0) {
				mSparioTimer[3] = 0;
				mNPCEvent->generateEnemyBullet(0, 0, (int)mX + 64, (int)mY + 64 + (int)mCameraY);
			}
			for (int i = 0; i < 4; i++) {
				mSparioTimer[i]++;
			}
		}
		// 当たり判定
		collision();
	}
	// 倒されたら
	else {
		strikenEvent();
	}
	// 画面外に出たらフラグを折る
	if (mY + 96 < 0 && !mIsStriken ||
		mY > mCameraY + Define::GameScreenHeight && mIsStriken) {
		mActiveFlag = false;
		// BGM停止
		BGM::getIns()->stopBGM(BGM_Boss);
	}
}

/*------------------------------------------------------------------------------
 描画
------------------------------------------------------------------------------*/
void AddorGuileness::draw() const
{
	// 通常
	if (!mIsStriken) {
		// 本体
		DrawGraph((int)mX, (int)mY, Image::getIns()->getImage(Image_Boss, mColor), true);
		DrawGraph((int)mX + 40, (int)mY + 40, Image::getIns()->getImage(Image_BossParts, mColor), true);
		// 左上
		{
			if (!mBrokenFlg[0]) {
				DrawGraph((int)mX + 24, (int)mY + 24, Image::getIns()->getImage(Image_BossParts, 5 + mColor), true);
			}
			// 爆発
			else {
				if (mBrokenAnime[0] < 6) {
					DrawGraph((int)mX + 16, (int)mY + 16, Image::getIns()->getImage(Image_EnemyStriken, 6 + (int)mBrokenAnime[0]), true);
				}
			}
		}
		// 右上
		{
			if (!mBrokenFlg[1]) {
				DrawGraph((int)mX + 56, (int)mY + 24, Image::getIns()->getImage(Image_BossParts, 10 + mColor), true);
			}
			// 爆発
			else {
				if (mBrokenAnime[1] < 6) {
					DrawGraph((int)mX + 48, (int)mY + 16, Image::getIns()->getImage(Image_EnemyStriken, 6 + (int)mBrokenAnime[1]), true);
				}
			}
		}
		// 左下
		{
			if (!mBrokenFlg[2]) {
				DrawGraph((int)mX + 24, (int)mY + 56, Image::getIns()->getImage(Image_BossParts, 15 + mColor), true);
			}
			// 爆発
			else {
				if (mBrokenAnime[2] < 6) {
					DrawGraph((int)mX + 16, (int)mY + 48, Image::getIns()->getImage(Image_EnemyStriken, 6 + (int)mBrokenAnime[2]), true);
				}
			}
		}
		// 右下
		{
			if (!mBrokenFlg[3]) {
				DrawGraph((int)mX + 56, (int)mY + 56, Image::getIns()->getImage(Image_BossParts, 20 + mColor), true);
			}
			// 爆発
			else {
				if (mBrokenAnime[3] < 6) {
					DrawGraph((int)mX + 48, (int)mY + 48, Image::getIns()->getImage(Image_EnemyStriken, 6 + (int)mBrokenAnime[3]), true);
				}
			}
		}
	}
	// 倒されたとき
	else {
		// 本体
		if (mAnime < 1) {
			DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Boss, 5), true);
		}
		else {
			DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Boss, 4), true);
		}
		// 爆発エフェクト
		{
			if (mAnime < 6) {
				DrawGraph((int)mX + 32, (int)mY + 32 - (int)mCameraY, Image::getIns()->getImage(Image_EnemyStriken, 6 + (int)mAnime), true);
			}
			// 脱出するあの玉
			else {
				DrawGraph((int)mX + 40, (int)mEscapeY + 40 - (int)mCameraY, Image::getIns()->getImage(Image_EnemyBullet, 20 + (int)mEscapeAnime), true);
			}
			if (mBrokenFlg[0] && mBrokenAnime[0] < 6) {
				DrawGraph((int)mX + 16, (int)mY + 16 - (int)mCameraY, Image::getIns()->getImage(Image_EnemyStriken, 6 + (int)mBrokenAnime[0]), true);
			}
			if (mBrokenFlg[1] && mBrokenAnime[1] < 6) {
				DrawGraph((int)mX + 48, (int)mY + 16 - (int)mCameraY, Image::getIns()->getImage(Image_EnemyStriken, 6 + (int)mBrokenAnime[1]), true);
			}
			if (mBrokenFlg[2] && mBrokenAnime[2] < 6) {
				DrawGraph((int)mX + 16, (int)mY + 48 - (int)mCameraY, Image::getIns()->getImage(Image_EnemyStriken, 6 + (int)mBrokenAnime[2]), true);
			}
			if (mBrokenFlg[3] && mBrokenAnime[3] < 6) {
				DrawGraph((int)mX + 48, (int)mY + 48 - (int)mCameraY, Image::getIns()->getImage(Image_EnemyStriken, 6 + (int)mBrokenAnime[3]), true);
			}
		}
	}
	// デバッグ
	//DrawFormatString(0, 0, GetColor(255, 255, 255), "\ny:%0.1f s:%d col:%d", mY, mIsStriken, getCollisionPoint(1));
}

/*------------------------------------------------------------------------------
当たり判定描画
------------------------------------------------------------------------------*/
void AddorGuileness::drawCollision(int r, int g, int b) const
{
	if (!mIsStriken) {
		DrawBox((int)mX + 26, (int)mY + 26, (int)mX + 24 + 14, (int)mY + 24 + 14, GetColor(r, g, b), FALSE);
		DrawBox((int)mX + 58, (int)mY + 26, (int)mX + 56 + 14, (int)mY + 24 + 14, GetColor(r, g, b), FALSE);
		DrawBox((int)mX + 26, (int)mY + 58, (int)mX + 24 + 14, (int)mY + 56 + 14, GetColor(r, g, b), FALSE);
		DrawBox((int)mX + 58, (int)mY + 58, (int)mX + 56 + 14, (int)mY + 56 + 14, GetColor(r, g, b), FALSE);
		DrawBox((int)mX + 40, (int)mY + 40, (int)mX + 40 + 16, (int)mY + 40 + 16, GetColor(r, g, b), FALSE);
	}
}

/*------------------------------------------------------------------------------
エリアの変わり目に当たり判定を移動する
------------------------------------------------------------------------------*/
void AddorGuileness::areaChange()
{
	// 倒された時
	if (mIsStriken) {
		mY += Define::MapHeight;
	}
}

/*------------------------------------------------------------------------------
倒されたとき時
------------------------------------------------------------------------------*/
void AddorGuileness::strikenEvent()
{	
	// 爆発アニメ
	if (mAnime < 6) {
		mAnime += (float)0.2;
	}
	// 脱出ボール
	else {
		mEscapeY -= 5;
		mEscapeAnime += 0.5;
		if (mEscapeAnime >= 8) {
			mEscapeAnime = 0;
		}
	}
	// パーツ爆発アニメ
	for (int i = 0; i < 4; i++) {
		if (mBrokenFlg[i] && mBrokenAnime[i] < 6) {
			mBrokenAnime[i] += (float)0.2;
		}
	}
}

/*------------------------------------------------------------------------------
ブラスターに当たった時
------------------------------------------------------------------------------*/
void AddorGuileness::collisionGameObject(eTag aTag)
{
	if (aTag == Tag_Blaster && !mIsStriken) {
		// ブラスター座標
		Blaster* blaster = mPlayer->getBlaster();
		int bx = blaster->getCollisionPoint(0);
		int by = blaster->getCollisionPoint(1);
		int bw = blaster->getCollisionPoint(2);
		int bh = blaster->getCollisionPoint(3);
		// 当たり判定 幅 / 高さ
		int w = 14;
		int h = 14;
		int x, y;
		// 真ん中に当たったら
		{
			x = (int)mX + 40;
			y = (int)mY + 40 + (int)mCameraY;
			if (bx + bw >= x && bx <= x + w &&
				by + bh >= y && by <= y + h) {
				// y座標加算
				mY += mCameraY;
				mEscapeY = mY;
				// 倒されたフラグを立てる
				mIsStriken = true;
				for (int i = 0; i < 4; i++) {
					mBrokenFlg[i] = true;
				}
				// スコアを加算する
				mNPCEvent->addScore(4800);
				// 効果音再生
				SE::getIns()->playSE(SE_EnemyStrikenBlaster);
				// BGM停止
				BGM::getIns()->stopBGM(BGM_Boss);
			}
		}
		// 左上に当たったら
		{
			x = (int)mX + 26;
			y = (int)mY + 26 + (int)mCameraY;
			if (!mBrokenFlg[0] &&
				bx + bw >= x && bx <= x + w &&
				by + bh >= y && by <= y + h) {
				mBrokenFlg[0] = true;
				// スコアを加算する
				mNPCEvent->addScore(1000);
				// 効果音再生
				SE::getIns()->playSE(SE_EnemyStrikenBlaster);
			}
		}
		// 右上に当たったら
		{
			x = (int)mX + 58;
			y = (int)mY + 26 + (int)mCameraY;
			if (!mBrokenFlg[1] &&
				bx + bw >= x && bx <= x + w &&
				by + bh >= y && by <= y + h) {
				mBrokenFlg[1] = true;
				// スコアを加算する
				mNPCEvent->addScore(1000);
				// 効果音再生
				SE::getIns()->playSE(SE_EnemyStrikenBlaster);
			}
		}
		// 左下に当たったら
		{
			x = (int)mX + 26;
			y = (int)mY + 58 + (int)mCameraY;
			if (!mBrokenFlg[2] &&
				bx + bw >= x && bx <= x + w &&
				by + bh >= y && by <= y + h) {
				mBrokenFlg[2] = true;
				// スコアを加算する
				mNPCEvent->addScore(1000);
				// 効果音再生
				SE::getIns()->playSE(SE_EnemyStrikenBlaster);
			}
		}
		// 右上に当たったら
		{
			x = (int)mX + 58;
			y = (int)mY + 58 + (int)mCameraY;
			if (!mBrokenFlg[3] &&
				bx + bw >= x && bx <= x + w &&
				by + bh >= y && by <= y + h) {
				mBrokenFlg[3] = true;
				// スコアを加算する
				mNPCEvent->addScore(1000);
				// 効果音再生
				SE::getIns()->playSE(SE_EnemyStrikenBlaster);
			}
		}
	}
}

/*------------------------------------------------------------------------------
ブラスターとの当たり判定
------------------------------------------------------------------------------*/
void AddorGuileness::collision()
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
int AddorGuileness::getCollisionPoint(int aPos) const
{
	switch (aPos) {
	case 0: // x
		return (int)mX;
	case 1: // y
		if (!mIsStriken) {
			return (int)mY + (int)mCameraY;
		}
		else {
			return (int)mY;
		}		
	case 2: // w
		return 96;
	case 3: // h
		return 96;
	}
	return -1;
}

/*------------------------------------------------------------------------------
タグを返す
------------------------------------------------------------------------------*/
eTag AddorGuileness::getTag() const
{
	return Tag_Enemy_Ground;
}
