#pragma once

#include <list>
#include <memory>

#include "InterfaceNPCEvent.h"
#include "BaseNPC.h"
#include "Player.h"
#include "NPCDATA.h"

class NPCManager : public InterfaceNPCEvent
{
public:
	NPCManager(Player* aPlayer);
	~NPCManager();
	void update();
	void draw() const;
	void generateEnemyBullet(int aType, int aPtn, int aX, int aY) override;
	void readNPCFile();
	void clearList();
	void setCamera(float aY);
	void setNowArea(int aNowArea);
	void setDifficulty(int aRank);
	void addScore(int aScore) override;
	void addLeft(int aLeft) override;
	int getScore() const;
	void testPlayModeOn();
	int getNPCListSize(int a) const;
	void resetScore();
	int getAddLeft() const;
	void resetAddLeft();

private:
	// �ϐ�
	int mDifficulty;
	int mNowArea;
	int mTortalScore;
	int mAddLeft;
	int mColor;
	int mColorTimer;
	int mNPCDataMax[5][16];
	bool mIsTestPlay;
	NPCDATA mNPCData[5][16][256];

	float mCameraY;

	// ���X�g
	std::list<std::shared_ptr<BaseNPC>> mNPCList[2];

	// �I�u�W�F�N�g
	Player* mPlayer;
	Zapper* mZapper;
	Blaster* mBlaster;

	// �֐�
	void drawCollision() const;
	void collisionPlayer();
	void generateNPC();
	void colorAnime();
	
};