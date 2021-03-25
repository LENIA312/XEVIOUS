#pragma once

#include "BaseNPC.h"

class Barra : public BaseNPC
{
public:
	Barra(InterfaceNPCEvent* aNPCEvent, NPCDATA aData, Player* aPlayer);
	~Barra();
	void update() override;
	void draw() const override;
	int getCollisionPoint(int aPos) const override;
	void collisionGameObject(eTag aTag) override;
	eTag getTag() const override;

private:
	void strikenEvent() override;
	void collision() override;

};