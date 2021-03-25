#pragma once

#include "BaseNPC.h"

class Bacura : public BaseNPC
{
public:
	Bacura(InterfaceNPCEvent* aNPCEvent, NPCDATA aData, Player* aPlayer);
	~Bacura();
	void update() override;
	void draw() const override;
	int getCollisionPoint(int aPos) const override;
	void collisionGameObject(eTag aTag) override;
	eTag getTag() const override;

private:
	// ïœêî
	int mEfectNum;
	int* mEfectX;
	int* mEfectShift;
	bool* mEfectFlg;
	float* mEfectAnime;

	// ä÷êî
	void collision() override;

};