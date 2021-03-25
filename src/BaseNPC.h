#pragma once

#include "AbstractGameObject.h"
#include "InterfaceNPCEvent.h"
#include "NPCDATA.h"
#include "Player.h"

class BaseNPC : public AbstractGameObject
{
public:
	BaseNPC(InterfaceNPCEvent* aNPCEvent);
	~BaseNPC();
	void setColor(int aColor);
	bool getIsStriken();
	virtual void areaChange();

protected:
	// 変数
	int mPtn;
	int mColor;
	bool mIsStriken;
	float mStrikenAnime;

	// オブジェクト
	Player* mPlayer;
	InterfaceNPCEvent* mNPCEvent;

	// 関数
	virtual void collision() {}
	virtual void strikenEvent() {}

};