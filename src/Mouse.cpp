#include "Mouse.h"

//-------------------------------------------------------------------------------------------------
#include <DxLib.h>
#include "Define.h"
#include "Image.h"

//-------------------------------------------------------------------------------------------------
// �R���X�g���N�^
Mouse::Mouse()
	: mX(0)
	, mY(0)
	, mAnime(0)
	, mWheel(0)
	, mClickL(0)
	, mClickR(0)
	, mScreenMagnification(0)
	, mIsInScreen(false)
{
}

//-------------------------------------------------------------------------------------------------
// ����������
void Mouse::initialize(int aScreenMagnification)
{
	mScreenMagnification = aScreenMagnification;
}

//-------------------------------------------------------------------------------------------------
// �X�V
void Mouse::update()
{
	// �}�E�X�̍��W�擾
	GetMousePoint(&mX, &mY);
	mX /= mScreenMagnification;
	mY /= mScreenMagnification;

	// �N���b�N�擾
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
		mClickL++;
	} else {
		mClickL = 0;
	}
	if ((GetMouseInput() & MOUSE_INPUT_RIGHT) != 0) {
		mClickR++;
	} else {
		mClickR = 0;
	}

	// �z�C�[���擾
	mWheel = GetMouseWheelRotVol();

	// ��ʓ��Ƀ}�E�X�����݂��邩�ǂ���
	if (mX >= 0 && mX <= Define::GameScreenWidth && mY >= 0 && mY <= Define::GameScreenHeight) {
		mIsInScreen = true;
	} else {
		mIsInScreen = false;
	}

	// �A�j���ʒu������
	mAnime = 0;
}

//-------------------------------------------------------------------------------------------------
// �`��
void Mouse::draw() const
{
	if (mIsInScreen) {
		DrawGraph(mX, mY, Image::getIns()->getImage(Image_Mouse, mAnime), true);
	}
}

//-------------------------------------------------------------------------------------------------
// ���W��Ԃ�
int Mouse::getXY(int aXY)
{
	switch (aXY) {
	case 0:
		return mX;
	case 1:
		return mY;
	}
	return -1;
}

//-------------------------------------------------------------------------------------------------
// �N���b�N��Ԃ�Ԃ�
int Mouse::getClickLR(int aLR)
{
	switch (aLR) {
	case 0:
		return mClickL;
	case 1:
		return mClickR;
	}
	return -1;
}

//-------------------------------------------------------------------------------------------------
// �z�C�[�����l��Ԃ�
int Mouse::getWheel()
{
	return mWheel;
}

//-------------------------------------------------------------------------------------------------
// ��ʓ��ɂ��邩�̃t���O��Ԃ�
bool Mouse::getIsInScreen()
{
	return mIsInScreen;
}

//-------------------------------------------------------------------------------------------------
// �A�j���ʒu���Z�b�g
void Mouse::setAnime(int aAnime)
{
	mAnime = aAnime;
}