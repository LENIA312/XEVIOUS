#pragma once

#include "BaseNPC.h"

class Domogram : public BaseNPC
{
public:
	Domogram(InterfaceNPCEvent* aNPCEvent, NPCDATA aData, Player* aPlayer);
	~Domogram();
	void update() override;
	void draw() const override;
	int getCollisionPoint(int aPos) const override;
	void collisionGameObject(eTag aTag) override;
	eTag getTag() const override;

private:
	// ïœêî
	int  mCnt;
	int mAttackMode;
	int mSparioTimer;

	// ä÷êî
	void strikenEvent() override;
	void collision() override;

};