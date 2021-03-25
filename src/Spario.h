#pragma once

#include "BaseNPC.h"

class Spario : public BaseNPC
{
public:
	Spario(InterfaceNPCEvent* aNPCEvent, NPCDATA aData, Player* aPlayer);
	~Spario();
	void update() override;
	void draw() const override;
	void areaChange() override;
	int getCollisionPoint(int aPos) const override;
	eTag getTag() const override;

private:
	float mSpeed;
	float mTheta;
	bool mInitFlg;

};