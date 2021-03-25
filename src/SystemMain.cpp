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
// �R���X�g���N�^
SystemMain::SystemMain()
	: mGame()
	, mWindowWidth(Define::GameScreenWidth)
	, mWindowHeight(Define::GameScreenHeight)
	, mScreenMagnification(0)
{
}

//-------------------------------------------------------------------------------------------------
// �f�X�g���N�^
SystemMain::~SystemMain()
{
	DxLib_End();
}

//-------------------------------------------------------------------------------------------------
// �V�X�e���̏���������
bool SystemMain::initialize()
{
	// �E�B���h�E�T�C�Y��ݒ肷��
	if (!setWindowSize()) {
		return false;
	}
	// DX���C�u�����̏�����
	if (!initializeDxLib()) {
		return false;
	}
	// �V���O���g���N���X�̏�����
	Image::getIns()->initialize();
	BGM::getIns()->initialize();
	SE::getIns()->initialize();
	Keyboard::getIns()->initialize();
	Pad::getIns()->initialize();
	Mouse::getIns()->initialize(mScreenMagnification);
	// �����񁕉�ʑJ�ڃG�t�F�N�g�`��֐�������
	Xprintf_Init();
	FadeEfect_Init();

	return true;
}

//-------------------------------------------------------------------------------------------------
// �E�B���h�E�T�C�Y��ݒ肷��
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
// DX���C�u�����̏�����
bool SystemMain::initializeDxLib()
{	
	// ��{�ݒ�
	SetOutApplicationLogValidFlag(FALSE);
	SetWindowIconID(ID_ICON);
	SetMainWindowText("XEVIOUS MAKER");
	SetWindowStyleMode(0);
	SetDrawMode(DX_DRAWMODE_NEAREST);
	SetFullScreenResolutionMode(DX_FSRESOLUTIONMODE_DESKTOP);
	SetFullScreenScalingMode(DX_FSSCALINGMODE_NEAREST);
	SetGraphMode(mWindowWidth, mWindowHeight, 32);

	// ��ʔ{����4�ȉ��Ȃ�E�B���h�E���[�h�ɕύX
	if (mScreenMagnification <= 4) {
		ChangeWindowMode(TRUE);
	}

	// DX���C�u���������������i�ُ킪�N������false��Ԃ��j
	if (DxLib_Init() == -1) {
		return false;
	}

	// �}�E�X�̔�\��
	SetMouseDispFlag(FALSE);

	// ����ʏ���
	SetDrawScreen(DX_SCREEN_BACK);

	return true;
}

//-------------------------------------------------------------------------------------------------
// ���C�����[�v
void SystemMain::loop()
{
	int offscreenHandle = MakeScreen(Define::GameScreenWidth, Define::GameScreenHeight, FALSE);
	while (!ScreenFlip() && !ProcessMessage() && !SetDrawScreen(offscreenHandle) && !ClearDrawScreen()) {
		// �Q�[���̃��C�����[�v
		if (!mGame.mainLoop()) {
			break;
		}
		// ��ʊg�又��
		SetDrawScreen(DX_SCREEN_BACK);
		DrawExtendGraph(0, 0, mWindowWidth, mWindowHeight, offscreenHandle, FALSE);
	}
}
