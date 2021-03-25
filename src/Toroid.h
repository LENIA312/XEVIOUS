#pragma once

#include "BaseNPC.h"

class Toroid : public BaseNPC
{
public:
	Toroid(InterfaceNPCEvent* aNPCEvent, NPCDATA aData, Player* aPlayer);
	~Toroid();
	void update() override;
	void draw() const override;
	int getCollisionPoint(int aPos) const override;
	void collisionGameObject(eTag aTag) override;
	eTag getTag() const override;

private:
	// �ϐ�
	float mSpeedX;
	float mSpeedY;
	bool mLRFlg;
	bool mAcceleFlg;

	// �֐�
	void init();
	void strikenEvent() override;
	void collision() override;

};