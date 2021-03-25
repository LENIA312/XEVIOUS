#include "Fps.h"

//-------------------------------------------------------------------------------------------------
#include <DxLib.h>
#include <math.h>

//-------------------------------------------------------------------------------------------------
// �R���X�g���N�^
Fps::Fps()
{
	mStartTime = 0;
	mCount = 0;
	mFps = 60;
}

//-------------------------------------------------------------------------------------------------
// �X�V
bool Fps::update()
{
	// 1�t���[���ڂȂ玞�����L��
	if (mCount == 0) {
		mStartTime = GetNowCount();
	}
	// 60�t���[���ڂȂ畽�ς��v�Z����
	if (mCount == N) {
		int t = GetNowCount();
		mFps = 1000.f / ((t - mStartTime) / (float)N);
		mCount = 0;
		mStartTime = t;
	}
	mCount++;
	return true;
}

//-------------------------------------------------------------------------------------------------
// ���݂�FPS�̕\��
void Fps::draw()
{
	DrawFormatString(0, 0, GetColor(0, 255, 0), "%2.0ffps", mFps);
}

//-------------------------------------------------------------------------------------------------
// 60FPS�𒴂��Ă�����҂�
void Fps::wait()
{
	// ������������
	int tookTime = GetNowCount() - mStartTime;
	// �҂ׂ�����
	int waitTime = mCount * 1000 / FPS - tookTime;
	// �ҋ@
	if (waitTime > 0) {
		Sleep(waitTime);
	}
}