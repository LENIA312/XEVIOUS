#pragma once

//-------------------------------------------------------------------------------------------------
#include "SceneList.h"

//-------------------------------------------------------------------------------------------------
// �V�[�����L���J�G���C���^�[�t�F�[�X�N���X
class InterfaceSceneChanger
{
public:
	virtual ~InterfaceSceneChanger() {}
	virtual void changeScene(SceneList aNextScene) = 0;

};