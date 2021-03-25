#pragma once

#include "BaseNPC.h"

class Torken : public BaseNPC
{
public:
	Torken(InterfaceNPCEvent* aNPCEvent, NPCDATA aData, Player* aPlayer);
	~Torken();
	void update() override;
	void draw() const override;
	int getCollisionPoint(int aPos) const override;
	void collisionGameObject(eTag aTag) override;
	eTag getTag() const override;

private:

	float mSpeed;
	float mSpeedR;

	bool mAcceleFlg;

	float mTheta;
	float mVx, mVy;
	bool mMoveFlg;
	bool mWaitFlg;

	int mTt;
	int mStopTime;
	int mTimer;
	// ä÷êî
	void init();
	void strikenEvent() override;
	void collision() override;

};