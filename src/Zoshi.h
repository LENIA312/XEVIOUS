#pragma once

#include "BaseNPC.h"

class Zoshi : public BaseNPC
{
public:
	Zoshi(InterfaceNPCEvent* aNPCEvent, NPCDATA aData, Player* aPlayer);
	~Zoshi();
	void update() override;
	void draw() const override;
	int getCollisionPoint(int aPos) const override;
	void collisionGameObject(eTag aTag) override;
	void areaChange() override;
	eTag getTag() const override;

private:
	// �ϐ�
	int mAngleChangeTime;
	int mSparioTimer;
	bool mInitFlg;
	float mTheta;

	// �֐�
	void strikenEvent() override;
	void collision() override;

};