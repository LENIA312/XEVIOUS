#include <DxLib.h>
#include "Terrazi.h"
#include "Image.h"
#include "Define.h"
#include "SE.h"

/*------------------------------------------------------------------------------
�R���X�g���N�^
------------------------------------------------------------------------------*/
Terrazi::Terrazi(InterfaceNPCEvent* aNPCEvent, NPCDATA aData, Player* aPlayer) : BaseNPC(aNPCEvent)
{
	mX = (float)aData.x;
	mY = (float)aData.y - 16;
	mPtn = aData.ptn;
	mPlayer = aPlayer;

	mSpeedX = -1;
	mSpeedY = -1;
	mLRFlg = false;
	mAcceleFlg = false;
	mShotFlg = false;

	// �ʒu�̏�����
	init();
}

/*------------------------------------------------------------------------------
�f�X�g���N�^
------------------------------------------------------------------------------*/
Terrazi::~Terrazi()
{

}

/*------------------------------------------------------------------------------
�X�V
------------------------------------------------------------------------------*/
void Terrazi::update()
{
	// �v���C���[���W
	int px = mPlayer->getCollisionPoint(0);
	int pw = mPlayer->getCollisionPoint(2);
	// �ړ�
	{
		mX += mSpeedX;
		mY += mSpeedY;
		// �v���C���[�Ǝ����d�Ȃ�����A�܂��̓U�b�p�[�ɓ����肻���ɂȂ���������t���O�𗧂Ă�
		if (!mAcceleFlg && !mLRFlg && mX + 16 >= px && !mIsStriken ||
			!mAcceleFlg && mLRFlg && mX <= px + pw && !mIsStriken) {

			mSpeedX *= 0.5;
			mSpeedY *= 0.5;
			if (mSpeedX <= 0.02) {
				mAcceleFlg = true;
				mShotFlg = true;
			}
			// �p�^�[��1�Ȃ�X�p���I�e����
			if (mPtn >= 1 && mShotFlg) {
				for (int i = 0; i < 3; i++) {

					mNPCEvent->generateEnemyBullet(0, 0, (int)mX + GetRand(15) + 9, (int)mY + GetRand(15) + 8);

				}
			

			}
		}
		// �����t���O�������Ă��鎞
		if (mAcceleFlg) {
			// ������
			if (!mLRFlg) {
				mSpeedX -= (float)0.15;
				mSpeedY -= (float)0.2;
				// �A�j��
				if (!mIsStriken) {
					mAnime += (float)0.1;
					if (mAnime >= 7) {
						mAnime = 0;
					}
				}
			}
			// �E����
			else {
				mSpeedX += (float)0.15;
				mSpeedY -= (float)0.2;
				// �A�j��
				if (!mIsStriken) {
					mAnime -= (float)0.1;
					if (mAnime <= -1) {
						mAnime = 6;
					}
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
void Terrazi::draw() const
{
	// �ʏ�
	if (!mIsStriken) {
		switch (mColor) {
		case 0:
			DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_a, (int)mAnime), true);
			break;
		case 1:
			DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_b, (int)mAnime), true);
			break;
		case 2:
			DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_c, (int)mAnime), true);
			break;
		case 3:
			DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_d, (int)mAnime), true);
			break;
		case 4:
			DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_e, (int)mAnime), true);
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
void Terrazi::init()
{
	int pcx = mPlayer->getCollisionPoint(0) + mPlayer->getCollisionPoint(2) / 2;
	// �ǂ�����o�����邩���v���C���[�̈ʒu�Ō��߂�
	{
		// �����ɃZ�b�g
		if (pcx >= 208) {
			mLRFlg = false;
		}
		// �E���ɃZ�b�g
		else if (pcx < 144) {
			mLRFlg = true;
		}
		// ���E�����_��
		if (pcx >= 144 && pcx < 208) {
			mLRFlg = GetRand(1);
		}
	}
	// �����ʒu�ƃX�s�[�h���Z�b�g
	{
		// �����̏ꍇ
		if (!mLRFlg) {
			mX = (float)(GetRand(64) + 48);
			mSpeedX = /*GetRand(0.3) + 2*/2;
		}
		// �E���̏ꍇ
		else {
			mX = (float)(GetRand(64) + 240);
			mSpeedX = 2;
			mSpeedX *= -1;
		}
		// �c�X�s�[�h
		mSpeedY = 22;
		mSpeedY *= (float)0.1;
	}
}

/*------------------------------------------------------------------------------
�|���ꂽ�Ƃ���
------------------------------------------------------------------------------*/
void Terrazi::strikenEvent()
{
	mAnime += (float)0.8;
	if (mAnime > 5) {
		mActiveFlag = false;
	}
}

/*------------------------------------------------------------------------------
�U�b�p�[�ɓ���������
------------------------------------------------------------------------------*/
void Terrazi::collisionGameObject(eTag aTag)
{
	if (aTag == Tag_Zapper && !mIsStriken) {
		// �|���ꂽ�t���O�𗧂Ă�
		mIsStriken = true;
		// �A�j���ʒu��������
		mAnime = 0;
		// �X�R�A�����Z����
		mNPCEvent->addScore(700);
		// ���ʉ��Đ�
		SE::getIns()->playSE(SE_EnemyStrikenZapper);
	}
}

/*------------------------------------------------------------------------------
�U�b�p�[�Ƃ̓����蔻��
------------------------------------------------------------------------------*/
void Terrazi::collision()
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
int Terrazi::getCollisionPoint(int aPos) const
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
eTag Terrazi::getTag() const
{
	return Tag_Enemy_Sky;
}