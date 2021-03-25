#pragma once

#include "BaseNPC.h"

class Zolbak : public BaseNPC
{
public:
	Zolbak(InterfaceNPCEvent* aNPCEvent, NPCDATA aData, Player* aPlayer);
	~Zolbak();
	void update() override;
	void draw() const override;
	int getCollisionPoint(int aPos) const override;
	void collisionGameObject(eTag aTag) override;
	eTag getTag() const override;

private:
	void strikenEvent() override;
	void collision() override;

};