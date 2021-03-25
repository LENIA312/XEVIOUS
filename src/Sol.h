#pragma once

#include "BaseNPC.h"

class Sol : public BaseNPC
{
public:
	Sol(InterfaceNPCEvent* aNPCEvent, NPCDATA aData, Player* aPlayer);
	~Sol();
	void update() override;
	void draw() const override;
	int getCollisionPoint(int aPos) const override;
	void collisionGameObject(eTag aTag) override;
	eTag getTag() const override;

private:
	// ïœêî
	int hitCnt;

	// ä÷êî
	bool appearFlg;
	bool strikeOn;
	void strikenEvent() override;
	void collision();

};