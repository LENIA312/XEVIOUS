#pragma warning(disable:26812)

//-------------------------------------------------------------------------------------------------
#include "SE.h"

//-------------------------------------------------------------------------------------------------
#include <DxLib.h>

//-------------------------------------------------------------------------------------------------
// コンストラクタ
SE::SE()
	: mSE()
	, mCanPlay(true)
{
}

//-------------------------------------------------------------------------------------------------
// デストラクタ
SE::~SE()
{
	// 今までロードしたSEを解放する
	for (int& element : mSE) {
		DeleteSoundMem(element);
	}
}

//-------------------------------------------------------------------------------------------------
// 読み込み
void SE::initialize()
{
	mSE[SE_Credit] = LoadSoundMem("res/se/credit.wav");
	mSE[SE_Zapper] = LoadSoundMem("res/se/zapper.wav");
	mSE[SE_Blaster] = LoadSoundMem("res/se/blaster.wav");
	mSE[SE_EnemyStrikenZapper] = LoadSoundMem("res/se/enemy_striken_zapper.wav");
	mSE[SE_EnemyStrikenBlaster] = LoadSoundMem("res/se/enemy_striken_blaster.wav");
	mSE[SE_BacuraHitZapper] = LoadSoundMem("res/se/bacura_hit_zapper.wav");
	mSE[SE_Miss] = LoadSoundMem("res/se/miss.wav");
	mSE[SE_Extend] = LoadSoundMem("res/se/extend.wav");
	mSE[SE_SpecialFlagGet] = LoadSoundMem("res/se/special_flag_get.wav");
	mSE[SE_ZakatoDethout] = LoadSoundMem("res/se/zakato_dethout.wav");
	mSE[SE_GaruZakatoExplosion] = LoadSoundMem("res/se/garu_zakato_explosion.wav");
	mSE[SE_ShioniteDocking] = LoadSoundMem("res/se/shionite_docking.wav");
}

//-------------------------------------------------------------------------------------------------
// 再生
void SE::playSE(eSE se) const
{
	if (mCanPlay) {
		PlaySoundMem(mSE[se], DX_PLAYTYPE_BACK, TRUE);
	}
}

//-------------------------------------------------------------------------------------------------
// ストップ
void SE::stopSE(eSE se) const
{
	StopSoundMem(mSE[se]);
}

//-------------------------------------------------------------------------------------------------
// フラグをセット　※trueだと通常、falseだと音が出なくなる
void SE::setCanPlaySE(bool aCanPlay)
{
	mCanPlay = aCanPlay;
}