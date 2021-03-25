#include <DxLib.h>
#include "Kapi.h"
#include "Image.h"
#include "Define.h"
#include "SE.h"

/*------------------------------------------------------------------------------
�R���X�g���N�^
------------------------------------------------------------------------------*/
Kapi::Kapi(InterfaceNPCEvent* aNPCEvent, NPCDATA aData, Player* aPlayer) : BaseNPC(aNPCEvent)
{
	mPtn = aData.ptn;
	mX = (float)aData.x;
	mY = (float)aData.y - 16;
	mPlayer = aPlayer;

	mTimer = 0;
	mEscapeTimer = 0;
	mSparioTimer = 0;
	mSpeedX = 0;
	mSpeedY = 0;
	mLRFlg = false;
	mAcceleFlg = false;

	// �ʒu������
	init();
}

/*------------------------------------------------------------------------------
�f�X�g���N�^
------------------------------------------------------------------------------*/
Kapi::~Kapi()
{

}

/*------------------------------------------------------------------------------
�X�V
------------------------------------------------------------------------------*/
void Kapi::update()
{
	// �v���C���[���W
	int px = mPlayer->getCollisionPoint(0);
	int pw = mPlayer->getCollisionPoint(2);
	// �ړ�
	{
		mX += mSpeedX;
		mY += mSpeedY;
		// �X�p���I����
		{
			int r = GetRand(60);
			// 60����1�̊m���Œe������
			if (mSparioTimer >= 40 && r == 0) {
				mNPCEvent->generateEnemyBullet(0, 0, (int)mX + 8, (int)mY + 8);
				mSparioTimer = 0;
			}
			mSparioTimer++;
		}
		// ��莞�Ԃ��������������
		if (mTimer >= mEscapeTimer ) {
			mAcceleFlg = true;
		}
		else {
			mTimer++;
		}
		// �����t���O�������Ă��鎞
		if (mAcceleFlg) {
			// ������
			if (!mLRFlg) {
				mSpeedX -= (float)0.07;
			}
			// �E����
			else {
				mSpeedX += (float)0.07;
			}
			mSpeedY -= (float)0.07;
			// �A�j��
			if (!mIsStriken) {
				mAnime += (float)0.15;
				if (mAnime >= 7) {
					mAnime = 6;
				}
			}
		}
	}
	// �|����Ă��Ȃ���
	if (!mIsStriken) {
		collision();
	}
	// �|���ꂽ��
	else {
		strikenEvent();
	}
	// ��ʊO�ɏo����t���O��܂�
	if (mX + 16 < 0 ||
		mX > Define::GameScreenWidth - 128 ||
		mY + 16 < mCameraY ||
		mY > mCameraY + Define::GameScreenHeight) {
		mActiveFlag = false;
	}
}

/*------------------------------------------------------------------------------
 �`��
------------------------------------------------------------------------------*/
void Kapi::draw() const
{
	// �ʏ�
	if (!mIsStriken) {
		switch (mColor) {
		case 0:
			DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_a, 24 + (int)mAnime), true);
			break;
		case 1:
			DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_b, 24 + (int)mAnime), true);
			break;
		case 2:
			DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_c, 24 + (int)mAnime), true);
			break;
		case 3:
			DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_d, 24 + (int)mAnime), true);
			break;
		case 4:
			DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_e, 24 + (int)mAnime), true);
			break;
		}
	}
	// �j��
	else {
		DrawGraph((int)mX - 8, ((int)mY - 8) - (int)mCameraY, Image::getIns()->getImage(Image_EnemyStriken, (int)mAnime), true);
	}
}

/*------------------------------------------------------------------------------
�o���ʒu�̏�����
------------------------------------------------------------------------------*/
void Kapi::init()
{
	int pcx = mPlayer->getCollisionPoint(0) + mPlayer->getCollisionPoint(2) / 2;
	int pcy = mPlayer->getCollisionPoint(1) + mPlayer->getCollisionPoint(3) / 2;
	// �o���ʒu�̓����_��
	mLRFlg = GetRand(1);
	// �����ʒu�ƃX�s�[�h���Z�b�g
	{
		// �����̏ꍇ
		if (!mLRFlg) {
			mX = (float)(GetRand(64) + 48);
		}
		// �E���̏ꍇ
		else {
			mX = (float)(GetRand(64) + 240);
		}
		// �X�s�[�h
		mSpeedX = (pcx - mX) * (float)0.006;
		mSpeedY = (pcy - mY) * (float)0.002;
		mSpeedY += (1 - std::abs(mSpeedX)) * (float)0.1;
		mSpeedY *= (float)2.5;
	}
	// �P�ނ܂ł̃^�C�}�[
	mEscapeTimer = GetRand(90) + 10;
}

/*------------------------------------------------------------------------------
�|���ꂽ�Ƃ���
------------------------------------------------------------------------------*/
void Kapi::strikenEvent()
{
	mAnime += (float)0.8;
	if (mAnime > 5) {
		mActiveFlag = false;
	}
}

/*------------------------------------------------------------------------------
�U�b�p�[�ɓ���������
------------------------------------------------------------------------------*/
void Kapi::collisionGameObject(eTag aTag)
{
	if (aTag == Tag_Zapper && !mIsStriken) {
		// �|���ꂽ�t���O�𗧂Ă�
		mIsStriken = true;
		// �A�j���ʒu��������
		mAnime = 0;
		// �X�R�A�����Z����
		mNPCEvent->addScore(300);
		// ���ʉ��Đ�
		SE::getIns()->playSE(SE_EnemyStrikenZapper);
	}
}

/*------------------------------------------------------------------------------
�U�b�p�[�Ƃ̓����蔻��
------------------------------------------------------------------------------*/
void Kapi::collision()
{
	// �G�̍��W
	int ex = getCollisionPoint(0);
	int ey = getCollisionPoint(1);
	int ew = getCollisionPoint(2);
	int eh = getCollisionPoint(3);
	// �U�b�p�[���G�ɓ���������
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
�����蔻����W��Ԃ�
------------------------------------------------------------------------------*/
int Kapi::getCollisionPoint(int aPos) const
{
	switch (aPos) {
	case 0: // x
		return (int)mX + 1;
	case 1: // y
		return (int)mY + 1;
	case 2: // w
		return 14;
	case 3: // h
		return 14;
	}
	return -1;
}

/*------------------------------------------------------------------------------
�^�O��Ԃ�
------------------------------------------------------------------------------*/
eTag Kapi::getTag() const
{
	return Tag_Enemy_Sky;
}