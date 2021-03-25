#include <DxLib.h>
#include<math.h>
#include "Sheonite.h"
#include "Image.h"
#include "Define.h"
#include "SE.h"
/*------------------------------------------------------------------------------
�R���X�g���N�^
------------------------------------------------------------------------------*/
Sheonite::Sheonite(InterfaceNPCEvent* aNPCEvent, NPCDATA aData, Player* aPlayer) : BaseNPC(aNPCEvent)
{
	mPtn = aData.ptn;
	mX[0] = (float)aData.x;
	mY[0] = (float)aData.y - 16;
	mPlayer = aPlayer;

	mSpeed = -1; // �ړ��X�s�[�h
	mMotionNum = 0; // ���݂̃��[�V�����ԍ�
	mTT = 0; // �^�C�}�[
	mAnimSpeed = 0.3; // �A�j���[�V�����Đ����x
	mTransFlg[0] = mTransFlg[1] = false;
	mFlyCnt = 0;

	// �ʒu������
	init();
}

/*------------------------------------------------------------------------------
�f�X�g���N�^
------------------------------------------------------------------------------*/
Sheonite::~Sheonite()
{

}

/*------------------------------------------------------------------------------
�X�V
------------------------------------------------------------------------------*/
void Sheonite::update()
{
	// �v���C���[���W
	int px = mPlayer->getCollisionPoint(0);
	int py = mPlayer->getCollisionPoint(1);
	int pw = mPlayer->getCollisionPoint(2);
	int ph = mPlayer->getCollisionPoint(3);

	//***************************�ړ�***************************************

	switch (mMotionNum)
	{
	case 0:
		// 1���@�̏���
		if (py - ph > mY[0]) {
			// ���@�Ƃ̃x�N�g�����v�Z
			mTheta[0] = atan2((py - ph) - mY[0], (px + pw) - mX[0]);
			mVx[0] = cos(mTheta[0]);
			mVy[0] = sin(mTheta[0]);
			//���@�̕����ֈړ�
			mX[0] = mX[0] + mVx[0] * mSpeed;
			mY[0] = mY[0] + mVy[0] * mSpeed;
		}else {
			mTransFlg[0] = true; //���������t���O���I��
			mX[0] = px + pw; mY[0] = py - ph; // ���W���Z�b�g
		}

		// 2���@�̏���
		if (py - ph > mY[1]) {
			// ���@�Ƃ̃x�N�g�����v�Z
			mTheta[1] = atan2((py - ph) - mY[1], (px - pw) - mX[1]);
			mVx[1] = cos(mTheta[1]);
			mVy[1] = sin(mTheta[1]);
			//���@�̕����ֈړ�
			mX[1] = mX[1] + mVx[1] * mSpeed;
			mY[1] = mY[1] + mVy[1] * mSpeed;
		}else {
			mTransFlg[1] = true; //���������t���O���I��
			mX[1] = px - pw;mY[1] = py - ph;
		}

		// 2�Ƃ������ɍ��̂�����
		if (mTransFlg[0] && mTransFlg[1]) {
			mMotionNum = 1; // ���̃��[�V������
		}
		break;
	case 1:
		// �^�C�}�[���N��
		mTT++;

		// ���W���Z�b�g
		mX[0] = px + pw;mY[0] = py - ph;
		mX[1] = px - pw;mY[1] = py - ph;

		// 600�t���[�����
		if (mTT == 600) {
			mAnimMax = 4; // �A�j���[�V�����̍ő�l
			mAnimSpeed = 0.1; // �A�j���[�V�����̍Đ����x
			mAnime = 0; 
			mAnimP[0] = 10;mAnimP[1] = 18; // �Đ�����A�j���[�V�����ԍ�
		}

		if (40 + mAnimP[0] + mAnime == 54) {
			mMotionNum = 2; // ���̃��[�V������
			mTT = 0; // �^�C�}�[�̏�����
			mAnimSpeed = 0; // �A�j���[�V�����̍Đ����x
		}
		break;
	case 2:
		mBetX++;
		mY[0] = mY[1]= py - ph;
		mX[0] = px + pw - mBetX;
		mX[1] = px - pw + mBetX;

		if (mX[0] == px) {
			mMotionNum = 3;// ���̃��[�V������
			mAnime = 0;
		}
		break;
	case 3:
		mAnimP[0] = 8;mAnimP[1] = 16; // �Đ�����A�j���[�V�����ԍ�
		mAnimMax = 2; // �A�j���[�V�����̍ő�l
		mAnimSpeed = 0.5; // �A�j���[�V�����̍Đ����x
		// ���ł���
		mY[0] -= mSpeed;
		mY[1] = mSpeed;
		mFlyCnt++;
		break;
	default:
		break;
	}

	// �P�ނ�SE���Đ�����
	if (mFlyCnt == 1)SE::getIns()->playSE(SE_ShioniteDocking);

	//�A�j���Đ�
	mAnime += mAnimSpeed;
	if (mAnime >= mAnimMax) {
		mAnime = 0;
	}

	// ��ʊO�ɏo����t���O��܂�
	if (mY[0] + 16 < mCameraY) {
		mActiveFlag = false;
	}
}

/*------------------------------------------------------------------------------
 �`��
------------------------------------------------------------------------------*/
void Sheonite::draw() const
{
	// �F�ɂ���ĕ\������摜��ς���
	if (!mIsStriken) {
		for (int i = 0; i < 2; i++) {
				switch (mColor) {
				case 0:
					DrawGraph((int)mX[i], (int)mY[i] - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_a, (40 + mAnimP[i]) + (int)mAnime), true);
					break;
				case 1:
					DrawGraph((int)mX[i], (int)mY[i] - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_b, (40 + mAnimP[i]) + (int)mAnime), true);
					break;
				case 2:
					DrawGraph((int)mX[i], (int)mY[i] - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_c, (40 + mAnimP[i]) + (int)mAnime), true);
					break;
				case 3:
					DrawGraph((int)mX[i], (int)mY[i] - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_d, (40 + mAnimP[i]) + (int)mAnime), true);
					break;
				case 4:
					DrawGraph((int)mX[i], (int)mY[i] - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_e, (40 + mAnimP[i]) + (int)mAnime), true);
					break;
				}
		}
	}
}

/*------------------------------------------------------------------------------
������
------------------------------------------------------------------------------*/
void Sheonite::init()
{
	// �c�X�s�[�h���Z�b�g
	mSpeed = 7;
	mAnimMax = 4;
	//2�����Z�b�g
	mX[1] = mX[0] + 100;
	mY[1] = mY[0];
	mBetX = 0;
	//�A�j���Đ��ʒu���Z�b�g
	mAnimP[0] = 0;mAnimP[1] = 4;
}

/*------------------------------------------------------------------------------
�����蔻����W��Ԃ�
------------------------------------------------------------------------------*/
int Sheonite::getCollisionPoint(int aPos) const
{
	switch (aPos) {
	case 0: // x
		return (int)mX;
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
�^�O��Ԃ�
------------------------------------------------------------------------------*/
eTag Sheonite::getTag() const
{
	return Tag_None;
}