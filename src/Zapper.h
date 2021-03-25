#pragma once

#include "AbstractGameObject.h"

class Zapper : public AbstractGameObject
{
public:
	Zapper();
	~Zapper();
	void update() override;
	void draw() const override;
	void collisionGameObject(eTag aTag) override;
	int getCollisionPoint(int aPos) const override;
	eTag getTag() const;
	void generate(int aX, int aY);

};