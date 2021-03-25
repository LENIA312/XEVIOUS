#pragma once

#include "BaseNPC.h"

class Kapi : public BaseNPC
{
public:
	Kapi(InterfaceNPCEvent* aNPCEvent, NPCDATA aData, Player* aPlayer);
	~Kapi();
	void update() override;
	void draw() const override;
	int getCollisionPoint(int aPos) const override;
	void collisionGameObject(eTag aTag) override;
	eTag getTag() const override;

private:
	// ïœêî
	int mTimer;
	int mEscapeTimer;
	int mSparioTimer;
	float mSpeedX;
	float mSpeedY;
	bool mLRFlg;
	bool mAcceleFlg;

	// ä÷êî
	void init();
	void strikenEvent() override;
	void collision() override;

};