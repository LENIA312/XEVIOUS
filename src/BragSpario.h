#pragma once

#include "BaseNPC.h"

class BragSpario : public BaseNPC
{
public:
	BragSpario(InterfaceNPCEvent* aNPCEvent, NPCDATA aData, Player* aPlayer);
	~BragSpario();
	void update() override;
	void draw() const override;
	void areaChange() override;
	int getCollisionPoint(int aPos) const override;
	void collisionGameObject(eTag aTag) override;
	eTag getTag() const override;

private:
	int mTimer;
	float mSpeed;
	float mTheta;
	bool mInitFlg;

	// ä÷êî
	void collision() override;
};