#pragma once

//-------------------------------------------------------------------------------------------------
#include "Singleton.h"

//-------------------------------------------------------------------------------------------------
// マウス制御クラス
class Mouse : public Singleton<Mouse> {
public:
	Mouse();

	void initialize(int aScreenMagnification);

	void update();
	void draw() const;

	bool getIsInScreen();
	int getXY(int aXY);
	int getWheel();
	int getClickLR(int aLR);

	void setAnime(int aAnime);

private:
	int mX;
	int mY;
	int mAnime;

	int mWheel;
	int mClickR;
	int mClickL;

	int mScreenMagnification;
	bool mIsInScreen;

};