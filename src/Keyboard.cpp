#include <DxLib.h>
#include "Keyboard.h"

//-------------------------------------------------------------------------------------------------
// �R���X�g���N�^
Keyboard::Keyboard()
{
}

//-------------------------------------------------------------------------------------------------
// ����������
void Keyboard::initialize()
{
	mPressingCount[0] = 0;
	mReleasingCount[0] = 0;
	for (int i = 0; i < 256; i++) {
		mPressingCount[i] = 0;
		mReleasingCount[i] = 0;
	}
}

//-------------------------------------------------------------------------------------------------
// �X�V
bool Keyboard::update() {
	char nowKeyStatus[KEY_NUM];
	//���̃L�[�̓��͏�Ԃ��擾
	GetHitKeyStateAll(nowKeyStatus);
	for (int i = 0; i<KEY_NUM; i++) {
		//i�Ԃ̃L�[��������Ă�����
		if (nowKeyStatus[i] != 0) {
			if (mReleasingCount[i] > 0) {
				// ������J�E���^��0���傫�����0�ɖ߂�
				mReleasingCount[i] = 0;
			}
			// ������J�E���^�𑝂₷
			mPressingCount[i]++;
		}
		//i�Ԃ̃L�[��������Ă�����
		else {
			if (mPressingCount[i] > 0) {
				// ������J�E���^��0���傫�����0�ɖ߂�
				mPressingCount[i] = 0;
			}
			//������J�E���^�𑝂₷
			mReleasingCount[i]++;
		}
	}
	return true;
}

//-------------------------------------------------------------------------------------------------
// keyCode�̃L�[��������Ă���t���[������Ԃ�
int Keyboard::getPressingCount(int aKeyCode) {
	if (!isAvailableCode(aKeyCode)) {
		return -1;
	}
	return mPressingCount[aKeyCode];
}

//-------------------------------------------------------------------------------------------------
// keyCode�̃L�[��������Ă���t���[������Ԃ�
int Keyboard::getReleasingCount(int aKeyCode) {
	if (!isAvailableCode(aKeyCode)) {
		return -1;
	}
	return mReleasingCount[aKeyCode];
}

//-------------------------------------------------------------------------------------------------
// keyCode���L���Ȓl���ۂ���Ԃ�
bool Keyboard::isAvailableCode(int aKeyCode) {
	if (!(0 <= aKeyCode && aKeyCode<KEY_NUM)) {
		return false;
	}
	return true;
}
