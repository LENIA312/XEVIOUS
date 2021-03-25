#include <DxLib.h>
#include "Toroid.h"
#include "Image.h"
#include "Define.h"
#include "SE.h"

/*------------------------------------------------------------------------------
�R���X�g���N�^
------------------------------------------------------------------------------*/
Toroid::Toroid(InterfaceNPCEvent* aNPCEvent, NPCDATA aData, Player* aPlayer) : BaseNPC(aNPCEvent)
{
	mPtn = aData.ptn;
	mX = (float)aData.x;
	mY = (float)aData.y - 16;
	mPlayer = aPlayer;

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
Toroid::~Toroid()
{

}

/*------------------------------------------------------------------------------
�X�V
------------------------------------------------------------------------------*/
void Toroid::update()
{
	// �v���C���[���W
	int px = mPlayer->getCollisionPoint(0);
	int pw = mPlayer->getCollisionPoint(2);
	// �ړ�
	{
		mX += mSpeedX;
		mY += mSpeedY;
		// �v���C���[�Ǝ����d�Ȃ���������t���O�𗧂Ă�
		if (!mAcceleFlg && !mLRFlg && mX + 16 >= px && !mIsStriken ||
			!mAcceleFlg &&  mLRFlg && mX <= px + pw && !mIsStriken ) {
			mAcceleFlg = true;
			// �p�^�[��1�Ȃ�X�p���I�e����
			if (mPtn >= 1) {
				mNPCEvent->generateEnemyBullet(0, 0, (int)mX + 8, (int)mY + 8);
			}
		}
		// �����t���O�������Ă��鎞
		if (mAcceleFlg) {
			// ������
			if (!mLRFlg) {
				mSpeedX -= (float)0.07;
				// �A�j��
				if (!mIsStriken) {
					mAnime += 0.5;
					if (mAnime >= 8) {
						mAnime = 0;
					}
				}
			}
			// �E����
			else {
				mSpeedX += (float)0.07;
				// �A�j��
				if (!mIsStriken) {
					mAnime -= (float)0.6;
					if (mAnime < 0) {
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
		mY > mCameraY + Define::GameScreenHeight) {
		mActiveFlag = false;
	}
}

/*------------------------------------------------------------------------------
 �`��
------------------------------------------------------------------------------*/
void Toroid::draw() const
{
	// �ʏ�
	if (!mIsStriken) {
		switch (mColor) {
		case 0:
			DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_a, 8 + (int)mAnime), true);
			break;
		case 1:
			DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_b, 8 + (int)mAnime), true);
			break;
		case 2:
			DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_c, 8 + (int)mAnime), true);
			break;
		case 3:
			DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_d, 8 + (int)mAnime), true);
			break;
		case 4:
			DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_e, 8 + (int)mAnime), true);
			break;
		}
	}
	// �j��
	else {
		DrawGraph((int)mX - 8, ((int)mY - 8) - (int)mCameraY, Image::getIns()->getImage(Image_EnemyStriken, (int)mAnime), true);
	}
	// �f�o�b�O
	//clsDx();
	//printfDx("x%0.1f y%0.1f",mSpeedX, mSpeedY);
	//DrawLine(mPlayerX, 0, mPlayerX, 300, GetColor(0, 0, 0));
}

/*------------------------------------------------------------------------------
�o���ʒu�̏�����
------------------------------------------------------------------------------*/
void Toroid::init()
{
	int pcx = mPlayer->getCollisionPoint(0) + mPlayer->getCollisionPoint(2) / 2;
	int pcy = mPlayer->getCollisionPoint(1) + mPlayer->getCollisionPoint(3) / 2;
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
		}
		// �E���̏ꍇ
		else {
			mX = (float)(GetRand(64) + 240);
		}
		// �X�s�[�h
		mSpeedX = (pcx - mX) * (float)0.006;
		mSpeedY = (pcy - mY) * (float)0.002;
		mSpeedY += (1 - std::abs(mSpeedX)) * (float)0.1;
	}
}

/*------------------------------------------------------------------------------
�|���ꂽ�Ƃ���
------------------------------------------------------------------------------*/
void Toroid::strikenEvent()
{
	mAnime += (float)0.8;
	if (mAnime > 5) {
		mActiveFlag = false;
	}
}

/*------------------------------------------------------------------------------
�U�b�p�[�ɓ���������
------------------------------------------------------------------------------*/
void Toroid::collisionGameObject(eTag aTag)
{
	// ���������I�u�W�F�N�g���U�b�p�[�Ȃ�
	if (aTag == Tag_Zapper && !mIsStriken) {
		// �|���ꂽ�t���O�𗧂Ă�
		mIsStriken = true;
		// �A�j���ʒu��������
		mAnime = 0;
		// �X�R�A�����Z����
		mNPCEvent->addScore(30);
		// ���ʉ��Đ�
		SE::getIns()->playSE(SE_EnemyStrikenZapper);
	}
}

/*------------------------------------------------------------------------------
�U�b�p�[�Ƃ̓����蔻��
------------------------------------------------------------------------------*/
void Toroid::collision()
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
int Toroid::getCollisionPoint(int aPos) const
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
�^�O��Ԃ�
------------------------------------------------------------------------------*/
eTag Toroid::getTag() const
{
	return Tag_Enemy_Sky;
}
