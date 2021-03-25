#pragma once

#include "BaseNPC.h"

class SpecialFlag : public BaseNPC
{
public:
	SpecialFlag(InterfaceNPCEvent* aNPCEvent, NPCDATA aNPC_DATA, Player* aPlayer);
	~SpecialFlag();
	void update() override;
	void draw() const override;
	int getCollisionPoint(int aPos) const override;
	void collisionGameObject(eTag aTag) override;
	eTag getTag() const override;

private:
	// ïœêî
	float mSpeedX;
	float mSpeedY;
	bool mLRFlg;
	bool mAcceleFlg;
	bool mGetOn;
	bool mGetFlg;

	// ä÷êî
	void init();
	void strikenEvent() override;
	void collision() override;

};