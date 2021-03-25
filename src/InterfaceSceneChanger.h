#pragma once

//-------------------------------------------------------------------------------------------------
#include "SceneList.h"

//-------------------------------------------------------------------------------------------------
// シーンヲキリカエルインターフェースクラス
class InterfaceSceneChanger
{
public:
	virtual ~InterfaceSceneChanger() {}
	virtual void changeScene(SceneList aNextScene) = 0;

};