#include <DxLib.h>
#include "SetNPC.h"
#include "Image.h"

/*------------------------------------------------------------------------------
�R���X�g���N�^
------------------------------------------------------------------------------*/
SetNPC::SetNPC(int aType, int aPtn, int aX, int aY)
{
	mType = aType;
	mPtn = aPtn;
	mX = aX;
	mY = aY;
	mW = 0;
	mH = 0;
	mActive = true;
	mCameraY = 0;
	mMouseX = 0;
	mMouseY = 0;
	mMouseCL = 0;
	mMouseCR = 0;
	// �␳
	switch (mType) {
	case 0:case 1:case 2:case 3:case 4:case 5:
	case 6:case 7:case 8:case 9:case 11:case 12:
	case 13: case 20: case 21: case 22: case 23: case 16: // 16��
		mW = 16;
		mH = 16;
		break;
	case 14:case 15:case 17: // 32��
		mW = 32;
		mH = 32;
		break;
	case 18: // �{�X
		mW = 96;
		mH = 96;
		break;
	case 19: // �A�C�e��
		mW = 16;
		mH = 16;
		break;
	case 10: // �{�U���O����
		mW = 37;
		mH = 37;
		break;
	}
}

/*------------------------------------------------------------------------------
�f�X�g���N�^
------------------------------------------------------------------------------*/
SetNPC::~SetNPC()
{

}

/*------------------------------------------------------------------------------
�X�V
------------------------------------------------------------------------------*/
void SetNPC::update()
{
	int tmpX = mX;
	int tmpY = mY;
	// �{�U���O�����␳
	if (mType == 10) {
		tmpX -= 12;
		tmpY -= 12;
	}
	// ���N���b�N�Ńp�^�[���ύX
	if (mMouseCL == 1 &&
		mMouseX >= tmpX && mMouseX <= tmpX + mW &&
		mMouseY + mCameraY >= tmpY && mMouseY + mCameraY <= tmpY + mH) {
		mPtn++;
		if (mPtn > 9) {
			mPtn = 0;
		}
	}
	// �E�N���b�N�ŐG�ꂽ�������
	if (mMouseCR > 0 &&
		mMouseX >= tmpX && mMouseX <= tmpX + mW &&
		mMouseY + mCameraY >= tmpY && mMouseY + mCameraY <= tmpY + mH) {
		mActive = false;
	}
}

/*------------------------------------------------------------------------------
�`��
------------------------------------------------------------------------------*/
void SetNPC::draw() const
{
	switch (mType) {
	case 0:case 1:case 2:case 3:case 4:case 5:
	case 6:case 7:case 8:case 9:case 11:case 12:
	case 13: // 16��
		DrawGraph(mX, mY - mCameraY, Image::getIns()->getImage(Image_EnemyIcon, mType), TRUE);
		break;
	case 14:case 15:case 16:
	case 17: // 32��
		DrawGraph(mX, mY - mCameraY, Image::getIns()->getImage(Image_EnemyBigIcon, mType - 14), TRUE);
		break;
	case 18: // �{�X
		DrawGraph(mX, mY - mCameraY, Image::getIns()->getImage(Image_BossIcon), TRUE);
		break;
	case 19: // �A�C�e��
		DrawGraph(mX, mY - mCameraY, Image::getIns()->getImage(Image_SpecialFlag), TRUE);
		break;
	case 10: // �{�U���O����
		DrawGraph(mX - 12, mY - mCameraY, Image::getIns()->getImage(Image_EnemyIcon, 8), TRUE);
		DrawGraph(mX + 12, mY - mCameraY, Image::getIns()->getImage(Image_EnemyIcon, 8), TRUE);
		DrawGraph(mX, mY - 12 - mCameraY, Image::getIns()->getImage(Image_EnemyIcon, 8), TRUE);
		DrawGraph(mX, mY + 12 - mCameraY, Image::getIns()->getImage(Image_EnemyIcon, 8), TRUE);
		DrawGraph(mX, mY - mCameraY, Image::getIns()->getImage(Image_EnemyIcon, 10), TRUE);
		break;
	case 20: case 21: case 22: case 23: // �e��
		DrawGraph(mX, mY - mCameraY, Image::getIns()->getImage(Image_EnemyIcon, mType - 6), TRUE);
		break;
	}
	DrawGraph(mX, mY - mCameraY, Image::getIns()->getImage(Image_Number, mPtn), TRUE);

	// �f�o�b�O
	//DrawBox(mX, mY - mCameraY, mX + mW, mY + mH - mCameraY, GetColor(0, 255, 255), false);
}

/*------------------------------------------------------------------------------
�����蔻���Ԃ�
------------------------------------------------------------------------------*/
void SetNPC::setCameraY(int aY)
{
	mCameraY = aY;
}

/*------------------------------------------------------------------------------
�����蔻���Ԃ� aPos = 0 : x, 1 : y, 2 = w, 3 = h
------------------------------------------------------------------------------*/
int SetNPC::getCollisionPoint(int aPos) const
{
	switch (aPos) {
	case 0: // x
		return (int)mX;
	case 1: // y
		return (int)mY;
	case 2: // w
		return mW;
	case 3: // h
		return mH;
	}
	return -1;
}

/*------------------------------------------------------------------------------
�}�E�X���W���Z�b�g
------------------------------------------------------------------------------*/
void SetNPC::setMouse(int aX, int aY, int aL, int aR)
{
	mMouseX = aX;
	mMouseY = aY;
	mMouseCL = aL;
	mMouseCR = aR;
}

/*------------------------------------------------------------------------------
�A�N�e�B�u�t���O��Ԃ�
------------------------------------------------------------------------------*/
bool SetNPC::getActiveFlag() {
	return mActive;
}

/*------------------------------------------------------------------------------
�\���̃f�[�^��Ԃ�
------------------------------------------------------------------------------*/
int SetNPC::getData(int a)
{
	switch (a) {
	case 0:
		return mType;
	case 1:
		return mPtn;
	case 2:
		return mX;
	case 3:
		return mY + mH;
	}
	return -1;
}