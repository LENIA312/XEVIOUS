#pragma once

//-------------------------------------------------------------------------------------------------
#include <list>
#include <memory>
#include "AbstractScene.h"
#include "Camera.h"
#include "Background.h"
#include "NPCDATA.h"
#include "EditorUI.h"
#include "SetNPC.h"
#include "Player.h"
#include "NPCManager.h"
#include "UI.h"

//-------------------------------------------------------------------------------------------------
// エディター画面
class EditorScene : public AbstractScene
{
public:
	EditorScene(InterfaceSceneChanger* aChanger);
	~EditorScene();

	void update() override;
	void draw() const override;

private:
	// 関数
	void setNPC();
	void editorButton();
	void readNPCFile();
	void saveNPCFile();

	// リスト
	std::list<std::shared_ptr<SetNPC>> mSetNPCList[5][16];

	// オブジェクト
	Camera* mCamera;
	Background mBackground;
	EditorUI* mEditorUI;
	Player* mPlayer;
	NPCManager* mNPCMgr;
	UI* mUI;

	// 変数
	int mMouseX;
	int mMouseY;
	int mNowArea;
	int mDifficulty;
	int mEditorButtonState[3];
	bool mOkemasukaFlag;
	bool mSugunihaMuriFlag;
	bool mSavedFlag;
	bool mIsTestPlay;

	int mEnemyNum;
	
};
