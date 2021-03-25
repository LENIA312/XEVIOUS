#pragma once

#include "BaseNPC.h"

class GaruDerota : public BaseNPC
{
public:
	GaruDerota(InterfaceNPCEvent* aNPCEvent, NPCDATA aData, Player* aPlayer);
	~GaruDerota();
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