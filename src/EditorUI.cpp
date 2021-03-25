#include <DxLib.h>
#include "EditorUI.h"
#include "SE.h"
#include "Mouse.h"
#include "Xprintf.h"
#include "FadeEffect.h"
#include "Image.h"

/*------------------------------------------------------------------------------
�Ȃɂ��Ȃ񂾂��悭�킩��Ȃ��Ȃ�����`
------------------------------------------------------------------------------*/
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

/*------------------------------------------------------------------------------
�R���X�g���N�^
------------------------------------------------------------------------------*/
EditorUI::EditorUI(Camera* aCamera, Background* aBackground)
{
	// �ϐ�
	mMouseX = 0;
	mMouseY = 0;
	mNowArea = 0;
	mIsPushAreaChangeButton[0] = false;
	mIsPushAreaChangeButton[1] = false;
	mDifficulty = 2;
	mBlend = 0;
	mBlendFlg = false;
	mEnemyButtonNum = 0;
	mEnemyNum = 0;
	mEditorButtonState[0] = 0;
	mEditorButtonAnime[0] = 0;
	mEditorButtonState[1] = 0;
	mEditorButtonAnime[1] = 0;
	mEditorButtonState[2] = 0;
	mEditorButtonAnime[2] = 0;
	mEnemyCount = 0;
	mSaveMeter = 0;

	// �I�u�W�F�N�g
	mCamera = aCamera;
	mBackground = aBackground;
}

/*------------------------------------------------------------------------------
�f�X�g���N�^
------------------------------------------------------------------------------*/
EditorUI::~EditorUI()
{

}

