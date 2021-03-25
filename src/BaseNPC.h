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
	// �ϐ�
	int mPtn;
	int mColor;
	bool mIsStriken;
	float mStrikenAnime;

	// �I�u�W�F�N�g
	Player* mPlayer;
	InterfaceNPCEvent* mNPCEvent;

	// �֐�
	virtual void collision() {}
	virtual void strikenEvent() {}

};