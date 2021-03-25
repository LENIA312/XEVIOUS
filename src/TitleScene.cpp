#include "TitleScene.h"

//-------------------------------------------------------------------------------------------------
#include <DxLib.h>
#include "Pad.h"
#include "Image.h"
#include "SE.h"
#include "Xprintf.h"
#include "FadeEffect.h"
#include "Mouse.h"

//-------------------------------------------------------------------------------------------------
// �R���X�g���N�^
TitleScene::TitleScene(InterfaceSceneChanger* aChanger) : AbstractScene(aChanger)
{
	mButtonAnime = 0;
	mTenmetuTimer = 0;
	mIsGameStart = false;
	mIsEditorStart = false;

	mPlayer = new Player(168, 270 - 74);
	mUI = new UI(SceneList::Title);
	mCamera = new Camera();

	// �n�C�X�R�A
	int fp = FileRead_open("res/file/high_score.txt");
	FileRead_gets(mHighScore, 10, fp);
	FileRead_close(fp);
}

//-------------------------------------------------------------------------------------------------
// �f�X�g���N�^
TitleScene::~TitleScene()
{
	delete mPlayer;
	delete mUI;
	delete mCamera;
}

//-------------------------------------------------------------------------------------------------
// �X�V
void TitleScene::update()
{
	// �J����
	mCamera->update();
	float cameraY = mCamera->getY();
	mPlayer->setCameraY(cameraY);
	mBackground.setCameraY(cameraY);

	// �e�I�u�W�F�N�g�̍X�V
	mBackground.update();
	mPlayer->update();
	mUI->setHighScore(atoi(mHighScore));

	// PRESS SPACE�̓_��
	mTenmetuTimer++;
	if (mTenmetuTimer > 120) {
		mTenmetuTimer = 0;
	}

	// �X�y�[�X�L�[�ŃV�[���ύX
	{
		if (Pad::getIns()->get(Pad_Space) == 1 && !mIsGameStart && !mIsEditorStart && FadeEfect_GetState() == FadeState_Stop) {
			// ���ʉ�
			SE::getIns()->playSE(SE_Credit);
			SE::getIns()->setCanPlaySE(false);
			// �X�^�[�g�t���O��true��
			mIsGameStart = true;
			// �t�F�[�h�C��
			FadeEfect_Mode(FadeMode_Up);
			FadeEfect_Play();
		}
		// �X�^�[�g�t���O�������Ă�����
		if (mIsGameStart && FadeEfect_GetState() == FadeState_End) {
			// �t�F�[�h�A�E�g
			FadeEfect_Mode(FadeMode_Down);
			FadeEfect_Play();
			// �Q�[����ʂ�
			mSceneChanger->changeScene(SceneList::Game);
		}
	}

	// �G�f�B�^�[�{�^������������V�[���ύX
	{
		int mx = Mouse::getIns()->getXY(0);
		int my = Mouse::getIns()->getXY(1);
		mButtonAnime = 0;
		// �}�E�X�̈ʒu���{�^���ɐG��Ă�����
		if (mx >= 368 && mx <= 464 && my >= 236 && my <= 252) {
			// �{�^���A�j���ύX
			mButtonAnime = 1;
			// �{�^�������N���b�N������
			if (Mouse::getIns()->getClickLR(0) == 1 && !mIsGameStart && FadeEfect_GetState() == FadeState_Stop) {
				// ���ʉ�
				SE::getIns()->playSE(SE_Credit);
				// �X�^�[�g�t���O��true��
				mIsEditorStart = true;
				// �t�F�[�h�C��
				FadeEfect_Mode(FadeMode_Right);
				FadeEfect_Play();
			}
		}
		// �X�^�[�g�t���O�������Ă�����
		if (mIsEditorStart) {
			// �{�^���A�j���ύX
			mButtonAnime = 2;
			// �V�[���ύX
			if (FadeEfect_GetState() == FadeState_End) {
				// �t�F�[�h�A�E�g
				FadeEfect_Mode(FadeMode_Left);
				FadeEfect_Play();
				// �G�f�B�^�[��ʂ�
				mSceneChanger->changeScene(SceneList::Editor);
			}
		}
	}
}

//-------------------------------------------------------------------------------------------------
// �`��
void TitleScene::draw() const
{
	// �e�I�u�W�F�N�g�̕`��
	mBackground.draw();
	mPlayer->draw();

	// �^�C�g�����S & NAMCO���W
	DrawGraph(84, 32, Image::getIns()->getImage(Image_TitleRogoEditor), TRUE);
	Xprintf_Color(0);
	Xprintf_Position(2);
	Xprintf("@ ^ 1982 NAMCO LTD.", 176, 232);
	DrawGraph(144, 248, Image::getIns()->getImage(Image_Namco), TRUE);

	// PRESS_SPACE����
	if (mTenmetuTimer < 90 && !mIsGameStart) {
		Xprintf_Position(2);
		Xprintf("PRESS SPACE", 176, 144);
		Xprintf("ONE PLAYER ONLY", 176, 144 + 16);
	}

	// UI
	mUI->draw();

	// �G�f�B�^�[�{�^��
	DrawGraph(368, 236, Image::getIns()->getImage(Image_LevelEditorButton, mButtonAnime), true);
}