/*------------------------------------------------------------------------------
�X�V
------------------------------------------------------------------------------*/
void EditorUI::update()
{
	// ��ʓ����AUI�ɐG��Ă��Ȃ����̂݃}�E�X�ϐ��ɑ��
	if (Mouse::getIns()->getIsInScreen() && Mouse::getIns()->getXY(0) < 352) {
		mMouseX = Mouse::getIns()->getXY(0);
		mMouseY = Mouse::getIns()->getXY(1);
	}
	// ���݂̃G���A����
	mNowArea = mBackground->getArea();
	// �u�����h
	{
		if (mBlendFlg == false) {
			mBlend += 5;
		}
		else {
			mBlend -= 5;
		}
		if (mBlend > 255) {
			mBlendFlg = true;
		}
		if (mBlend < 0) {
			mBlendFlg = false;
		}
	}
	// �}�E�X�擾
	int mouse_x = Mouse::getIns()->getXY(0);
	int mouse_y = Mouse::getIns()->getXY(1);
	int mouse_c = 0;
	if (FadeEfect_GetState() == FadeState_Stop) {
		mouse_c = Mouse::getIns()->getClickLR(0);
	}
	// �G���A�؂�ւ����{�b�N�X
	{
		if (mouse_x >= 376 && mouse_x <= 390 && mouse_y >= 10 && mouse_y <= 24 && mouse_c == 1) {
			mBackground->addArea(-1);
			mIsPushAreaChangeButton[0] = true;
		}
		if (mouse_x >= 440 && mouse_x <= 454 && mouse_y >= 10 && mouse_y <= 24 && mouse_c == 1) {
			mBackground->addArea(1);
			mIsPushAreaChangeButton[1] = true;
		}
		if (mIsPushAreaChangeButton[0] && mouse_c == 0) {
			mIsPushAreaChangeButton[0] = false;
		}
		if (mIsPushAreaChangeButton[1] && mouse_c == 0) {
			mIsPushAreaChangeButton[1] = false;
		}
	}
	// ��Փx�������
	for (int i = 0; i < 5; i++) {
		if (mouse_x >= 376 + (i * 16) && mouse_x <= 391 + (i * 16) && mouse_y >= 30 && mouse_y <= 44 && mouse_c > 0) {
			mDifficulty = i;
		}
	}
	// �G�푰�I���{�^��
	{
		if (mEnemyButtonNum != 0 && mouse_x >= 373 && mouse_x <= 396 && mouse_y >= 80 && mouse_y <= 103 && mouse_c == 1) {
			mEnemyButtonNum = 0;
		}
		if (mEnemyButtonNum != 1 && mouse_x >= 404 && mouse_x <= 426 && mouse_y >= 80 && mouse_y <= 103 && mouse_c == 1) {
			mEnemyButtonNum = 1;
		}
		if (mEnemyButtonNum != 2 && mouse_x >= 435 && mouse_x <= 457 && mouse_y >= 80 && mouse_y <= 103 && mouse_c == 1) {
			mEnemyButtonNum = 2;
		}
		if (mEnemyButtonNum == 2 && mouse_x >= 435 && mouse_x <= 457 && mouse_y >= 80 && mouse_y <= 103 && mouse_c >= 100) {
			mEnemyButtonNum = 3;
		}
	}
	// �G�I��
	{
		switch (mEnemyButtonNum) {
		case 0: // 16��
			for (int i = 0; i < 5; i++) {
				for (int j = 0; j < 4; j++) {
					if (mouse_x >= 363 + (i * 16) + (i * 6) &&
						mouse_x <= 378 + (i * 16) + (i * 6) &&
						mouse_y >= 115 + (j * 16) + (j * 6) &&
						mouse_y <= 130 + (j * 16) + (j * 6) &&
						mouse_c == 1) {
						mEnemyNum = i + (j * 5);
						// �S�������␳
						if (mEnemyNum == 14) {
							mEnemyNum = 20;
						} 
						else if(mEnemyNum == 15) {
							mEnemyNum = 21;
						}
						else if (mEnemyNum == 16) {
							mEnemyNum = 22;
						}
						else if (mEnemyNum == 17) {
							mEnemyNum = 23;
						}
					}
				}
			}
			break;

		case 1: // 32��
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 2; j++) {
					if (mouse_x >= 375 + (i * 32) + (i * 12) &&
						mouse_x <= 406 + (i * 32) + (i * 12) &&
						mouse_y >= 120 + (j * 32) + (j * 12) &&
						mouse_y <= 151 + (j * 32) + (j * 12) &&
						mouse_c == 1 &&
						mEnemyNum != ENEMY_START_1 + (i + (j * 2)) && i + (j * 2) < ENEMY_VOLUME_1) {
						mEnemyNum = ENEMY_START_1 + (i + (j * 2));
					}
				}
			}
			break;

		case 2: // �{�X
			if (mouse_x >= 370 && mouse_x <= 465 && mouse_y >= 113 && mouse_y <= 208 && mouse_c == 1 &&
				mEnemyNum != ENEMY_START_2) {
				mEnemyNum = ENEMY_END_2;
			}
			break;

		case 3: // �A�C�e��
			if (mouse_x >= 405 && mouse_x <= 420 && mouse_y >= 150 && mouse_y <= 165 && mouse_c == 1 &&
				mEnemyNum != ENEMY_START_3) {
				mEnemyNum = ENEMY_END_3;
			}
			break;

		}
	}
	// �G�f�B�^�[�{�^��
	{
		// ������
		for (int i = 0; i < 3; i++) {
			if (mEditorButtonState[i] != 2) {
				mEditorButtonState[i] = 0;
			}
		}
		// �G�ꂽ����鏈��
		if (mEditorButtonState[0] == 0 && mouse_x >= 356 && mouse_x <= 393 && mouse_y >= 240 && mouse_y <= 262) {
			mEditorButtonState[0] = 1;
		}
		if (mEditorButtonState[1] == 0 && mouse_x >= 397 && mouse_x <= 434 && mouse_y >= 240 && mouse_y <= 262) {
			mEditorButtonState[1] = 1;
		}
		if (mEditorButtonState[2] == 0 && mouse_x >= 438 && mouse_x <= 475 && mouse_y >= 240 && mouse_y <= 262) {
			mEditorButtonState[2] = 1;
		}
		// �A�j��
		for (int i = 0; i < 3; i++) {
			// �����Ă��Ȃ�������A�j���[�V����������
			if (mEditorButtonState[i] == 0 && mEditorButtonAnime[i] != 0) {
				mEditorButtonAnime[i] = 0;
			}
			// �����Ă��鎞�̓A�j���[�V����
			if (mEditorButtonState[i] == 1) {
				mEditorButtonAnime[i] += (float)0.1;
				if (mEditorButtonAnime[i] >= 3) {
					mEditorButtonAnime[i] = 0;
				}
			}
		}
		// �����Ă��鎞�ɃN���b�N������
		if (mEditorButtonState[0] == 1 && mEditorButtonState[1] != 2 && mEditorButtonState[2] != 2 && mouse_c == 1) {
			SE::getIns()->playSE(SE_Credit);
			mEditorButtonState[0] = 2;
		}
		if (mEditorButtonState[1] == 1 && mEditorButtonState[0] != 2 && mEditorButtonState[2] != 2 && mouse_c > 0) {
			mSaveMeter += (float)38 / 40;
			if (mSaveMeter > 38) {
				mSaveMeter = 38;
			}
		}
		if (mSaveMeter > 0 && mSaveMeter < 38 && mouse_c == 0) {
			mSaveMeter = 0;
		}
		if (mSaveMeter >= 38) {
			SE::getIns()->playSE(SE_Credit);
			mEditorButtonState[1] = 2;
		}
		if (mEditorButtonState[2] == 1 && mEditorButtonState[0] != 2 && mEditorButtonState[1] != 2 && mouse_c == 1) {
			SE::getIns()->playSE(SE_Credit);
			mEditorButtonState[2] = 2;
		}
	}
}

