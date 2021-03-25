#pragma once

#include "BaseNPC.h"

class Bozalogram : public BaseNPC
{
public:
	Bozalogram(InterfaceNPCEvent* aNPCEvent, NPCDATA aData, Player* aPlayer);
	~Bozalogram();
	void update() override;
	void draw() const override;
	void drawCollision(int r, int g, int b) const override;
	int getCollisionPoint(int aPos) const override;
	void collisionGameObject(eTag aTag) override;
	eTag getTag() const override;

private:
	// ïœêî
	bool mAtkFlg[4];
	int lAtkMode[4];
	int mSparioTimer[4];
	float mPartAnime[4];
	float mBrokenAnime[4];
	bool mBrokenFlg[4];

	// ä÷êî
	void strikenEvent() override;
	void collision() override;

};