#pragma once

#include "BaseNPC.h"

class Terrazi : public BaseNPC
{
public:
	Terrazi(InterfaceNPCEvent* aNPCEvent, NPCDATA aData, Player* aPlayer);
	~Terrazi();
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
	bool mShotFlg;

	// ä÷êî
	void init();
	void strikenEvent() override;
	void collision() override;

};