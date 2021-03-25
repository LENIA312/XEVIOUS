#pragma once

#include "Camera.h"

class SetNPC
{
public:
	SetNPC(int mType, int aPtn, int mX, int mY);
	~SetNPC();
	void update();
	void draw() const;
	void setCameraY(int aY);
	int getCollisionPoint(int aPos) const;
	void setMouse(int aX, int aY, int aL, int aR);
	bool getActiveFlag();
	int getData(int a);

private:
	int mX;
	int mY;
	int mW;
	int mH;
	int mPtn;
	int mType;
	bool mActive;
	int mCameraY;
	int mMouseX;
	int mMouseY;
	int mMouseCL;
	int mMouseCR;

};