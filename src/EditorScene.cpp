#include "EditorScene.h"

//-------------------------------------------------------------------------------------------------
#include <DxLib.h>
#include <fstream>
#include "FadeEffect.h"
#include "Image.h"
#include "Mouse.h"
#include "SE.h"
#include "Pad.h"
#include "BGM.h"

//-------------------------------------------------------------------------------------------------
// ��`
// 1�y�[�W�̓G�̎�ނ̗ʁi��������G�̗ʂɕω����������ꍇ�A�ς���΂����̂͂����������j
static const int ENEMY_VOLUME_0 = 14;
static const int ENEMY_VOLUME_1 = 4;
static const int ENEMY_VOLUME_2 = 1;
static const int ENEMY_VOLUME_3 = 1;
// 0����n�܂�y�[�W�ʂ̎n�_�ƏI�_�i�������͕ς��Ȃ��Ă������j
static const int ENEMY_START_1 = ENEMY_VOLUME_0;
static const int ENEMY_START_2 = ENEMY_START_1 + ENEMY_VOLUME_1;
static const int ENEMY_START_3 = ENEMY_START_2 + ENEMY_VOLUME_2;
static const int ENEMY_END_0 = ENEMY_VOLUME_0 - 1;
static const int ENEMY_END_1 = ENEMY_END_0 + ENEMY_VOLUME_1;
static const int ENEMY_END_2 = ENEMY_END_1 + ENEMY_VOLUME_2;
static const int ENEMY_END_3 = ENEMY_END_2 + ENEMY_VOLUME_3;

//-------------------------------------------------------------------------------------------------
// �R���X�g���N�^
EditorScene::EditorScene(InterfaceSceneChanger* aChanger) : AbstractScene(aChanger)
{
	// �ϐ�
	mNowArea = 0;
	mDifficulty = 2;
	mMouseX = 0;
	mMouseY = 0;
	mEnemyNum = 0;
	mEditorButtonState[0] = 0;
	mEditorButtonState[1] = 0;
	mEditorButtonState[2] = 0;
	mOkemasukaFlag = true;
	mSugunihaMuriFlag = false;
	mSavedFlag = false;
	mIsTestPlay = false;

	// �I�u�W�F�N�g
	mCamera = new Camera();
	mCamera->setMode(1);
	mBackground.setMode(1);
	mEditorUI = new EditorUI(mCamera, &mBackground);
	mPlayer = new Player();
	mUI = new UI(SceneList::Editor);
	mNPCMgr = new NPCManager(mPlayer);

	// �t�@�C���ǂݍ���
	readNPCFile();

	// BGM�Đ�
	BGM::getIns()->playBGM(BGM_Editer_01, true, true);
	BGM::getIns()->playBGM(BGM_Editer_02, true, true);
	BGM::getIns()->volume(BGM_Editer_02, 0);
}

//-------------------------------------------------------------------------------------------------
// �f�X�g���N�^
EditorScene::~EditorScene()
{
	delete mCamera;
	delete mEditorUI;
	delete mPlayer;
	delete mUI;
	delete mNPCMgr;
}

