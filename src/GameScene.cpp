#include "GameScene.h"

//-------------------------------------------------------------------------------------------------
#include <DxLib.h>
#include <fstream>
#include "Pad.h"
#include "BGM.h"
#include "SE.h"
#include "Xprintf.h"
#include "FadeEffect.h"
#include "Mouse.h"

//-------------------------------------------------------------------------------------------------
// �R���X�g���N�^
GameScene::GameScene(InterfaceSceneChanger* aChanger) : AbstractScene(aChanger)
{
	mPlayerLeft = 2;
	mScore = 0;
	mDifficulty = 2;
	mIsStartEvent = true;
	mStartEventTimer = 0;
	mIsGameOverEvent = false;
	mGameOverEventTimer = 0;
	mIsPause = false;
	mExtendFlg = false;
	mExtendCnt = 0;
	mReturnTitleTimer = 0;
	mReturnTitleFlg = false;

	mPlayer = new Player();
	mUI = new UI(SceneList::Game);
	mCamera = new Camera();
	mNPCMgr = new NPCManager(mPlayer);

	// �n�C�X�R�A
	int fp = FileRead_open("res/file/high_score.txt");
	char c[10];
	FileRead_gets(c, 10, fp);
	mHighScore = atoi(c);
	FileRead_close(fp);
}

//-------------------------------------------------------------------------------------------------
// �f�X�g���N�^
GameScene::~GameScene()
{
	delete mPlayer;
	delete mUI;
	delete mCamera;
	delete mNPCMgr;
}

//-------------------------------------------------------------------------------------------------
// �X�V
void GameScene::update()
{
	// �|�[�Y���Ă��Ȃ���
	if (!mIsPause)
	{
		// �e�I�u�W�F�N�g�̍X�V
		{
			// �}�E�X��\��
			Mouse::getIns()->setAnime(3);
			// �J�����̍X�V
			{
				mCamera->update();
				// �J�����̍��W���Z�b�g
				float cameraY = mCamera->getY();
				mPlayer->setCameraY(cameraY);
				mBackground.setCameraY(cameraY);
				mNPCMgr->setCamera(cameraY);
			}
			// �w�i�X�V
			{
				// �O�̃G���A��ێ�
				int tmpArea = mBackground.getArea();
				// �X�V
				mBackground.update();
				// �G���A���ς�������Փx+1
				if (tmpArea != mBackground.getArea() && mDifficulty < 4) {
					mDifficulty++;
				}
			}
			// �e�I�u�W�F�N�g�̍X�V
			{
				mPlayer->update();
				mNPCMgr->setNowArea(mBackground.getArea());
				mNPCMgr->setDifficulty(mDifficulty);
				mNPCMgr->update();
			}
			// �c����Z
			{
				// ���Z����0�ȊO�Ȃ�
				if (mNPCMgr->getAddLeft() != 0) {
					mPlayerLeft += mNPCMgr->getAddLeft();
					// ���Z�������Z�b�g
					mNPCMgr->resetAddLeft();
				}
				// �X�R�A�����܂�����
				if (!mExtendFlg && mNPCMgr->getScore() >= 20000 && mExtendCnt == 0) {
					mExtendFlg = true;
					mExtendCnt++;
				}
				if (!mExtendFlg && mNPCMgr->getScore() >= 60000 * mExtendCnt && mExtendCnt > 0) {
					mExtendFlg = true;
					mExtendCnt++;
				}
				if (mExtendFlg) {
					mExtendFlg = false;
					mPlayerLeft++;
					SE::getIns()->playSE(SE_Extend);
				}
				// �c���9�ȏ㑝���Ȃ�
				if (mPlayerLeft > 9) {
					mPlayerLeft = 9;
				}
			}
			// �n�C�X�R�A�X�V
			if (mNPCMgr->getScore() > mHighScore) {
				mHighScore = mNPCMgr->getScore();
			}
			// UI�̍X�V
			mUI->setHighScore(mHighScore);
			mUI->setScore(mNPCMgr->getScore());
			mUI->setLeft(mPlayerLeft);
			mUI->setArea(mBackground.getArea() + 1);
		}
		// �C�x���g
		{
			startEvent();
			gameOverEvent();			
		}
	}
	// �|�[�Y�C�x���g
	pauseEvent();
}

