#pragma once

class InterfaceNPCEvent {
public:
	virtual ~InterfaceNPCEvent() {}
	virtual void addScore(int aScore) = 0;
	virtual void generateEnemyBullet(int aType, int aPtn, int aX, int aY) = 0;
	virtual void addLeft(int aLeft) = 0;

};