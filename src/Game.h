#pragma once

//-------------------------------------------------------------------------------------------------
#include "Fps.h"
#include "SceneManager.h"

//-------------------------------------------------------------------------------------------------
// ƒQ[ƒ€‘S‘Ì
class Game
{
public:
	Game();

	bool mainLoop();

private:
	Fps mFps;
	SceneManager mSceneManager;

};