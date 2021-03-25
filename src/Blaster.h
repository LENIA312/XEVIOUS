#pragma once

#include "AbstractGameObject.h"

class Blaster : public AbstractGameObject
{
public:
	Blaster();
	~Blaster();
	void update() override;
	void draw() const override;
	void collisionGameObject(eTag aTag) override;
	int getCollisionPoint(int aPos) const override;
	eTag getTag() const;
	void generate(int aX, int aY);
	bool getHitFlag();

private:
	float mBlasterSpeed;
	float mTargetY;

	bool mIsHit;

};