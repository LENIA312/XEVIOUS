#include <DxLib.h>
#include "Player.h"
#include "Pad.h"
#include "Image.h"
#include "SE.h"
#include "Define.h"

/*------------------------------------------------------------------------------
定義
------------------------------------------------------------------------------*/
const static float PLAYER_SPEED   = 2;
const static int RAPID_ZAPPR_TIME = 6;

/*------------------------------------------------------------------------------
コンストラクタ
------------------------------------------------------------------------------*/
Player::Player(int aX, int aY)
{
	mX = (float)aX;
	mY = (float)aY;

	mBlasterTargetY = 0;

	mRpidZapperTimer = 0;
	mAimingAnime = 0;

	mTikaTikaFlg = false;

	mZapper = new Zapper[Define::ZapperMax];
	mBlaster = new Blaster();

}

/*------------------------------------------------------------------------------
デストラクタ
------------------------------------------------------------------------------*/
Player::~Player()
{	
	delete[] mZapper;
	delete mBlaster;
}

/*------------------------------------------------------------------------------
更新
------------------------------------------------------------------------------*/
void Player::update()
{
	// 弾オブジェクト
	{
		for (int i = 0; i < Define::ZapperMax; i++) {
			mZapper[i].setCameraY(mCameraY);
			mZapper[i].update();
		}
		mBlaster->setCameraY(mCameraY);
		mBlaster->update();
	}
	// プレイヤー
	{
		// 通常時
		if (mActiveFlag) {
			move();
			zapper();
			blaster();
		}
		// 撃墜された時
		else {
			if (mAnime < 7) {
				mAnime += (float)0.2;
			}
		}
	}
}

/*------------------------------------------------------------------------------
描画
------------------------------------------------------------------------------*/
void Player::draw() const
{
	// ブラスター
	mBlaster->draw();
	// 照準
	{
		if (mActiveFlag) {
			// 通常	
			if (!mBlaster->checkActiveFlag()) {
				DrawGraph((int)mX, (int)mY - 96,
					Image::getIns()->getImage(Image_Aiming, (int)mAimingAnime), TRUE);
			}
			// ブラスターを撃った時
			else {
				DrawGraph((int)mX, (int)mY - 96,
					Image::getIns()->getImage(Image_Aiming, (int)mAimingAnime + 2), TRUE);
			}
		}
	}
	// ザッパー
	for (int i = 0; i < Define::ZapperMax; i++) {
		mZapper[i].draw();
	}
	// プレイヤー
	{
		// 通常時
		if (mActiveFlag) {
			DrawGraph((int)mX, (int)mY,
				Image::getIns()->getImage(Image_Player), TRUE);
		}
		// 撃墜されたとき
		else if (mAnime < 7) {
			DrawGraph((int)mX - 8, (int)mY - 8,
				Image::getIns()->getImage(Image_PlayerBroken, (int)mAnime), TRUE);
		}
	}
	// デバッグ
	//DrawFormatString(0,0,GetColor(255,255,255),"y:%d y - camera:%d", (int)mY,(int)mY - (int)mCameraY);
}

/*------------------------------------------------------------------------------
当たり判定描画
------------------------------------------------------------------------------*/
void Player::drawCollision(int r, int g, int b) const {
	// 自機
	{
		int x = getCollisionPoint(0);
		int y = getCollisionPoint(1) - (int)mCameraY;
		int w = getCollisionPoint(2);
		int h = getCollisionPoint(3);
		DrawBox(x, y, x + w, y + h, GetColor(r, g, b), FALSE);
	}
	// 照準
	{
		int x = (int)mX + 2;
		int y = (int)mY + 2 - 96;
		int w = mBlaster->getCollisionPoint(2);
		int h = mBlaster->getCollisionPoint(3);
		DrawBox(x, y, x + w, y + h, GetColor(0, 255, 0), FALSE);
	}
	// ザッパー
	for (int i = 0; i < Define::ZapperMax; i++) {
		if (mZapper[i].checkActiveFlag()) {
			mZapper[i].drawCollision(r, g, b);
		}
	}
	// ブラスター
	if (mBlaster->checkActiveFlag()) {
		mBlaster->drawCollision(r, g, b);
	}
}

