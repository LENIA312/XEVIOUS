#pragma once

#include "AbstractGameObject.h"

#include "Zapper.h"
#include "Blaster.h"

class Player : public AbstractGameObject
{
public:
	Player(int aX = 168, int aY = 224);
	~Player();
	void update() override;
	void draw() const override;
	void drawCollision(int r, int g, int b) const override;
	void collisionGameObject(eTag aTag) override;
	int getCollisionPoint(int aPos) const override;
	Zapper* getZapper() const;
	Blaster* getBlaster() const;
	eTag getTag() const;
	void reset();
	void setTikaTikaFlg(bool aFlg);
	int getAimngXY(int xy);

private:
	// �ϐ�
	int mBlasterTargetY;
	float mRpidZapperTimer;
	float mAimingAnime;
	bool mTikaTikaFlg;

	// �I�u�W�F�N�g
	Zapper* mZapper;
	Blaster* mBlaster;

	// �֐�
	void move();
	void zapper();
	void blaster();

};