//-------------------------------------------------------------------------------------------------
// �X�V
void EditorScene::update()
{
	// �e�X�g�v���C���Ă��Ȃ���----------------------------
	if (!mIsTestPlay) {
		if (mEditorButtonState[1] != 2) {
			// �J����
			mCamera->update();
			mBackground.setCameraY(mCamera->getY());
			// �w�i
			mBackground.update();
			mNowArea = mBackground.getArea();
			// UI
			{
				mEditorUI->update();
				mEnemyNum = mEditorUI->getEnemyNum();
				mDifficulty = mEditorUI->getDifficulty();
				for (int i = 0; i < 3; i++) {
					mEditorButtonState[i] = mEditorUI->getEditorButtonState(i);
				}
				mEditorUI->setEnemyCount(mSetNPCList[mDifficulty][mNowArea].size());
			}
			// NPC�X�V
			{
				// �X�V
				for (auto npc : mSetNPCList[mDifficulty][mNowArea]) {
					npc->setCameraY((int)mCamera->getY());
					npc->setMouse(Mouse::getIns()->getXY(0), Mouse::getIns()->getXY(1),
						Mouse::getIns()->getClickLR(0), Mouse::getIns()->getClickLR(1));
					npc->update();
				}
				// �A�N�e�B�u�t���O��false�Ȃ烊�X�g����폜
				for (auto enemy = mSetNPCList[mDifficulty][mNowArea].begin(); enemy != mSetNPCList[mDifficulty][mNowArea].end();) {
					if ((*enemy)->getActiveFlag() == false) {
						enemy = mSetNPCList[mDifficulty][mNowArea].erase(enemy);
					}
					else {
						enemy++;
					}
				}
			}
			// �G�̔z�u
			setNPC();
			// �V���[�g�J�b�g�L�[
			{
				// ���L�[�ŃG���A�ύX
				if (Pad::getIns()->get(Pad_Left) == 1) {
					mBackground.addArea(-1);
				}
				// �E�L�[�ŃG���A�ύX
				if (Pad::getIns()->get(Pad_Right) == 1) {
					mBackground.addArea(1);
				}
				// �X�y�[�X�L�[�Ńe�X�g�v���C
				if (Pad::getIns()->get(Pad_Space) == 1) {
					mEditorButtonState[0] = 2;
					SE::getIns()->playSE(SE_Credit);
				}
			}
			// �G�f�B�^�[�{�^��
			editorButton();
		}
		// �Z�[�u
		else
		{
			//�Z�[�u����
			if (!mSavedFlag) {
				saveNPCFile();
				mSavedFlag = true;
			}
			// ���N���b�N��SAVED��ʉ���
			if (Mouse::getIns()->getClickLR(0) == 1) {
				mEditorUI->resetEditorButton();
				mEditorButtonState[1] = 0;
				mSugunihaMuriFlag = true;
				mSavedFlag = false;
			}
		}
	}
	// �e�X�g�v���C��------------------------------------------
	else {
		// �e�I�u�W�F�N�g�̍X�V
		{
			// �J�����̍X�V
			{
				mCamera->update();
				// �J�����̍��W���Z�b�g
				float cameraY = mCamera->getY();
				mPlayer->setCameraY(cameraY);
				mBackground.setCameraY(cameraY);
				mNPCMgr->setCamera(cameraY);
			}
			// �e�I�u�W�F�N�g�̍X�V
			{
				mBackground.update();
				mPlayer->update();
				mNPCMgr->setNowArea(mBackground.getArea());
				mNPCMgr->setDifficulty(mDifficulty);
				mNPCMgr->update();
			}
			// UI�̍X�V
			{
				mUI->setScore(mNPCMgr->getScore());
				mUI->setArea(mBackground.getArea() + 1);
				mUI->setDifficulty(mDifficulty + 1);
				mUI->setSkyNPC(mNPCMgr->getNPCListSize(1));
				mUI->setGroundNPC(mNPCMgr->getNPCListSize(0));
			}
			// �X�y�[�X�L�[�ł�߂�
			if (Pad::getIns()->get(Pad_Space) == 1) {
				SE::getIns()->playSE(SE_Credit);
				mIsTestPlay = false;
				BGM::getIns()->volume(BGM_Editer_01, 255);
				BGM::getIns()->volume(BGM_Editer_02, 0);
				mEditorUI->resetEditorButton();
				mBackground.setMode(1);
				mCamera->setMode(1);
				mNPCMgr->clearList();
				mNPCMgr->resetScore();
				mUI->setScore(mNPCMgr->getScore());
				BGM::getIns()->stopBGM(BGM_Boss);
				for (auto npc : mSetNPCList[mDifficulty][mNowArea]) {
					npc->setCameraY((int)mCamera->getY());
				}
			}
		}
	}
}

