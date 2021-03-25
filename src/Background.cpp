#include <DxLib.h>
#include "Background.h"
#include "Image.h"
#include "Define.h"

/*------------------------------------------------------------------------------
�R���X�g���N�^
------------------------------------------------------------------------------*/
Background::Background()
{
	// ���[�h
	mMode = 0;
	// y���W
	mCameraY = (float)(Define::MapHeight + Define::ForestHeight - Define::GameScreenHeight);
	mTmpCameraY = mCameraY;
	mBackgroundY[0] = 0;
	mBackgroundY[1] = Define::MapHeight;
	mForestY		= Define::MapHeight;
	// �X�\���t���O
	mIsForest = true;
	// �G���A����x���W
	mAreaPoint[ 0] = 480;
	mAreaPoint[ 1] = 736;
	mAreaPoint[ 2] =  80;
	mAreaPoint[ 3] = 608;
	mAreaPoint[ 4] = 240;
	mAreaPoint[ 5] = 528;
	mAreaPoint[ 6] =  16;
	mAreaPoint[ 7] = 720;
	mAreaPoint[ 8] = 432;
	mAreaPoint[ 9] =   0;
	mAreaPoint[10] = 608;
	mAreaPoint[11] = 720;
	mAreaPoint[12] = 304;
	mAreaPoint[13] = 80;
	mAreaPoint[14] = 560;
	mAreaPoint[15] = 736;
	// ���݂̃G���A
	mNowArea[0] = 0;
	mNowArea[1] = 0;
	// �ĊJ����G���A
	mRestartArea = 0;
	mCanAddRestarArea = true;
}

/*------------------------------------------------------------------------------
�f�X�g���N�^
------------------------------------------------------------------------------*/
Background::~Background()
{
}

/*------------------------------------------------------------------------------
�X�V
------------------------------------------------------------------------------*/
void Background::update()
{
	// �ʏ탂�[�h
	if (mMode == 0) {
		// �O�t���[���̃J�����̈ʒu�Ɣ�r���ăG���A��ύX������
		if (mTmpCameraY < mCameraY) {
			mNowArea[0]++;
			mNowArea[1] = mNowArea[0] - 1;
			mCanAddRestarArea = true;
			mRestartArea = mNowArea[0];
		}
		// �G���A�i�s�x��7���ȏ�Ȃ�ĊJ�G���A���X�V
		if (mCameraY <= Define::MapHeight * 0.3 && mCanAddRestarArea) {
			mRestartArea++;
			mCanAddRestarArea = false;
		}
		if (mRestartArea > 15) {
			mRestartArea = 6;
		}
		// �G���A16�܂ōs������G���A7��
		if (mNowArea[0] > 15) {
			mNowArea[0] = 6;
			mNowArea[1] = 15;
		}
		// �X�𒴂������ǂ����̔���
		if (mIsForest && mCameraY < Define::MapHeight - Define::GameScreenHeight) {
			mIsForest = false;
		}
		mTmpCameraY = mCameraY;
	}
	// �G�f�B�^�[�V�[��
	else {

	}
}

/*------------------------------------------------------------------------------
�`��
------------------------------------------------------------------------------*/
void Background::draw() const
{
	// �ʏ탂�[�h
	if (mMode == 0) {
		// �X
		if (mIsForest) {
			DrawGraph(0, mForestY - (int)mCameraY,
				Image::getIns()->getImage(Image_Forest), TRUE);
		}
		// �w�i1
		DrawGraph(-mAreaPoint[mNowArea[0]], mBackgroundY[0] - (int)mCameraY,
			Image::getIns()->getImage(Image_Background), TRUE);
		// �w�i2 : ��ʊO�̎��͕\�����Ȃ�
		if (mBackgroundY[1] - (int)mCameraY < Define::GameScreenHeight && !mIsForest) {
			DrawGraph(-mAreaPoint[mNowArea[1]], mBackgroundY[1] - (int)mCameraY,
				Image::getIns()->getImage(Image_Background), TRUE);
		}
	}
	// �G�f�B�^�[�V�[��
	else {
		// �X
		if (mForestY - (int)mCameraY < Define::GameScreenHeight) {
			DrawGraph(0, mForestY - (int)mCameraY,
				Image::getIns()->getImage(Image_Forest), TRUE);
		}
		
		// �w�i1
		DrawGraph(-mAreaPoint[mNowArea[0]], mBackgroundY[0] - (int)mCameraY,
			Image::getIns()->getImage(Image_Background), TRUE);
	}
	// �f�o�b�O
	//DrawFormatString(0, 0, GetColor(255, 255, 255), "Forest:%d Map1:%d Map2:%d\n %d",
	//	mForestY - (int)mCameraY, mBackgroundY[0], mBackgroundY[1], mRestartArea);
}

/*------------------------------------------------------------------------------
���Z�b�g
------------------------------------------------------------------------------*/
void Background::reset()
{
	// y���W
	mCameraY = float(Define::MapHeight + Define::ForestHeight - Define::GameScreenHeight);
	mTmpCameraY = mCameraY;
	mBackgroundY[0] = 0;
	mBackgroundY[1] = Define::MapHeight;
	mForestY = Define::MapHeight;
	// �X�\���t���O
	mIsForest = true;
	// �G���A
	mNowArea[0] = mRestartArea;
}

/*------------------------------------------------------------------------------
���݂̃G���A��Ԃ�
------------------------------------------------------------------------------*/
int Background::getArea()
{
	return mNowArea[0];
}

/*------------------------------------------------------------------------------
�J�����̍��W���Z�b�g
------------------------------------------------------------------------------*/
void Background::setCameraY(float aY)
{
	mCameraY = aY;
}

/*------------------------------------------------------------------------------
���[�h���Z�b�g
------------------------------------------------------------------------------*/
void Background::setMode(int aMode) {
	mMode = aMode;
	mTmpCameraY = mCameraY;
}

/*------------------------------------------------------------------------------
�G���A���Z
------------------------------------------------------------------------------*/
void Background::addArea(int aAdd) {
	mNowArea[0] += aAdd;
	if (mNowArea[0] < 0) {
		mNowArea[0] = 15;
	}
	if (mNowArea[0] > 15) {
		mNowArea[0] = 0;
	}
}