/*------------------------------------------------------------------------------
�`��
------------------------------------------------------------------------------*/
void EditorUI::draw() const
{
	// BOX
	DrawBox(352, 0, 480, 270, GetColor(0, 0, 0), TRUE);
	// �G���A���̕\��
	Xprintf_Position(2);
	Xprintf_Color(1);
	Xprintf("AREA", 416, 5);
	Xprintf_Color(0);
	XNprintf(mNowArea + 1, 416, 18);
	// �G���A�؂�ւ����{�b�N�X
	{
		// �����
		if (mIsPushAreaChangeButton[0]) {
			DrawGraph(408 - 32, 10, Image::getIns()->getImage(Image_AreaChangeButton, 1), TRUE);
		}
		else {
			DrawGraph(408 - 32, 10, Image::getIns()->getImage(Image_AreaChangeButton, 0), TRUE);
		}
		// �E���
		if (mIsPushAreaChangeButton[1]) {
			DrawGraph(408 + 32, 10, Image::getIns()->getImage(Image_AreaChangeButton, 3), TRUE);
		}
		else {
			DrawGraph(408 + 32, 10, Image::getIns()->getImage(Image_AreaChangeButton, 2), TRUE);
		}
	}
	// ��Փx���
	for (int i = 0; i < 5; i++) {
		if (i <= mDifficulty) {
			DrawGraph(376 + (i * 16), 30, Image::getIns()->getImage(Image_DifficultyMeter, i), TRUE);
			SetDrawBlendMode(DX_BLENDMODE_ADD, mBlend); // �u�����h���[�h�����Z(255/255)�ɐݒ�
			DrawGraph(376 + (i * 16), 30, Image::getIns()->getImage(Image_DifficultyMeter, i + 10), TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);  // �u�����h���[�h���I�t
		}
		else {
			DrawGraph(376 + (i * 16), 30, Image::getIns()->getImage(Image_DifficultyMeter, i + 5), TRUE);
		}
	}
	// xy���W
	Xprintf_Position(2);
	Xprintf_Color(1);
	Xprintf("~~~~~~~~~~~~", 416, 49);
	Xprintf("~~~~~~~~~~~~", 416, 74);
	Xprintf_Position(0);
	Xprintf("X : ", 376, 54);
	Xprintf("Y : ", 376, 63);
	Xprintf_Position(1);
	XNprintf(mMouseX / 4 * 4, 456, 54);
	XNprintf((mMouseY / 4 * 4)+ (int)mCamera->getY(), 456, 63);
	// �푰�I���{�^��
	{
		// 16��
		if (mEnemyButtonNum == 0) {
			DrawGraph(373, 81, Image::getIns()->getImage(Image_EnemyButton, 1), TRUE);
		}
		else {
			DrawGraph(373, 81, Image::getIns()->getImage(Image_EnemyButton, 0), TRUE);
		}
		// 32��
		if (mEnemyButtonNum == 1) {
			DrawGraph(404, 81, Image::getIns()->getImage(Image_EnemyButton, 3), TRUE);
		}
		else {
			DrawGraph(404, 81, Image::getIns()->getImage(Image_EnemyButton, 2), TRUE);
		}
		// �{�X��
		if (mEnemyButtonNum == 2) {
			DrawGraph(434, 81, Image::getIns()->getImage(Image_EnemyButton, 5), TRUE);
		}
		else if (mEnemyButtonNum == 3) {
			DrawGraph(434, 81, Image::getIns()->getImage(Image_EnemyButton, 6), TRUE);
		}
		else {
			DrawGraph(434, 81, Image::getIns()->getImage(Image_EnemyButton, 4), TRUE);
		}
	}
	// �G�I��BOX
	{
		int num = 0;
		int widSel = 0;
		int heiSel = 0;
		int mouse_x = Mouse::getIns()->getXY(0);
		int mouse_y = Mouse::getIns()->getXY(1);
		int tmpEneNum;
		DrawGraph(355, 110, Image::getIns()->getImage(Image_EnemyBox), TRUE);
		switch (mEnemyButtonNum) {
		case 0: // 16��
			widSel = 0, heiSel = 0;
			// �A�C�R��
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 5; j++) {
					if (num < 18) {
						DrawGraph(363 + (j * 22), 115 + (i * 22), Image::getIns()->getImage(Image_EnemyIcon, num), TRUE);
					}
					if (num < 20) {
						num++;
					}
				}
			}
			// �e�����艟��
			tmpEneNum = mEnemyNum;
			if (mEnemyNum >= 20) {
				tmpEneNum -= 6;
			}
			widSel = tmpEneNum % 5;
			heiSel = tmpEneNum / 5;
			// �I���J�[�\��
			if (mEnemyNum >= 0 && mEnemyNum <= ENEMY_END_0 || mEnemyNum >= 20) {
				DrawGraph(363 + (widSel * 22), 115 + (heiSel * 22), Image::getIns()->getImage(Image_Select), TRUE);
			}
			for (int i = 0; i < 5; i++) {
				for (int j = 0; j < 4; j++) {
					if (mouse_x >= 363 + (i * 16) + (i * 6) &&
						mouse_x <= 378 + (i * 16) + (i * 6) &&
						mouse_y >= 115 + (j * 16) + (j * 6) &&
						mouse_y <= 130 + (j * 16) + (j * 6) &&
						mEnemyNum != i + (j * 5) && i + (j * 5) < 18) {
						DrawGraph(363 + (i * 16) + (i * 6), 115 + (j * 16) + (j * 6), Image::getIns()->getImage(Image_Aiming, 2), TRUE);
					}
				}
			}
			break;

		case 1: // 32��
			widSel = 0, heiSel = 0;
			// �A�C�R��
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 2; j++) {
					DrawGraph(375 + (j * 44), 120 + (i * 44), Image::getIns()->getImage(Image_EnemyBigIcon, num), TRUE);
					if (num < ENEMY_VOLUME_1) {
						num++;
					}
				}
			}
			widSel = (mEnemyNum - 14) % 2;
			heiSel = (mEnemyNum - 14) / 2;
			// �I���J�[�\��
			if (mEnemyNum >= ENEMY_START_1 && mEnemyNum <= ENEMY_END_1) {
				DrawRotaGraph(391 + (widSel * 44), 136 + (heiSel * 44), 2, 0, Image::getIns()->getImage(Image_Select), TRUE);
			}
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 2; j++) {
					if (mouse_x >= 375 + (i * 32) + (i * 12) &&
						mouse_x <= 406 + (i * 32) + (i * 12) &&
						mouse_y >= 120 + (j * 32) + (j * 12) &&
						mouse_y <= 151 + (j * 32) + (j * 12) &&
						mEnemyNum != ENEMY_START_1 + (i + (j * 2)) && i + (j * 2) < ENEMY_VOLUME_1) {
						DrawRotaGraph(391 + (i * 32) + (i * 12), 136 + (j * 32) + (j * 12), 2, 0, Image::getIns()->getImage(Image_Aiming, 2), TRUE);
					}
				}
			}
			break;

		case 2: // �{�X
			// �A�C�R��
			DrawGraph(370, 113, Image::getIns()->getImage(Image_BossIcon), TRUE);
			// �I���J�[�\��
			if (mEnemyNum == ENEMY_START_2) {
				DrawRotaGraph(418, 161, 6, 0, Image::getIns()->getImage(Image_Select), TRUE);
			}
			if (mouse_x >= 370 && mouse_x <= 465 && mouse_y >= 113 && mouse_y <= 208 &&
				mEnemyNum != ENEMY_START_2) {
				DrawRotaGraph(418, 161, 6, 0, Image::getIns()->getImage(Image_Aiming, 2), TRUE);
			}
			break;

		case 3: // �A�C�e��
			// �A�C�R��
			DrawGraph(405, 150, Image::getIns()->getImage(Image_SpecialFlag), TRUE);
			// �I���J�[�\��
			if (mEnemyNum == ENEMY_START_3) {
				DrawGraph(405, 150, Image::getIns()->getImage(Image_Select), TRUE);
			}
			if (mouse_x >= 405 && mouse_x <= 420 && mouse_y >= 150 && mouse_y <= 165 &&
				mEnemyNum != ENEMY_START_3) {
				DrawGraph(405, 150, Image::getIns()->getImage(Image_Aiming, 2), TRUE);
			}
			break;
		}
	}
	// �G�̖��O
	drawEnemyName();
	// �z�u�ł���c�萔
	Xprintf_Position(1);
	Xprintf_Color(1);
	Xprintf("/ 256", 452, 228);
	Xprintf_Position(2);
	XNprintf(mEnemyCount, 392, 228);
	// �G�f�B�^�[�{�^��
	switch (mEditorButtonState[0]) {
	case 0:
		DrawGraph(356, 240, Image::getIns()->getImage(Image_EditorButton, 0), TRUE);
		break;
	case 1:
		DrawGraph(356, 240, Image::getIns()->getImage(Image_EditorButton, 1 + (int)mEditorButtonAnime[0]), TRUE);
		break;
	case 2:
		DrawGraph(356, 240, Image::getIns()->getImage(Image_EditorButton, 4), TRUE);
		break;
	}
	switch (mEditorButtonState[1]) {
	case 0:
		DrawGraph(397, 240, Image::getIns()->getImage(Image_EditorButton, 5), TRUE);
		break;
	case 1:
		DrawGraph(397, 240, Image::getIns()->getImage(Image_EditorButton, 6 + (int)mEditorButtonAnime[1]), TRUE);
		break;
	case 2:
		DrawGraph(397, 240, Image::getIns()->getImage(Image_EditorButton, 9), TRUE);
		break;
	}
	switch (mEditorButtonState[2]) {
	case 0:
		DrawGraph(438, 240, Image::getIns()->getImage(Image_EditorButton, 10), TRUE);
		break;
	case 1:
		DrawGraph(438, 240, Image::getIns()->getImage(Image_EditorButton, 11 + (int)mEditorButtonAnime[2]), TRUE);
		break;
	case 2:
		DrawGraph(438, 240, Image::getIns()->getImage(Image_EditorButton, 14), TRUE);
		break;
	}
	if (mSaveMeter < 38) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
		DrawBox(397, 240, 397 + (int)mSaveMeter, 240 + 23, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

/*------------------------------------------------------------------------------
�G�̖��O�\��
------------------------------------------------------------------------------*/
void EditorUI::drawEnemyName() const
{
	Xprintf_Position(2);
	Xprintf_Color(0);
	switch (mEnemyNum) {
	case 0:
		Xprintf("TERRAZI", 416, 215);
		break;
	case 1:
		Xprintf("ZOLBAK", 416, 215);
		break;
	case 2:
		Xprintf("TOROID", 416, 215);
		break;
	case 3:
		Xprintf("TORKEN", 416, 215);
		break;
	case 4:
		Xprintf("BARRA", 416, 215);
		break;
	case 5:
		Xprintf("KAPI", 416, 215);
		break;
	case 6:
		Xprintf("DEROTA", 416, 215);
		break;
	case 7:
		Xprintf("ZOSHI", 416, 215);
		break;
	case 8:
		Xprintf("LOGRAM", 416, 215);
		break;
	case 9:
		Xprintf("SHEONITE", 416, 215);
		break;
	case 10:
		Xprintf("BOZALOGRAM", 416, 215);
		break;
	case 11:
		Xprintf("DOMOGRAM", 416, 215);
		break;
	case 12:
		Xprintf("GROBDA", 416, 215);
		break;
	case 13:
		Xprintf("JARA", 416, 215);
		break;
	case 14:
		Xprintf("GARU DEROTA", 416, 215);
		break;
	case 15:
		Xprintf("GARU BARRA", 416, 215);
		break;
	case 16:
		Xprintf("SOL", 416, 215);
		break;
	case 17:
		Xprintf("BACURA", 416, 215);
		break;
	case 18:
		Xprintf("ADDOR GUILENESS", 416, 215);
		break;
	case 19:
		Xprintf("SPECIAL FLAG", 416, 215);
		break;
	case 20:
		Xprintf("ZAKATO", 416, 215);
		break;
	case 21:
		Xprintf("GIDO SPARIO", 416, 215);
		break;
	case 22:
		Xprintf("BRAG ZAKATO", 416, 215);
		break;
	case 23:
		Xprintf("GARU ZAKATO", 416, 215);
		break;
	}
}

/*------------------------------------------------------------------------------
���ݑI�����Ă���G�ԍ���Ԃ�
------------------------------------------------------------------------------*/
int EditorUI::getEnemyNum()
{
	return mEnemyNum;
}

/*------------------------------------------------------------------------------
���݂̓G�̐ݒu�����Z�b�g
------------------------------------------------------------------------------*/
void EditorUI::setEnemyCount(int aCount)
{
	mEnemyCount = aCount;
}

/*------------------------------------------------------------------------------
�G�f�B�^�[�{�^���̏�Ԃ�Ԃ�
------------------------------------------------------------------------------*/
int EditorUI::getEditorButtonState(int a)
{
	return mEditorButtonState[a];
}

/*------------------------------------------------------------------------------
�G�f�B�^�[�{�^���̏�Ԃ�Ԃ�
------------------------------------------------------------------------------*/
int EditorUI::getDifficulty()
{
	return mDifficulty;
}

/*------------------------------------------------------------------------------
�G�f�B�^�[�{�^���̏�Ԃ�Ԃ�
------------------------------------------------------------------------------*/
void EditorUI::resetEditorButton()
{
	mEditorButtonState[0] = 0;
	mEditorButtonState[1] = 0;
	mSaveMeter = 0;
}
