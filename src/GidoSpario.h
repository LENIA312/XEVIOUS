#pragma once

#include "BaseNPC.h"

class GidoSpario : public BaseNPC
{
public:
	GidoSpario(InterfaceNPCEvent* aNPCEvent, NPCDATA aData, Player* aPlayer);
	~GidoSpario();
	void update() override;
	void draw() const override;
	int getCollisionPoint(int aPos) const override;
	void collisionGameObject(eTag aTag) override;
	eTag getTag() const override;

private:
	// ïœêî
	float mTheta;

	// ä÷êî
	void strikenEvent() override;
	void collision() override;

};