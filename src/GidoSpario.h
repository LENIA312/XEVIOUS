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
	// �ϐ�
	float mTheta;

	// �֐�
	void strikenEvent() override;
	void collision() override;

};