#include <DxLib.h>
#include "SceneManager.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "EditorScene.h"

/*------------------------------------------------------------------------------
�R���X�g���N�^
------------------------------------------------------------------------------*/
SceneManager::SceneManager()
{
	mNextScene = SceneList::None;
	mNowScene  = SceneList::None;
	mScene = new TitleScene(this);
}

/*------------------------------------------------------------------------------
�f�X�g���N�^
------------------------------------------------------------------------------*/
SceneManager::~SceneManager()
{
	delete mScene;
}

/*------------------------------------------------------------------------------
�X�V
------------------------------------------------------------------------------*/
void SceneManager::update() {
	// ���̃V�[�����Z�b�g����Ă�����
	if (mNextScene != SceneList::None) {
		delete mScene;
		switch (mNextScene) {
		case SceneList::Title:
			mScene = new TitleScene(this);
			break;
		case SceneList::Game:
			mScene = new GameScene(this);
			break;
		case SceneList::Editor:
			mScene = new EditorScene(this);
			break;
		}
		mNowScene = mNextScene;
		mNextScene = SceneList::None;
	}
	mScene->update(); //�V�[���̍X�V
}

/*------------------------------------------------------------------------------
�`��
------------------------------------------------------------------------------*/
void SceneManager::draw() {
	mScene->draw();
}

/*------------------------------------------------------------------------------
�V�[����ύX����
------------------------------------------------------------------------------*/
void SceneManager::changeScene(SceneList aNextScene) {
	mNextScene = aNextScene;
}

/*------------------------------------------------------------------------------
���݂̃V�[����Ԃ�
------------------------------------------------------------------------------*/
SceneList SceneManager::getNowScene()
{
	return mNowScene;
}