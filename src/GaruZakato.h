#pragma once

#include "BaseNPC.h"

class GaruZakato : public BaseNPC
{
public:
	GaruZakato(InterfaceNPCEvent* aNPCEvent, NPCDATA aData, Player* aPlayer);
	~GaruZakato();
	void update() override;
	void draw() const override;
	int getCollisionPoint(int aPos) const override;
	void collisionGameObject(eTag aTag) override;
	void areaChange() override;
	eTag getTag() const override;

private:
	// •Ï”
	int mTimer;
	int mBakuhatuTimer;

	// ŠÖ”
	void strikenEvent() override;
	void collision() override;

};