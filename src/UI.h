#pragma once

#include "SceneList.h"

class UI
{
public:
	UI(SceneList aNowScene = SceneList::None);
	~UI();
	void draw() const;
	void setHighScore(int aHighScore);
	void setScore(int aScore);
	void setLeft(int aLeft);
	void setArea(int aArea);
	void setDifficulty(int aDifficulty);
	void setSkyNPC(int aSkyNPC);
	void setGroundNPC(int aGroundNPC);

private:
	SceneList mNowScene;
	int mHighScore;
	int mScore;
	int mLeft;
	int mArea;
	int mDifficulty;
	int mSkyNPC;
	int mGroundNPC;

};