#include "Game.h"

//-------------------------------------------------------------------------------------------------
#include "Image.h"
#include "BGM.h"
#include "SE.h"
#include "Keyboard.h"
#include "Pad.h"
#include "Mouse.h"
#include "FadeEffect.h"

//-------------------------------------------------------------------------------------------------
// �R���X�g���N�^
Game::Game()
	: mFps()
	, mSceneManager()
{
}

//-------------------------------------------------------------------------------------------------
// ���C�����[�v
bool Game::mainLoop()
{
	// ���͏���
	Keyboard::getIns()->update();
	Pad::getIns()->update();
	Mouse::getIns()->update();

	// �V�[������
	mSceneManager.update();
	mSceneManager.draw();

	// �t�F�[�h�C��/�A�E�g
	FadeEfect_Update();
	FadeEfect_Draw();

	// �}�E�X�`��
	Mouse::getIns()->draw();

	// FPS����
	mFps.update();
	mFps.wait();
	//mFps.draw();

	// ESC�L�[�ŏI��
	if (Pad::getIns()->get(Pad_ESC) == 1) {
		return false;
	}

	return true;
}