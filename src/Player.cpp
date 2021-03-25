#include <DxLib.h>
#include "Player.h"
#include "Pad.h"
#include "Image.h"
#include "SE.h"
#include "Define.h"

/*------------------------------------------------------------------------------
��`
------------------------------------------------------------------------------*/
const static float PLAYER_SPEED   = 2;
const static int RAPID_ZAPPR_TIME = 6;

/*------------------------------------------------------------------------------
�R���X�g���N�^
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
�f�X�g���N�^
------------------------------------------------------------------------------*/
Player::~Player()
{	
	delete[] mZapper;
	delete mBlaster;
}

/*------------------------------------------------------------------------------
�X�V
------------------------------------------------------------------------------*/
void Player::update()
{
	// �e�I�u�W�F�N�g
	{
		for (int i = 0; i < Define::ZapperMax; i++) {
			mZapper[i].setCameraY(mCameraY);
			mZapper[i].update();
		}
		mBlaster->setCameraY(mCameraY);
		mBlaster->update();
	}
	// �v���C���[
	{
		// �ʏ펞
		if (mActiveFlag) {
			move();
			zapper();
			blaster();
		}
		// ���Ă��ꂽ��
		else {
			if (mAnime < 7) {
				mAnime += (float)0.2;
			}
		}
	}
}

/*------------------------------------------------------------------------------
�`��
------------------------------------------------------------------------------*/
void Player::draw() const
{
	// �u���X�^�[
	mBlaster->draw();
	// �Ə�
	{
		if (mActiveFlag) {
			// �ʏ�	
			if (!mBlaster->checkActiveFlag()) {
				DrawGraph((int)mX, (int)mY - 96,
					Image::getIns()->getImage(Image_Aiming, (int)mAimingAnime), TRUE);
			}
			// �u���X�^�[����������
			else {
				DrawGraph((int)mX, (int)mY - 96,
					Image::getIns()->getImage(Image_Aiming, (int)mAimingAnime + 2), TRUE);
			}
		}
	}
	// �U�b�p�[
	for (int i = 0; i < Define::ZapperMax; i++) {
		mZapper[i].draw();
	}
	// �v���C���[
	{
		// �ʏ펞
		if (mActiveFlag) {
			DrawGraph((int)mX, (int)mY,
				Image::getIns()->getImage(Image_Player), TRUE);
		}
		// ���Ă��ꂽ�Ƃ�
		else if (mAnime < 7) {
			DrawGraph((int)mX - 8, (int)mY - 8,
				Image::getIns()->getImage(Image_PlayerBroken, (int)mAnime), TRUE);
		}
	}
	// �f�o�b�O
	//DrawFormatString(0,0,GetColor(255,255,255),"y:%d y - camera:%d", (int)mY,(int)mY - (int)mCameraY);
}

/*------------------------------------------------------------------------------
�����蔻��`��
------------------------------------------------------------------------------*/
void Player::drawCollision(int r, int g, int b) const {
	// ���@
	{
		int x = getCollisionPoint(0);
		int y = getCollisionPoint(1) - (int)mCameraY;
		int w = getCollisionPoint(2);
		int h = getCollisionPoint(3);
		DrawBox(x, y, x + w, y + h, GetColor(r, g, b), FALSE);
	}
	// �Ə�
	{
		int x = (int)mX + 2;
		int y = (int)mY + 2 - 96;
		int w = mBlaster->getCollisionPoint(2);
		int h = mBlaster->getCollisionPoint(3);
		DrawBox(x, y, x + w, y + h, GetColor(0, 255, 0), FALSE);
	}
	// �U�b�p�[
	for (int i = 0; i < Define::ZapperMax; i++) {
		if (mZapper[i].checkActiveFlag()) {
			mZapper[i].drawCollision(r, g, b);
		}
	}
	// �u���X�^�[
	if (mBlaster->checkActiveFlag()) {
		mBlaster->drawCollision(r, g, b);
	}
}

/*------------------------------------------------------------------------------
�ړ�
------------------------------------------------------------------------------*/
void Player::move()
{
	float tmpSpeed = 0;
	// SHIFT�L�[�Ō�������
	{
		if (Pad::getIns()->get(Pad_LShift) || Pad::getIns()->get(Pad_RShift)) {
			tmpSpeed = PLAYER_SPEED / 2;
		}
		if (Pad::getIns()->get(Pad_LShift) == 0 && Pad::getIns()->get(Pad_RShift) == 0) {
			tmpSpeed = PLAYER_SPEED;
		}
	}
	// �ړ�
	{
		// ��
		if (Pad::getIns()->get(Pad_Up)) {
			mY -= tmpSpeed;
		}
		// ��
		if (Pad::getIns()->get(Pad_Down)) {
			mY += tmpSpeed * (float)0.7;
		}
		// ��
		if (Pad::getIns()->get(Pad_Left)) {
			mX -= tmpSpeed;
		}
		// ��
		if (Pad::getIns()->get(Pad_Right)) {
			mX += tmpSpeed;
		}
	}
	// ��ʊO�ɏo�Ȃ��悤�ɍ��W�␳
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
�U�b�p�[
------------------------------------------------------------------------------*/
void Player::zapper()
{
	if (Pad::getIns()->get(Pad_Z)) {
		if (mRpidZapperTimer == 0) {
			for (int i = 0; i < Define::ZapperMax; i++) {
			    // �t���O���I�t��������
				if (!mZapper[i].checkActiveFlag()) {
				    // �U�b�p�[�𐶐�
					mZapper[i].generate((int)mX, (int)mY);
					break;
				}
			}
		}
		// �A�ˑ��x�̒���
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
�u���X�^�[
------------------------------------------------------------------------------*/
void Player::blaster()
{
	// X�L�[�Ŕ���
	{
		if (Pad::getIns()->get(Pad_X)) {
			// �t���O���I�t��������
			if (!mBlaster->checkActiveFlag()) {
				// �u���X�^�[�𐶐�
				mBlaster->generate((int)mX, (int)mY);
			}
		}
	}
	// �Ə��`�J�`�J
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
�����蔻��̍��W��Ԃ�
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
�U�b�p�[��Ԃ�
------------------------------------------------------------------------------*/
Zapper* Player::getZapper() const
{
	return mZapper;
}

/*------------------------------------------------------------------------------
�u���X�^�[��Ԃ�
------------------------------------------------------------------------------*/
Blaster* Player::getBlaster() const
{
	return mBlaster;
}

/*------------------------------------------------------------------------------
���̃Q�[���I�u�W�F�N�g�ɓ����������̏���
------------------------------------------------------------------------------*/
void Player::collisionGameObject(eTag aTag)
{
	switch (aTag) {
	case Tag_Enemy_Bullet:
	case Tag_Enemy_Sky:
		// �t���O��܂�
		mActiveFlag = false;
		// �A�j�������ʒu
		mAnime = 0;
		// SE�炷
		SE::getIns()->playSE(SE_Miss);
		break;
	}
}

/*------------------------------------------------------------------------------
�^�O��Ԃ�
------------------------------------------------------------------------------*/
eTag Player::getTag() const
{
	return Tag_Player;
}

/*------------------------------------------------------------------------------
���Z�b�g
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
�Ə��`�J�`�J������
------------------------------------------------------------------------------*/
void Player::setTikaTikaFlg(bool aFlg)
{
	mTikaTikaFlg = aFlg;
}

/*------------------------------------------------------------------------------
�Ə��̓����蔻���Ԃ�
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