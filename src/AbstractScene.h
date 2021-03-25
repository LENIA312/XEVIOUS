#pragma once

//-------------------------------------------------------------------------------------------------
#include "InterfaceSceneChanger.h"

//-------------------------------------------------------------------------------------------------
// シーンの抽象クラス
class AbstractScene
{
public:
	AbstractScene(InterfaceSceneChanger* aChanger) {
		mSceneChanger = aChanger;
	}
	virtual ~AbstractScene() {}
	virtual void update() = 0;
	virtual void draw() const = 0;

protected:
	InterfaceSceneChanger* mSceneChanger;

};