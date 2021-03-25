#pragma once

//-------------------------------------------------------------------------------------------------
#include "Game.h"

//-------------------------------------------------------------------------------------------------
class SystemMain
{
public:
	SystemMain();
	~SystemMain();

	bool initialize();
	bool setWindowSize();
	bool initializeDxLib();

	void loop();

private:
	Game mGame;
	int mWindowWidth;
	int mWindowHeight;
	int mScreenMagnification;

};
