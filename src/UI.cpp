#pragma warning(disable:26812)

#include <Dxlib.h>
#include "UI.h"
#include "Xprintf.h"

/*------------------------------------------------------------------------------
�R���X�g���N�^
------------------------------------------------------------------------------*/
UI::UI(SceneList aNowScene)
{
	mNowScene = aNowScene;
	mHighScore = 40000;
	mScore = 0;
	mLeft = 2;
	mArea = 0;
	mDifficulty = 2;
	mSkyNPC = 0;
	mGroundNPC = 0;
}

/*------------------------------------------------------------------------------
�f�X�g���N�^
------------------------------------------------------------------------------*/
UI::~UI()
{
}

/*------------------------------------------------------------------------------
�`��
------------------------------------------------------------------------------*/
void UI::draw() const
{
	switch (mNowScene) {
	case SceneList::Title: // �^�C�g����ʂ̎�
		// BOX
		DrawBox(352, 0, 480, 270, GetColor(0, 0, 0), TRUE);
		// �I�����W�̕���
		Xprintf_Color(1);
		Xprintf_Position(0);
		Xprintf("HIGH SCORE", 376, 24);
		Xprintf("~~~~~~~~~~~~", 368, 64);
		Xprintf("[    ]", 376, 80);
		Xprintf("[    ]", 376, 80 + 2 * 16);
		Xprintf("[    ]", 376, 80 + 4 * 16);
		Xprintf("[    ]", 376, 80 + 6 * 16);
		Xprintf("ESC", 388, 80 + 6 * 16);
		Xprintf("~~~~~~~~~~~~", 368, 216);
		// �������l
		Xprintf_Color(0);
		Xprintf_Position(1);
		XNprintf(mHighScore, 456, 24 + 16);
		Xprintf_Position(0);
		Xprintf("<{}>", 384, 80);
		Xprintf("Z", 396, 80 + 2 * 16);
		Xprintf("X", 396, 80 + 4 * 16);
		// ����
		Xprintf_Position(1);
		Xprintf("MOVE", 456, 96);
		Xprintf("ZAPPER", 456, 96 + 2 * 16);
		Xprintf("BLASTER", 456, 96 + 4 * 16);
		Xprintf("END GAME", 456, 96 + 6 * 16);
		break;
	case SceneList::Game: // �Q�[����ʂ̎�
		// BOX
		DrawBox(352, 0, 480, 270, GetColor(0, 0, 0), TRUE);
		// �I�����W�̕���
		Xprintf_Color(1);
		Xprintf_Position(0);
		Xprintf("HIGH SCORE", 376, 24);
		Xprintf("SCORE", 376, 56);
		Xprintf("~~~~~~~~~~~~", 368, 96);
		Xprintf("SOLVALOU", 376, 120);
		Xprintf("AREA", 376, 248);
		// �������l
		Xprintf_Color(0);
		Xprintf_Position(1);
		XNprintf(mHighScore, 456, 24 + 16);
		XNprintf(mScore, 456, 56 + 16);
		for (int i = 0; i < mLeft; i++) {
			Xprintf("#", 456 - (i * 8), 120 + 16);
		}
		XNprintf(mArea, 456, 248);
		break;
	case SceneList::Editor:
		// BOX
		DrawBox(352, 0, 480, 270, GetColor(0, 0, 0), TRUE);
		// �I�����W�̕���
		Xprintf_Color(1);
		Xprintf_Position(0);
		Xprintf("DIFFICULTY", 376, 24);
		Xprintf("SCORE", 376, 56);
		Xprintf("~~~~~~~~~~~~", 368, 96);
		Xprintf("SKY NPC", 376, 120);
		Xprintf("GROUND NPC", 376, 152);
		Xprintf("AREA", 376, 248);
		// �������l
		Xprintf_Color(0);
		Xprintf_Position(1);
		XNprintf(mDifficulty, 456, 24 + 16);
		XNprintf(mScore, 456, 56 + 16);
		XNprintf(mSkyNPC, 456, 136);
		XNprintf(mGroundNPC, 456, 136 + 32);
		XNprintf(mArea, 456, 248);
		Xprintf_Position(2);
		Xprintf("[ SPACE ] : END", 176, 270 - 16);
		break;
	default:
		// BOX
		DrawBox(352, 0, 480, 270, GetColor(0, 0, 0), TRUE);
		// �I�����W�̕���
		Xprintf_Color(1);
		Xprintf_Position(0);
		Xprintf("ERROR", 376, 24);
		break;
	}
}

/*------------------------------------------------------------------------------
���݂̃n�C�X�R�A���Z�b�g
------------------------------------------------------------------------------*/
void UI::setHighScore(int aHighScore)
{
	mHighScore = aHighScore;
}

/*------------------------------------------------------------------------------
���݂̃X�R�A���Z�b�g
------------------------------------------------------------------------------*/
void UI::setScore(int aScore)
{
	mScore = aScore;
}

/*------------------------------------------------------------------------------
���݂̎c�@�����Z�b�g
------------------------------------------------------------------------------*/
void UI::setLeft(int aLeft)
{
	mLeft = aLeft;
}

/*------------------------------------------------------------------------------
���݂̃G���A���Z�b�g
------------------------------------------------------------------------------*/
void UI::setArea(int aArea)
{
	mArea = aArea;
}

/*------------------------------------------------------------------------------
���݂̓�Փx���Z�b�g
------------------------------------------------------------------------------*/
void UI::setDifficulty(int aDifficulty)
{
	mDifficulty = aDifficulty;
}

/*------------------------------------------------------------------------------
���݂̋��NPC�̐����Z�b�g
------------------------------------------------------------------------------*/
void UI::setSkyNPC(int aSkyNPC)
{
	mSkyNPC = aSkyNPC;
}

/*------------------------------------------------------------------------------
���݂̒n���NPC�̐����Z�b�g
------------------------------------------------------------------------------*/
void UI::setGroundNPC(int aGroundNPC)
{
	mGroundNPC = aGroundNPC;
}