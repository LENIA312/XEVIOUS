#pragma once

//-------------------------------------------------------------------------------------------------
#include "Fps.h"
#include "SceneManager.h"

//-------------------------------------------------------------------------------------------------
// �Q�[���S��
class Game
{
public:
	Game();

	bool mainLoop();

private:
	Fps mFps;
	SceneManager mSceneManager;

};