/*------------------------------------------------------------------------------
移動
------------------------------------------------------------------------------*/
void Player::move()
{
	float tmpSpeed = 0;
	// SHIFTキーで減速する
	{
		if (Pad::getIns()->get(Pad_LShift) || Pad::getIns()->get(Pad_RShift)) {
			tmpSpeed = PLAYER_SPEED / 2;
		}
		if (Pad::getIns()->get(Pad_LShift) == 0 && Pad::getIns()->get(Pad_RShift) == 0) {
			tmpSpeed = PLAYER_SPEED;
		}
	}
	// 移動
	{
		// ↑
		if (Pad::getIns()->get(Pad_Up)) {
			mY -= tmpSpeed;
		}
		// ↓
		if (Pad::getIns()->get(Pad_Down)) {
			mY += tmpSpeed * (float)0.7;
		}
		// ←
		if (Pad::getIns()->get(Pad_Left)) {
			mX -= tmpSpeed;
		}
		// →
		if (Pad::getIns()->get(Pad_Right)) {
			mX += tmpSpeed;
		}
	}
	// 画面外に出ないように座標補正
	{
		if (mX <= 0) {
			mX = 0;
		}
		if (mY <= 96) {
			mY = 96;
		}
		if (mX >= 336) {
			mX = 336;
		}
		if (mY >= 254) {
			mY = 254;
		}
	}
}

/*------------------------------------------------------------------------------
ザッパー
------------------------------------------------------------------------------*/
void Player::zapper()
{
	if (Pad::getIns()->get(Pad_Z)) {
		if (mRpidZapperTimer == 0) {
			for (int i = 0; i < Define::ZapperMax; i++) {
			    // フラグがオフだったら
				if (!mZapper[i].checkActiveFlag()) {
				    // ザッパーを生成
					mZapper[i].generate((int)mX, (int)mY);
					break;
				}
			}
		}
		// 連射速度の調整
		if (mRpidZapperTimer < RAPID_ZAPPR_TIME) {
			mRpidZapperTimer++;
		}
		else {
			mRpidZapperTimer = 0;
		}
	}
	if (Pad::getIns()->get(Pad_Z) == 0) {
		mRpidZapperTimer = 0;
	}
}

/*------------------------------------------------------------------------------
ブラスター
------------------------------------------------------------------------------*/
void Player::blaster()
{
	// Xキーで発射
	{
		if (Pad::getIns()->get(Pad_X)) {
			// フラグがオフだったら
			if (!mBlaster->checkActiveFlag()) {
				// ブラスターを生成
				mBlaster->generate((int)mX, (int)mY);
			}
		}
	}
	// 照準チカチカ
	{
		if (!mTikaTikaFlg) {
			mAimingAnime = 0;
		}
		else {
			mAimingAnime += (float)0.2;
			if (mAimingAnime >= 2) {
				mAimingAnime = 0;
			}
		}
	}
}

/*------------------------------------------------------------------------------
当たり判定の座標を返す
------------------------------------------------------------------------------*/
int Player::getCollisionPoint(int aPos) const
{
	switch (aPos) {
	case 0: // x
		return (int)mX;
	case 1: // y
		return (int)mY + (int)mCameraY;
	case 2: // w
		return 16;
	case 3: // h
		return 16;
	}
	return -1;
}

/*------------------------------------------------------------------------------
ザッパーを返す
------------------------------------------------------------------------------*/
Zapper* Player::getZapper() const
{
	return mZapper;
}

/*------------------------------------------------------------------------------
ブラスターを返す
------------------------------------------------------------------------------*/
Blaster* Player::getBlaster() const
{
	return mBlaster;
}

/*------------------------------------------------------------------------------
他のゲームオブジェクトに当たった時の処理
------------------------------------------------------------------------------*/
void Player::collisionGameObject(eTag aTag)
{
	switch (aTag) {
	case Tag_Enemy_Bullet:
	case Tag_Enemy_Sky:
		// フラグを折る
		mActiveFlag = false;
		// アニメ初期位置
		mAnime = 0;
		// SE鳴らす
		SE::getIns()->playSE(SE_Miss);
		break;
	}
}

/*------------------------------------------------------------------------------
タグを返す
------------------------------------------------------------------------------*/
eTag Player::getTag() const
{
	return Tag_Player;
}

/*------------------------------------------------------------------------------
リセット
------------------------------------------------------------------------------*/
void Player::reset()
{
	mX = 168;
	mY = 224;
	mAnime = 0;
	mBlasterTargetY = 0;
	mRpidZapperTimer = 0;
	mAimingAnime = 0;
	mActiveFlag = true;
}

/*------------------------------------------------------------------------------
照準チカチカするやつ
------------------------------------------------------------------------------*/
void Player::setTikaTikaFlg(bool aFlg)
{
	mTikaTikaFlg = aFlg;
}

/*------------------------------------------------------------------------------
照準の当たり判定を返す
------------------------------------------------------------------------------*/
int Player::getAimngXY(int xy)
{
	switch (xy) {
	case 0:
		return (int)mX + 2;
	case 1:
		return (int)mY + 2 - 96 + (int)mCameraY;
	}
	return -1;
}