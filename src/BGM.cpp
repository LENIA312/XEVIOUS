#pragma warning(disable:26812)

//-------------------------------------------------------------------------------------------------
#include "BGM.h"

//-------------------------------------------------------------------------------------------------
#include <DxLib.h>

//-------------------------------------------------------------------------------------------------
// �R���X�g���N�^
BGM::BGM()
	: mBGM()
{
}

//-------------------------------------------------------------------------------------------------
// �f�X�g���N�^
BGM::~BGM()
{
	// ���܂Ń��[�h����BGM���������
	for (int& element : mBGM) {
		DeleteSoundMem(element);
	}
}

//-------------------------------------------------------------------------------------------------
// ����������
void BGM::initialize()
{
	mBGM[BGM_Start] = LoadSoundMem("res/bgm/xev_start.wav");
	mBGM[BGM_Game] = LoadSoundMem("res/bgm/xev_game.wav");
	mBGM[BGM_NameEntry_01] = LoadSoundMem("res/bgm/xev_name_entry_01.wav");
	mBGM[BGM_NameEntry_02] = LoadSoundMem("res/bgm/xev_name_entry_02.wav");
	mBGM[BGM_Boss] = LoadSoundMem("res/bgm/xev_boss.wav");
	mBGM[BGM_Editer_01] = LoadSoundMem("res/bgm/xevious_maker_a.wav");
	mBGM[BGM_Editer_02] = LoadSoundMem("res/bgm/xevious_maker_b.wav");
}

//-------------------------------------------------------------------------------------------------
//BGM���Đ�
void BGM::playBGM(eBGM aBGM, bool aLoopFlag, bool aTopPositionFlag) const
{
	// ���[�v�Đ�
	if (aLoopFlag) {
		// �擪����Đ�
		if (aTopPositionFlag) {
			PlaySoundMem(mBGM[aBGM], DX_PLAYTYPE_LOOP, TRUE);
		}
		// �r������Đ�
		else {
			PlaySoundMem(mBGM[aBGM], DX_PLAYTYPE_LOOP, FALSE);
		}

	}
	// ��񂾂��Đ�
	else {
		// �擪����Đ�
		if (aTopPositionFlag) {
			PlaySoundMem(mBGM[aBGM], DX_PLAYTYPE_BACK, TRUE);
		}
		// �r������Đ�
		else {
			PlaySoundMem(mBGM[aBGM], DX_PLAYTYPE_BACK, FALSE);
		}
	}
}

//-------------------------------------------------------------------------------------------------
// �w���BGM���~
void BGM::stopBGM(eBGM aBGM) const
{
	StopSoundMem(mBGM[aBGM]);
}

//-------------------------------------------------------------------------------------------------
// BGM�̃{�����[���ύX
void BGM::volume(eBGM aBGM, int aVolume)
{
	ChangeVolumeSoundMem(aVolume, mBGM[aBGM]);
}

//-------------------------------------------------------------------------------------------------
// �����BGM������Ă��邩�`�F�b�N
int BGM::checkIsPlay(eBGM aBGM)
{
	return CheckSoundMem(mBGM[aBGM]);
}