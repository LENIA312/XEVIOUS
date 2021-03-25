#pragma once

#include "BaseNPC.h"

class Grobda : public BaseNPC
{
public:
	Grobda(InterfaceNPCEvent* aNPCEvent, NPCDATA aData, Player* aPlayer);
	~Grobda();
	void update() override;
	void draw() const override;
	int getCollisionPoint(int aPos) const override;
	void collisionGameObject(eTag aTag) override;
	eTag getTag() const override;

private:
	void strikenEvent() override;
	void collision() override;

};