//-------------------------------------------------------------------------------------------------
// �`��
void EditorScene::draw() const
{
	// �e�X�g�v���C���Ă��Ȃ���----------------------------
	if (!mIsTestPlay) {
		// �w�i�`��
		mBackground.draw();
		// �O���b�h�`��
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 60);
			for (int i = 0; i < 44; i++) {
				DrawLine(i * 8, 0, i * 8, 270, GetColor(0, 0, 0));
			}
			for (int i = 0; i < 34; i++) {
				DrawLine(0, i * 8, 352, i * 8, GetColor(0, 0, 0));
			}
			int cy = (int)mCamera->getY();
			for (int i = 0; i < 7; i++) {
				DrawLine(0, i * 272 + 152 - cy, 352, i * 272 + 152 - cy, GetColor(0, 0, 0));
			}
			DrawLine(176, 0, 176, 270, GetColor(0, 0, 0));
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		// �}�E�X�̈ʒu�Ɍ��ݑI�����Ă���G��\��
		if (Mouse::getIns()->getXY(0) < 352 &&
			Mouse::getIns()->getIsInScreen() &&
			Mouse::getIns()->getClickLR(1) == 0 && mEditorButtonState[1] != 2) {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
			switch (mEnemyNum) {
			case 0:case 1:case 2:case 3:case 4:case 5:
			case 6:case 7:case 8:case 9:case 11:case 12:
			case 13: // 16��
				DrawGraph(mMouseX / 4 * 4, mMouseY / 4 * 4, Image::getIns()->getImage(Image_EnemyIcon, mEnemyNum), TRUE);
				break;
			case 14:case 15:case 16:
			case 17: // 32��
				DrawGraph(mMouseX / 4 * 4, mMouseY / 4 * 4, Image::getIns()->getImage(Image_EnemyBigIcon, mEnemyNum - 14), TRUE);
				break;
			case 18: // �{�X
				DrawGraph(mMouseX / 4 * 4, mMouseY / 4 * 4, Image::getIns()->getImage(Image_BossIcon), TRUE);
				break;
			case 19: // �A�C�e��
				DrawGraph(mMouseX / 4 * 4, mMouseY / 4 * 4, Image::getIns()->getImage(Image_SpecialFlag), TRUE);
				break;
			case 10: // �{�U���O����
				DrawGraph((mMouseX / 4 * 4) - 12, mMouseY / 4 * 4, Image::getIns()->getImage(Image_EnemyIcon, 8), TRUE);
				DrawGraph((mMouseX / 4 * 4) + 12, mMouseY / 4 * 4, Image::getIns()->getImage(Image_EnemyIcon, 8), TRUE);
				DrawGraph(mMouseX / 4 * 4, (mMouseY / 4 * 4) - 12, Image::getIns()->getImage(Image_EnemyIcon, 8), TRUE);
				DrawGraph(mMouseX / 4 * 4, (mMouseY / 4 * 4) + 12, Image::getIns()->getImage(Image_EnemyIcon, 8), TRUE);
				DrawGraph(mMouseX / 4 * 4, mMouseY / 4 * 4, Image::getIns()->getImage(Image_EnemyIcon, 10), TRUE);
				break;
			case 20: case 21: case 22: case 23: // �e��
				DrawGraph(mMouseX / 4 * 4, mMouseY / 4 * 4, Image::getIns()->getImage(Image_EnemyIcon, mEnemyNum - 6), TRUE);
				break;
			}
			// �u���Ȃ�������Ԃ����i��\��
			if (!mOkemasukaFlag) {
				int mouse_w = 0;
				int mouse_h = 0;
				switch (mEnemyNum) {
				case 0:case 1:case 2:case 3:case 4:case 5:
				case 6:case 7:case 8:case 9:case 11:case 12:
				case 13: case 20: case 21: case 22: case 23: case 16:// 16��
					mouse_w = 16;
					mouse_h = 16;
					DrawBox(mMouseX / 4 * 4, mMouseY / 4 * 4, (mMouseX / 4 * 4) + mouse_w, (mMouseY / 4 * 4) + mouse_h, GetColor(255, 0, 0), TRUE);
					break;
				case 14:case 15:case 17: // 32��
					mouse_w = 32;
					mouse_h = 32;
					DrawBox(mMouseX / 4 * 4, mMouseY / 4 * 4, (mMouseX / 4 * 4) + mouse_w, (mMouseY / 4 * 4) + mouse_h, GetColor(255, 0, 0), TRUE);
					break;
				case 18: // �{�X
					mouse_w = 96;
					mouse_h = 96;
					DrawBox(mMouseX / 4 * 4, mMouseY / 4 * 4, (mMouseX / 4 * 4) + mouse_w, (mMouseY / 4 * 4) + mouse_h, GetColor(255, 0, 0), TRUE);
					break;
				case 19: // �A�C�e��
					mouse_w = 16;
					mouse_h = 16;
					DrawBox(mMouseX / 4 * 4, mMouseY / 4 * 4, (mMouseX / 4 * 4) + mouse_w, (mMouseY / 4 * 4) + mouse_h, GetColor(255, 0, 0), TRUE);
					break;
				case 10: // �{�U���O����
					mouse_w = 37;
					mouse_h = 37;
					DrawBox((mMouseX / 4 * 4) - 10, (mMouseY / 4 * 4) - 10, (mMouseX / 4 * 4) - 10 + mouse_w, (mMouseY / 4 * 4) - 10 + mouse_h, GetColor(255, 0, 0), TRUE);
					break;
				}
			}
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		// NPC�X�V
		for (const auto npc : mSetNPCList[mDifficulty][mNowArea]) {
			npc->draw();
		}
		// UI�`��
		mEditorUI->draw();
		// �Z�[�u������
		if (mEditorButtonState[1] == 2) {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
			DrawBox(0, 0, 352, 270, GetColor(0, 0, 0), TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			DrawGraph(96, 96, Image::getIns()->getImage(Image_Saved), TRUE);
		}
	}
	// �e�X�g�v���C��----------------------------------------------------
	else {
		// �e�I�u�W�F�N�g�̕`��
		mBackground.draw();
		mNPCMgr->draw();
		mPlayer->draw();
		// UI
		mUI->draw();
	}
}

//-------------------------------------------------------------------------------------------------
// �G�̔z�u
void EditorScene::setNPC()
{
	// ��ʓ����AUI�ɐG��Ă��Ȃ����̂ݕϐ��ɑ��
	if (Mouse::getIns()->getIsInScreen() && Mouse::getIns()->getXY(0) < 352) {
		mMouseX = Mouse::getIns()->getXY(0);
		mMouseY = Mouse::getIns()->getXY(1);
	}
	// ���N���b�N�œG��z�u
	{
		int okerukadouka = 0;
		int mouse_x = Mouse::getIns()->getXY(0);
		int mouse_y = Mouse::getIns()->getXY(1);
		int mouse_w;
		int mouse_h;
		switch (mEnemyNum) {
		case 0:case 1:case 2:case 3:case 4:case 5:
		case 6:case 7:case 8:case 9:case 11:case 12:
		case 13: case 20: case 21: case 22: case 23: case 16: // 16��
			mouse_w = 16 - 1;
			mouse_h = 16 - 1;
			break;
		case 14:case 15:case 17: // 32��
			mouse_w = 32 - 1;
			mouse_h = 32 - 1;
			break;
		case 18: // �{�X
			mouse_w = 96 - 1;
			mouse_h = 96 - 1;
			break;
		case 19: // �A�C�e��
			mouse_w = 16 - 1;
			mouse_h = 16 - 1;
			break;
		case 10: // �{�U���O����
			mouse_x = Mouse::getIns()->getXY(0) - 12;
			mouse_y = Mouse::getIns()->getXY(1) - 12;
			mouse_w = 37 - 1;
			mouse_h = 37 - 1;
		}
		int camera_y = (int)mCamera->getY();
		// �u���邩�ǂ����`�F�b�N
		{
			// �}�E�X�␳
			int tmp_x = mouse_x / 4 * 4;
			int tmp_y = mouse_y / 4 * 4;
			for (auto npc : mSetNPCList[mDifficulty][mNowArea]) {
				// NPC���W
				int npc_x = npc->getCollisionPoint(0);
				int npc_y = npc->getCollisionPoint(1);
				int npc_w = npc->getCollisionPoint(2) - 1;
				int npc_h = npc->getCollisionPoint(3) - 1;
				// �{�U���O�����␳
				if (npc->getData(0) == 10) {
					npc_x -= 12;
					npc_y -= 12;
				}
				if (tmp_x + mouse_w >= npc_x && tmp_x <= npc_x + npc_w &&
					tmp_y + camera_y + mouse_h >= npc_y && tmp_y + camera_y <= npc_y + npc_h) {
					okerukadouka++;
				}
			}
			// ��ʊO�ɒu���Ȃ�
			if (tmp_x + mouse_w >= 352 || tmp_y + mouse_h >= 270) {
				okerukadouka = 1;
			}
		}
		// �u���邩�ǂ����t���O��0�̎����N���b�N������z�u
		if (okerukadouka == 0 &&
			Mouse::getIns()->getClickLR(0) && Mouse::getIns()->getClickLR(1) == 0 &&
			mouse_x < 352 && Mouse::getIns()->getIsInScreen() &&
			mSetNPCList[mDifficulty][mNowArea].size() < 256 && !mSugunihaMuriFlag) {
			// �{�U���O�����␳
			if (mEnemyNum == 10) {
				mouse_x += 12;
				mouse_y += 12;
			}
			// ���X�g�ɒǉ�
			mSetNPCList[mDifficulty][mNowArea].emplace_back(std::make_shared<SetNPC>
				(mEnemyNum, 0, mouse_x / 4 * 4, (mouse_y + camera_y) / 4 * 4));
			// �J�����X�V
			for (auto npc : mSetNPCList[mDifficulty][mNowArea]) {
				npc->setCameraY((int)mCamera->getY());
			}
		}
		// �u���邩�ǂ����t���O�X�V
		if (okerukadouka == 0) {
			mOkemasukaFlag = true;
		}
		else {
			mOkemasukaFlag = false;
		}
	}
	// UI�ɐG��Ă��Ȃ����}�E�X�̉摜��ς���
	if (Mouse::getIns()->getXY(0) < 352) {
		if (Mouse::getIns()->getClickLR(1)) {
			Mouse::getIns()->setAnime(2);
		}
		else {
			Mouse::getIns()->setAnime(1);
		}
	}
	// ���N���b�N�𗣂����Ƃ������ɂ͖����t���O��false��
	if (mSugunihaMuriFlag && Mouse::getIns()->getClickLR(0) == 0) {
		mSugunihaMuriFlag = false;
	}
}

//-------------------------------------------------------------------------------------------------
// ���ɂ���R�̃{�^���̏���
void EditorScene::editorButton()
{

	// �e�X�g�v���C�J�n
	{
		if (mEditorButtonState[0] == 2) {
			// ������
			mIsTestPlay = true;
			saveNPCFile();
			mPlayer->reset();
			mBackground.setMode(0);
			mCamera->setMode(0);
			mNPCMgr->clearList();
			mNPCMgr->readNPCFile();
			mNPCMgr->testPlayModeOn();
			BGM::getIns()->volume(BGM_Editer_01, 0);
			BGM::getIns()->volume(BGM_Editer_02, 255);
		}
	}
	// �^�C�g����ʂ֖߂�
	{
		if (mEditorButtonState[2] == 2) {
			if (FadeEfect_GetState() == FadeState_Stop) {
				FadeEfect_Mode(FadeMode_Left);
				FadeEfect_Play();
			}
			if (FadeEfect_GetState() == FadeState_End) {
				// �V�[���ύX
				mSceneChanger->changeScene(SceneList::Title);
				FadeEfect_Mode(FadeMode_Right);
				FadeEfect_Play();
				// BGM���I�t��
				BGM::getIns()->stopBGM(BGM_Editer_01);
				BGM::getIns()->stopBGM(BGM_Editer_02);
				BGM::getIns()->volume(BGM_Editer_01, 255);
				BGM::getIns()->volume(BGM_Editer_02, 255);
			}
		}
	}
}

//-------------------------------------------------------------------------------------------------
// NPC�̃t�@�C����ǂݍ���
void EditorScene::readNPCFile()
{
	// �ϐ��錾
	int num;
	int area;
	int input[64];
	char inputc[64];
	int fp[5];
	int t, p, x, y;
	fp[0] = FileRead_open("res/file/NPCData_0.csv");
	fp[1] = FileRead_open("res/file/NPCData_1.csv");
	fp[2] = FileRead_open("res/file/NPCData_2.csv");
	fp[3] = FileRead_open("res/file/NPCData_3.csv");
	fp[4] = FileRead_open("res/file/NPCData_4.csv");
	// 5��t�@�C���ǂݍ���
	for (int i = 0; i < 5; i++) {
		num = 0;
		area = 0;
		t = 0;
		p = 0;
		x = 0;
		y = 0;
		while (1) {
			for (int j = 0; j < 64; j++) {
				// 1�����擾����
				inputc[j] = input[j] = FileRead_getc(fp[i]);
				// �J���}�����s�Ȃ炻���܂ł𕶎���Ƃ���
				if (input[j] == ',' || input[j] == '\n') {
					inputc[j] = '\0';
					break;
				}
				// �t�@�C���̏I���Ȃ�I��
				if (input[j] == EOF) {
					goto EXFILE;
				}
				// �X���b�V��������Ύ��̃G���A��
				if (inputc[j] == '/') {
					area++;
					while (FileRead_getc(fp[i]) != '\n');
					//�J�E���^���ŏ��ɖ߂�
					j = -1;
					continue;
				}
			}
			switch (num) {
			case 0: // �G�̎��
				t = atoi(inputc);
				break;
			case 1: // �s���p�^�[��
				p = atoi(inputc);
				break;
			case 2: // x���W
				x = atoi(inputc);
				break;
			case 3: // y���W
				y = atoi(inputc);
				break;
			}
			num++;
			if (num == 4) {
				num = 0;
				switch (t) {
				case 0:case 1:case 2:case 3:case 4:case 5:
				case 6:case 7:case 8:case 9:case 11:case 12:
				case 13:case 16: case 20: case 21: case 22: case 23:  // 16��
					y -= 16;
					break;
				case 14:case 15:case 17: // 32��
					y -= 32;
					break;
				case 18: // �{�X
					y -= 96;
					break;
				case 19: // �A�C�e��
					y -= 16;
					break;
				case 10: // �{�U���O����
					y -= 37;
					break;
				}
				mSetNPCList[i][area].emplace_back(std::make_shared<SetNPC>(t, p, x, y));
			}
		}
	EXFILE:
		FileRead_close(fp[i]);
	}
}

//-------------------------------------------------------------------------------------------------
// NPC�̃t�@�C�����Z�[�u
void EditorScene::saveNPCFile()
{
	// �ϐ��錾
	int t, p, x, y;
	t = 0;
	p = 0;
	x = 0;
	y = 0;
	std::ofstream ofs0("res/file/NPCData_0.csv");
	std::ofstream ofs1("res/file/NPCData_1.csv");
	std::ofstream ofs2("res/file/NPCData_2.csv");
	std::ofstream ofs3("res/file/NPCData_3.csv");
	std::ofstream ofs4("res/file/NPCData_4.csv");
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 16; j++) {
			for (auto npc : mSetNPCList[i][j]) {
				// �f�[�^����������
				t = npc->getData(0);
				p = npc->getData(1);
				x = npc->getData(2);
				y = npc->getData(3);
				switch (i) {
				case 0:
					ofs0 << t << ", " << p << ", " << x << "," << y << std::endl;
					break;
				case 1:
					ofs1 << t << ", " << p << ", " << x << "," << y << std::endl;
					break;
				case 2:
					ofs2 << t << ", " << p << ", " << x << "," << y << std::endl;
					break;
				case 3:
					ofs3 << t << ", " << p << ", " << x << "," << y << std::endl;
					break;
				case 4:
					ofs4 << t << ", " << p << ", " << x << "," << y << std::endl;
					break;
				}
			}
			// ���̃G���A��
			switch (i) {
			case 0:
				ofs0 << "/" << std::endl;
				break;
			case 1:
				ofs1 << "/" << std::endl;
				break;
			case 2:
				ofs2 << "/" << std::endl;
				break;
			case 3:
				ofs3 << "/" << std::endl;
				break;
			case 4:
				ofs4 << "/" << std::endl;
				break;
			}
		}
	}
}
