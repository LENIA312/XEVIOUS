#pragma once

#include "BaseNPC.h"

class Derota : public BaseNPC
{
public:
	Derota(InterfaceNPCEvent* aNPCEvent, NPCDATA aData, Player* aPlayer);
	~Derota();
	void update() override;
	void draw() const override;
	int getCollisionPoint(int aPos) const override;
	void collisionGameObject(eTag aTag) override;
	eTag getTag() const override;

private:
	// •Ï”
	int mTimer;

	// ŠÖ”
	void strikenEvent() override;
	void collision() override;

};