#pragma once

#include "BaseNPC.h"

class Zakato : public BaseNPC
{
public:
	Zakato(InterfaceNPCEvent* aNPCEvent, NPCDATA aData, Player* aPlayer);
	~Zakato();
	void update() override;
	void draw() const override;
	int getCollisionPoint(int aPos) const override;
	void collisionGameObject(eTag aTag) override;
	eTag getTag() const override;

private:
	// �ϐ�
	int mTimer;
	int mBakuhatuTimer;
	float mTheta;
	bool mBakuhatuFlg;

	// �֐�
	void strikenEvent() override;
	void collision() override;

};