#include <DxLib.h>
#include "SceneManager.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "EditorScene.h"

/*------------------------------------------------------------------------------
コンストラクタ
------------------------------------------------------------------------------*/
SceneManager::SceneManager()
{
	mNextScene = SceneList::None;
	mNowScene  = SceneList::None;
	mScene = new TitleScene(this);
}

/*------------------------------------------------------------------------------
デストラクタ
------------------------------------------------------------------------------*/
SceneManager::~SceneManager()
{
	delete mScene;
}

/*------------------------------------------------------------------------------
更新
------------------------------------------------------------------------------*/
void SceneManager::update() {
	// 次のシーンがセットされていたら
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
	mScene->update(); //シーンの更新
}

/*------------------------------------------------------------------------------
描画
------------------------------------------------------------------------------*/
void SceneManager::draw() {
	mScene->draw();
}

/*------------------------------------------------------------------------------
シーンを変更する
------------------------------------------------------------------------------*/
void SceneManager::changeScene(SceneList aNextScene) {
	mNextScene = aNextScene;
}

/*------------------------------------------------------------------------------
現在のシーンを返す
------------------------------------------------------------------------------*/
SceneList SceneManager::getNowScene()
{
	return mNowScene;
}