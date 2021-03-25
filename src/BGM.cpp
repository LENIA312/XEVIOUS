#pragma warning(disable:26812)

//-------------------------------------------------------------------------------------------------
#include "BGM.h"

//-------------------------------------------------------------------------------------------------
#include <DxLib.h>

//-------------------------------------------------------------------------------------------------
// コンストラクタ
BGM::BGM()
	: mBGM()
{
}

//-------------------------------------------------------------------------------------------------
// デストラクタ
BGM::~BGM()
{
	// 今までロードしたBGMを解放する
	for (int& element : mBGM) {
		DeleteSoundMem(element);
	}
}

//-------------------------------------------------------------------------------------------------
// 初期化処理
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
//BGMを再生
void BGM::playBGM(eBGM aBGM, bool aLoopFlag, bool aTopPositionFlag) const
{
	// ループ再生
	if (aLoopFlag) {
		// 先頭から再生
		if (aTopPositionFlag) {
			PlaySoundMem(mBGM[aBGM], DX_PLAYTYPE_LOOP, TRUE);
		}
		// 途中から再生
		else {
			PlaySoundMem(mBGM[aBGM], DX_PLAYTYPE_LOOP, FALSE);
		}

	}
	// 一回だけ再生
	else {
		// 先頭から再生
		if (aTopPositionFlag) {
			PlaySoundMem(mBGM[aBGM], DX_PLAYTYPE_BACK, TRUE);
		}
		// 途中から再生
		else {
			PlaySoundMem(mBGM[aBGM], DX_PLAYTYPE_BACK, FALSE);
		}
	}
}

//-------------------------------------------------------------------------------------------------
// 指定のBGMを停止
void BGM::stopBGM(eBGM aBGM) const
{
	StopSoundMem(mBGM[aBGM]);
}

//-------------------------------------------------------------------------------------------------
// BGMのボリューム変更
void BGM::volume(eBGM aBGM, int aVolume)
{
	ChangeVolumeSoundMem(aVolume, mBGM[aBGM]);
}

//-------------------------------------------------------------------------------------------------
// 特定のBGMが流れているかチェック
int BGM::checkIsPlay(eBGM aBGM)
{
	return CheckSoundMem(mBGM[aBGM]);
}