#pragma once

//-------------------------------------------------------------------------------------------------
#include "Singleton.h"

//-------------------------------------------------------------------------------------------------
// BGM���X�g
enum eBGM
{
	BGM_Start,
	BGM_Game,
	BGM_NameEntry_01,
	BGM_NameEntry_02,
	BGM_Boss,
	BGM_Editer_01,
	BGM_Editer_02,
};

//-------------------------------------------------------------------------------------------------
// BGM�N���X
class BGM final : public Singleton<BGM>
{
public:
	BGM();
	~BGM();

	void initialize();

	void playBGM(eBGM aBGM, bool, bool) const;
	void stopBGM(eBGM aBGM) const;
	void volume(eBGM aBGM, int aVolume);
	int checkIsPlay(eBGM aBGM);

private:
	int mBGM[7];

};