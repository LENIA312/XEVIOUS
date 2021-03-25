#pragma once

#include "BaseNPC.h"
#include "Player.h"

class AddorGuileness : public BaseNPC
{
public:
	AddorGuileness(InterfaceNPCEvent* aNPCEvent, NPCDATA aData, Player* aPlayer);
	~AddorGuileness();
	void update() override;
	void draw() const override;
	void drawCollision(int r, int g, int b) const override;
	void areaChange() override;
	int getCollisionPoint(int aPos) const override;
	void collisionGameObject(eTag aTag) override;
	eTag getTag() const override;

private:
	// �ϐ�
	int mSayonaraTimer;
	int mSparioTimer[4];
	float mEscapeY;
	float mEscapeAnime;
	float mBrokenAnime[4];
	bool mBrokenFlg[4];

	// �I�u�W�F�N�g
	Player* mPlayer;

	// �֐�
	void strikenEvent() override;
	void collision() override;

};