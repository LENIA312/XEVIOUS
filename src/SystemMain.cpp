#include "SystemMain.h"

//-------------------------------------------------------------------------------------------------
#include <DxLib.h>
#include "Define.h"
#include "Icon.h"
#include "Image.h"
#include "BGM.h"
#include "SE.h"
#include "Keyboard.h"
#include "Pad.h"
#include "Mouse.h"
#include "Xprintf.h"
#include "FadeEffect.h"

//-------------------------------------------------------------------------------------------------
// コンストラクタ
SystemMain::SystemMain()
	: mGame()
	, mWindowWidth(Define::GameScreenWidth)
	, mWindowHeight(Define::GameScreenHeight)
	, mScreenMagnification(0)
{
}

//-------------------------------------------------------------------------------------------------
// デストラクタ
SystemMain::~SystemMain()
{
	DxLib_End();
}

//-------------------------------------------------------------------------------------------------
// システムの初期化処理
bool SystemMain::initialize()
{
	// ウィンドウサイズを設定する
	if (!setWindowSize()) {
		return false;
	}
	// DXライブラリの初期化
	if (!initializeDxLib()) {
		return false;
	}
	// シングルトンクラスの初期化
	Image::getIns()->initialize();
	BGM::getIns()->initialize();
	SE::getIns()->initialize();
	Keyboard::getIns()->initialize();
	Pad::getIns()->initialize();
	Mouse::getIns()->initialize(mScreenMagnification);
	// 文字列＆画面遷移エフェクト描画関数初期化
	Xprintf_Init();
	FadeEfect_Init();

	return true;
}

//-------------------------------------------------------------------------------------------------
// ウィンドウサイズを設定する
bool SystemMain::setWindowSize()
{	
	int fileHandle = FileRead_open("res/file/screen_size.txt");
	if (fileHandle == -1) {
		return false;
	}

	char c[10];
	FileRead_gets(c, 2, fileHandle);
	mScreenMagnification = atoi(c);

	mWindowWidth *= mScreenMagnification;
	mWindowHeight *= mScreenMagnification;

	FileRead_close(fileHandle);

	return true;
}

//-------------------------------------------------------------------------------------------------
// DXライブラリの初期化
bool SystemMain::initializeDxLib()
{	
	// 基本設定
	SetOutApplicationLogValidFlag(FALSE);
	SetWindowIconID(ID_ICON);
	SetMainWindowText("XEVIOUS MAKER");
	SetWindowStyleMode(0);
	SetDrawMode(DX_DRAWMODE_NEAREST);
	SetFullScreenResolutionMode(DX_FSRESOLUTIONMODE_DESKTOP);
	SetFullScreenScalingMode(DX_FSSCALINGMODE_NEAREST);
	SetGraphMode(mWindowWidth, mWindowHeight, 32);

	// 画面倍率が4以下ならウィンドウモードに変更
	if (mScreenMagnification <= 4) {
		ChangeWindowMode(TRUE);
	}

	// DXライブラリ初期化処理（異常が起きたらfalseを返す）
	if (DxLib_Init() == -1) {
		return false;
	}

	// マウスの非表示
	SetMouseDispFlag(FALSE);

	// 裏画面処理
	SetDrawScreen(DX_SCREEN_BACK);

	return true;
}

//-------------------------------------------------------------------------------------------------
// メインループ
void SystemMain::loop()
{
	int offscreenHandle = MakeScreen(Define::GameScreenWidth, Define::GameScreenHeight, FALSE);
	while (!ScreenFlip() && !ProcessMessage() && !SetDrawScreen(offscreenHandle) && !ClearDrawScreen()) {
		// ゲームのメインループ
		if (!mGame.mainLoop()) {
			break;
		}
		// 画面拡大処理
		SetDrawScreen(DX_SCREEN_BACK);
		DrawExtendGraph(0, 0, mWindowWidth, mWindowHeight, offscreenHandle, FALSE);
	}
}