//-------------------------------------------------------------------------------------------------
// �`��
void GameScene::draw() const
{
	// �e�I�u�W�F�N�g�̕`��
	mBackground.draw();
	mNPCMgr->draw();
	mPlayer->draw();
	// �X�^�[�g�C�x���g���̕���
	if (mIsStartEvent && mStartEventTimer < 190 && !mIsGameOverEvent) {
		Xprintf_Position(2);
		Xprintf("PLAYER ONE", 176, 144);
		Xprintf("READY!", 176, 144 + 16);
		Xprintf_Position(0);
		XNprintf(mPlayerLeft, 116, 144 + 16 * 2);
		Xprintf_Position(2);
		Xprintf("  SOLVALOU LEFT", 176, 144 + 16 * 2);
	}
	// �Q�[���I�[�o�[���̕���
	if (mIsGameOverEvent && mPlayerLeft < 0 && mGameOverEventTimer > 120) {
		Xprintf_Position(2);
		Xprintf("GAME OVER", 176, 135);
	}
	// �|�[�Y���̕���
	if (mIsPause) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
		DrawBox(0, 270, 352, 270 - 24, GetColor(0, 0, 0), TRUE);
		DrawBox(0, 270, mReturnTitleTimer, 270 - 24, GetColor(231, 81, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		Xprintf_Position(2);
		Xprintf("[ SHIFT ] : RETURN TO TITLE", 176, 270 - 16);
	}
	// UI
	mUI->draw();

	// �f�o�b�O
	//clsDx();
	//printfDx("%d", mDifficulty);
}

//-------------------------------------------------------------------------------------------------
// �X�^�[�g�C�x���g
void GameScene::startEvent()
{
	if (mIsStartEvent && !mIsGameOverEvent) {
		// ���߂�BGM�𗬂���SE�Đ��t���O��܂�
		if (mStartEventTimer == 0) {
			BGM::getIns()->playBGM(BGM_Start, false, true);
			SE::getIns()->setCanPlaySE(false);
		}
		// BGM����I�������C�x���g�I��
		if (!BGM::getIns()->checkIsPlay(BGM_Start)) {
			mIsStartEvent = false;
			// SE�Đ��t���O�𗧂Ă�
			SE::getIns()->setCanPlaySE(true);
			// BGM�𗬂�
			BGM::getIns()->playBGM(BGM_Game, true, true);
		}
		mStartEventTimer++;
	}
	else {
		mStartEventTimer = 0;
	}
}

//-------------------------------------------------------------------------------------------------
// �Q�[���I�[�o�[
void GameScene::gameOverEvent()
{
	// �v���C���[�����Ă��ꂽ��
	if (!mPlayer->checkActiveFlag() && !mIsGameOverEvent) {
		// �t���O�𗧂Ă�
		mIsGameOverEvent = true;
		// BGM���~�߂�
		{
			// �X�^�[�g�C�x���g���Ȃ�
			if (mIsStartEvent) {
				BGM::getIns()->stopBGM(BGM_Start);
				static int tmpSE = LoadSoundMem("res/se/miss.wav");
				PlaySoundMem(tmpSE, DX_PLAYTYPE_BACK, TRUE);
			}
			// �Q�[���v���C���Ȃ�
			else {
				BGM::getIns()->stopBGM(BGM_Game);
				BGM::getIns()->stopBGM(BGM_Boss);
			}
		}
		// �c�@�����炷
		mPlayerLeft--;
	}
	// �C�x���g����
	if (mIsGameOverEvent) {
		// �c�@���c���Ă��鎞
		if (mPlayerLeft >= 0) {
			if (mGameOverEventTimer == 60) {
				FadeEfect_Mode(FadeMode_Up);
				FadeEfect_Play();
			}
			if (FadeEfect_GetState() == FadeState_End) {
				// �O�̃G���A��ێ�
				int tmpArea = mBackground.getArea();
				// ���X�^�[�g
				mCamera->reset();
				mPlayer->reset();
				mBackground.reset();
				mNPCMgr->clearList();
				mIsGameOverEvent = false;
				mIsStartEvent = true;
				FadeEfect_Mode(FadeMode_Down);
				FadeEfect_Play();
				// �G���A���ς���Ă��Ȃ��������Փx -1
				if (tmpArea == mBackground.getArea() && mDifficulty > 0) {
					mDifficulty -= 1;
				}
			}
		}
		// �c�@���c���Ă��Ȃ���
		else {
			if (mGameOverEventTimer == 240) {
				FadeEfect_Mode(FadeMode_Down);
				FadeEfect_Play();
			}
			if (FadeEfect_GetState() == FadeState_End) {
				// �n�C�X�R�A�Z�[�u
				std::ofstream ofs("res/file/high_score.txt");
				ofs << mHighScore << std::endl;
				// �^�C�g���V�[����
				mSceneChanger->changeScene(SceneList::Title);
				FadeEfect_Mode(FadeMode_Up);
				FadeEfect_Play();
			}
		}
		mGameOverEventTimer++;
	}
	else {
		mGameOverEventTimer = 0;
	}
}

//-------------------------------------------------------------------------------------------------
// �|�[�Y�C�x���g
void GameScene::pauseEvent()
{
	// �v���C��
	if (!mIsPause) {
		// �X�y�[�X�L�[�Ń|�[�Y
		if (Pad::getIns()->get(Pad_Space) == 1 && !mIsGameOverEvent) {
			mIsPause = true;
			// �X�^�[�g�C�x���g���Ȃ�
			if (mIsStartEvent) {
				SE::getIns()->setCanPlaySE(true);
				BGM::getIns()->stopBGM(BGM_Start);
			}
			// �Q�[���v���C���Ȃ�
			else {
				BGM::getIns()->stopBGM(BGM_Game);
				BGM::getIns()->stopBGM(BGM_Boss);
			}
			// SE
			SE::getIns()->playSE(SE_Credit);
		}
	}
	// �|�[�Y��
	else {
		// �^�C�g����ʂ֖߂�t���O��false�̎�
		if (!mReturnTitleFlg) {
			// �X�y�[�X�L�[�Ń|�[�Y����
			if (Pad::getIns()->get(Pad_Space) == 1) {
				mIsPause = false;
				// ���SE��炷
				SE::getIns()->playSE(SE_Credit);
				// �X�^�[�g�C�x���g���Ȃ�
				if (mIsStartEvent) {
					SE::getIns()->setCanPlaySE(false);
					BGM::getIns()->playBGM(BGM_Start, false, false);
				}
				// �Q�[���v���C���Ȃ�
				else {
					BGM::getIns()->playBGM(BGM_Game, true, false);
				}
			}
			// �V�t�g�L�[�Ń^�C�g���֖߂�
			if (Pad::getIns()->get(Pad_LShift) > 0 || Pad::getIns()->get(Pad_RShift) > 0) {
				mReturnTitleTimer += 8;
			}
			else if (mReturnTitleTimer > 0) {
				mReturnTitleTimer = 0;
			}
			if (mReturnTitleTimer >= 352) {
				mReturnTitleFlg = true;
				SE::getIns()->setCanPlaySE(true);
				SE::getIns()->playSE(SE_Credit);
			}
		}
		// �^�C�g����ʂ֖߂鏈��
		else {
			if (FadeEfect_GetState() == FadeState_Stop) {
				FadeEfect_Mode(FadeMode_Down);
				FadeEfect_Play();
			}
			if (FadeEfect_GetState() == FadeState_End) {
				// �^�C�g���V�[����
				mSceneChanger->changeScene(SceneList::Title);
				FadeEfect_Mode(FadeMode_Up);
				FadeEfect_Play();
			}
		}
	}
}
