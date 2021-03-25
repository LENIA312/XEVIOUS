#pragma warning(disable:26812)

#include <DxLib.h>
#include "Camera.h"
#include "Define.h"
#include "Mouse.h"
#include "Pad.h"

/*------------------------------------------------------------------------------
�R���X�g���N�^
------------------------------------------------------------------------------*/
Camera::Camera()
{
	mMode = 0;
	mY = (float)Define::MapHeight + (float)Define::ForestHeight - (float)Define::GameScreenHeight;
}

/*------------------------------------------------------------------------------
�f�X�g���N�^
------------------------------------------------------------------------------*/
Camera::~Camera()
{
}

/*------------------------------------------------------------------------------
�X�V
------------------------------------------------------------------------------*/
void Camera::update()
{
	switch (mMode) {
	case 0: // �Q�[��
		mY -= Define::ScrollSpeed;
		if (mY < 0) {
			mY += Define::MapHeight;
		}
		break;

	case 1: // �G�f�B�^�[
		// �z�C�[��
		if (Mouse::getIns()->getWheel() > 0) {
			mY -= 8;
		}
		if (Mouse::getIns()->getWheel() < 0) {
			mY += 8;
		}
		if (Pad::getIns()->get(Pad_Up)) {
			mY -= 32;
		}
		if (Pad::getIns()->get(Pad_Down)) {
			mY += 32;
		}
		// �}�b�v���ɃJ������}����
		if (mY < 0) {
			mY = 0;
		}
		if (mY > Define::MapHeight - Define::GameScreenHeight + 6) {
			mY = (float)Define::MapHeight - (float)Define::GameScreenHeight + 6;
		}
		break;
	}
}

/*------------------------------------------------------------------------------
�`��
------------------------------------------------------------------------------*/
void Camera::draw() const
{
	DrawFormatString(0, 0, GetColor(255, 255, 255), "\n\nCameraY:%0.1f", mY);
}

/*------------------------------------------------------------------------------
�J������y��Ԃ�
------------------------------------------------------------------------------*/
float Camera::getY() const
{
	return mY;
}

/*------------------------------------------------------------------------------
���Z�b�g
------------------------------------------------------------------------------*/
void Camera::reset()
{
	mY = (float)Define::MapHeight + (float)Define::ForestHeight - (float)Define::GameScreenHeight;
}

/*------------------------------------------------------------------------------
���[�h�Z�b�g
------------------------------------------------------------------------------*/
void Camera::setMode(int aMode)
{
	mMode = aMode;
	if (aMode == 1) {
		int tmp = (int)mY / 8 * 8;
  		mY = (float)tmp;
	}
}
