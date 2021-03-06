#pragma once

//-------------------------------------------------------------------------------------------------
#include "Singleton.h"

//-------------------------------------------------------------------------------------------------
// 効果音の種類
enum eSE
{
	SE_Credit,
	SE_Zapper,
	SE_Blaster,
	SE_EnemyStrikenZapper,
	SE_EnemyStrikenBlaster,
	SE_BacuraHitZapper,
	SE_Miss,
	SE_Extend,
	SE_SpecialFlagGet,
	SE_ZakatoDethout,
	SE_GaruZakatoExplosion,
	SE_ShioniteDocking,
};

//-------------------------------------------------------------------------------------------------
// SEクラス
class SE final : public Singleton<SE>
{
public:
	SE();
	~SE();

	void initialize();

	void playSE(eSE aSE) const;
	void stopSE(eSE aSE) const;
	void setCanPlaySE(bool aCanPlay);

private:
	int mSE[12];
	bool mCanPlay;

};
