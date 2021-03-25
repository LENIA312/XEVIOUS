#include <DxLib.h>
#include "NPCManager.h"
#include "Define.h"

// �e�G�̃w�b�_�[�t�@�C��-------------------------------------------------------
// �������G------
#include "Terrazi.h"
#include "ZolBak.h"
#include "Toroid.h"
#include "Torken.h"
#include "Barra.h"
#include "Kapi.h"
#include "Derota.h"
#include "Zoshi.h"
#include "Logram.h"
#include "Sheonite.h"
#include "Bozalogram.h"
#include "Domogram.h"
#include "Grobda.h"
#include "Jara.h"
// �傫���G------
#include "GaruDerota.h"
#include "GaruBarra.h"
#include "Sol.h"
#include "Bacura.h"
// �{�X--------
#include "AddorGuileness.h"
// �A�C�e��----
#include "SpecialFlag.h"
// �G�̒e------
#include "Spario.h"
#include "Zakato.h"
#include "GidoSpario.h"
#include "BragZakato.h"
#include "GaruZakato.h"
#include "BragSpario.h"

//------------------------------------------------------------------------------

using namespace std;

/*------------------------------------------------------------------------------
�R���X�g���N�^
------------------------------------------------------------------------------*/
NPCManager::NPCManager(Player* aPlayer)
{
	// �ϐ�
	mDifficulty = 2;
	mNowArea = 0;
	mTortalScore = 0;
	mAddLeft = 0;
	mColor = 4;
	mColorTimer = 0;
	mIsTestPlay = false;

	// �I�u�W�F�N�g
	mPlayer = aPlayer;
	mZapper = aPlayer->getZapper();
	mBlaster = aPlayer->getBlaster();

	// �t�@�C���ǂݍ���
	readNPCFile();
}

/*------------------------------------------------------------------------------
�f�X�g���N�^
------------------------------------------------------------------------------*/
NPCManager::~NPCManager()
{
}

/*------------------------------------------------------------------------------
�X�V
------------------------------------------------------------------------------*/
void NPCManager::update()
{
	// ����
	generateNPC();
	// �F�̕ω��A�j��
	colorAnime();
	// �v���C���[�̍��W
	int px = mPlayer->getCollisionPoint(0);
	int py = mPlayer->getCollisionPoint(1);
	int pw = mPlayer->getCollisionPoint(2);
	int ph = mPlayer->getCollisionPoint(3);
	// �Ə��`�J�`�J���Z�b�g
	mPlayer->setTikaTikaFlg(false);
	// NPC�X�V
	for (int i = 0; i < 2; i++) {
		for (const auto npc : mNPCList[i]) {
			// �J�����Z�b�g
			npc->setCameraY(mCameraY);
			// �G���A�̕ς��ڂ�y���W�̈ʒu���ړ�
			int ey = npc->getCollisionPoint(1) - 16;
			if (ey <= Define::GameScreenHeight && mCameraY > Define::MapHeight - Define::GameScreenHeight) {
				npc->areaChange();
			}
			// �J���[�Z�b�g
			npc->setColor(mColor);
			// �X�V
			npc->update();
		}
		// �A�N�e�B�u�t���O���܂��Ă����烊�X�g����G�f�[�^������
		for (auto npc = mNPCList[i].begin(); npc != mNPCList[i].end();) {
			if ((*npc)->checkActiveFlag() == false) {
				npc = mNPCList[i].erase(npc);
			}
			else {
				npc++;
			}
		}
	}
	// �v���C���[�Ƃ̓����蔻�菈��
	if (!mIsTestPlay) {
		collisionPlayer();
	}
}

/*------------------------------------------------------------------------------
�`��
------------------------------------------------------------------------------*/
void NPCManager::draw() const
{
	for (int i = 0; i < 2; i++) {
		for (const auto npc : mNPCList[i]) {
			npc->draw();
		}
	}
	// �f�o�b�O	
	if (mIsTestPlay) {
		drawCollision();
	}
	//DrawFormatString(0, 0, GetColor(255, 255, 255), "Camera:%d Sky:%d Ground:%d", (int)mCameraY, mNPCList[1].size(), mNPCList[0].size());
}

/*------------------------------------------------------------------------------
�����蔻��̕`��
------------------------------------------------------------------------------*/
void NPCManager::drawCollision() const
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);

	// NPC
	for (int i = 0; i < 2; i++) {
		for (const auto npc : mNPCList[i]) {
			npc->drawCollision(255,0,0);
		}
	}
	// �v���C���[
	mPlayer->drawCollision(0,255,255);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

/*------------------------------------------------------------------------------
�v���C���[�Ƃ̓����蔻�菈��
------------------------------------------------------------------------------*/
void NPCManager::collisionPlayer()
{
	// �v���C���[�̍��W
	int px = mPlayer->getCollisionPoint(0);
	int py = mPlayer->getCollisionPoint(1);
	int pw = mPlayer->getCollisionPoint(2);
	int ph = mPlayer->getCollisionPoint(3);
	// ���X�g
	for (auto npc : mNPCList[1]) {
		if (!npc->getIsStriken()) {
			int ex = npc->getCollisionPoint(0);
			int ey = npc->getCollisionPoint(1);
			int ew = npc->getCollisionPoint(2);
			int eh = npc->getCollisionPoint(3);
			// �v���C���[���G�ɓ���������
			if (px + pw >= ex && px <= ex + ew &&
				py + ph >= ey && py <= ey + eh && mPlayer->checkActiveFlag()) {
				mPlayer->collisionGameObject(npc->getTag());
			}
		}
	}
}

/*------------------------------------------------------------------------------
����
------------------------------------------------------------------------------*/
void NPCManager::generateNPC()
{
	int d = mDifficulty;
	int a = mNowArea;
	// �t�@�C���̍ő吔�����[�v
	for (int i = 0; i < mNPCDataMax[d][a]; i++) {
		// �܂���������Ă��Ȃ��ăJ�����̒��ɓ�������
		if (!mNPCData[d][a][i].flag && mNPCData[d][a][i].y >= mCameraY) {
			// NPC�𐶐�����
			switch (mNPCData[d][a][i].type) {
			case  0: // �e���W
				mNPCList[1].emplace_back(make_shared<Terrazi>(this, mNPCData[d][a][i], mPlayer));
				break;
			case  1: // �]���o�N
				mNPCList[0].emplace_back(make_shared<Zolbak>(this, mNPCData[d][a][i], mPlayer));
				break;
			case  2: // �g�[���C�h
				mNPCList[1].emplace_back(make_shared<Toroid>(this, mNPCData[d][a][i], mPlayer));
				break;
			case  3: // �^���P��
				mNPCList[1].emplace_back(make_shared<Torken>(this, mNPCData[d][a][i], mPlayer));
				break;
			case  4: // �o�[��
				mNPCList[0].emplace_back(make_shared<Barra>(this, mNPCData[d][a][i], mPlayer));
				break;
			case  5: //	�J�s
				mNPCList[1].emplace_back(make_shared<Kapi>(this, mNPCData[d][a][i], mPlayer));
				break;
			case  6: // �f���[�^
				mNPCList[0].emplace_back(make_shared<Derota>(this, mNPCData[d][a][i], mPlayer));
				break;
			case  7: // �]�V�[
				mNPCList[1].emplace_back(make_shared<Zoshi>(this, mNPCData[d][a][i], mPlayer));
				break;
			case  8: // ���O����
				mNPCList[0].emplace_back(make_shared<Logram>(this, mNPCData[d][a][i], mPlayer));
				break;
			case  9: // �V�I�i�C�g
				mNPCList[1].emplace_back(make_shared<Sheonite>(this, mNPCData[d][a][i], mPlayer));
				break;
			case 10: // �{�U���O����
				mNPCList[0].emplace_back(make_shared<Bozalogram>(this, mNPCData[d][a][i], mPlayer));
				break;
			case 11: // �h���O����
				mNPCList[0].emplace_back(make_shared<Domogram>(this, mNPCData[d][a][i], mPlayer));
				break;
			case 12: // �O���u�_�[
				mNPCList[0].emplace_back(make_shared<Grobda>(this, mNPCData[d][a][i], mPlayer));
				break;
			case 13: // �W�A��
				mNPCList[1].emplace_back(make_shared<Jara>(this, mNPCData[d][a][i], mPlayer));
				break;
			case 14: // �K���f���[�^
				mNPCList[0].emplace_back(make_shared<GaruDerota>(this, mNPCData[d][a][i], mPlayer));
				break;
			case 15: // �K���o�[��
				mNPCList[0].emplace_back(make_shared<GaruBarra>(this, mNPCData[d][a][i], mPlayer));
				break;
			case 16: // �\��
				mNPCList[0].emplace_back(make_shared<Sol>(this, mNPCData[d][a][i], mPlayer));
				break;
			case 17: // �o�L����
				mNPCList[1].emplace_back(make_shared<Bacura>(this, mNPCData[d][a][i], mPlayer));
				break;
			case 18: // �A���h�A�W�F�l�V�X
				mNPCList[0].emplace_back(make_shared<AddorGuileness>(this, mNPCData[d][a][i], mPlayer));
				break;
			case 19: // �X�y�V�����t���b�O
				mNPCList[0].emplace_back(make_shared<SpecialFlag>(this, mNPCData[d][a][i], mPlayer));
				break;
			case 20: // �U�J�[�g
				mNPCList[1].emplace_back(make_shared<Zakato>(this, mNPCData[d][a][i], mPlayer));
				break;
			case 21: // �M�h�X�p���I
				mNPCList[1].emplace_back(make_shared<GidoSpario>(this, mNPCData[d][a][i], mPlayer));
				break;
			case 22: // �u���O�U�J�[�g
				mNPCList[1].emplace_back(make_shared<BragZakato>(this, mNPCData[d][a][i], mPlayer));
				break;
			case 23: // �K���U�J�[�g
				mNPCList[1].emplace_back(make_shared<GaruZakato>(this, mNPCData[d][a][i], mPlayer));
				break;
			}
			// ������͐�������Ȃ��悤�Ƀt���O�𗧂Ă�
			mNPCData[d][a][i].flag = true;
		}
	}
}

/*------------------------------------------------------------------------------
�G�̒e�𐶐�
------------------------------------------------------------------------------*/
void NPCManager::generateEnemyBullet(int aType, int aPtn, int aX, int aY)
{
	NPCDATA data;
	data.ptn = aPtn;
	data.x = aX;
	data.y = aY;
	switch (aType) {
	case 0: // �X�p���I
		mNPCList[1].emplace_back(make_shared<Spario>(this, data, mPlayer));
		break;
	case 1: // �u���O�U�J�[�g�X�p���I
		for (int i = 0; i < 5; i++) {
			data.ptn = i + 1;
			mNPCList[1].emplace_back(make_shared<Spario>(this, data, mPlayer));
		}
		break;
	case 2: // �K���U�J�[�g
		for (int i = 0; i < 16; i++) {
			// �X�p���I
			data.ptn = i + 6;
			mNPCList[1].emplace_back(make_shared<Spario>(this, data, mPlayer));
			if (i < 4) {
				data.ptn = i;
				mNPCList[1].emplace_back(make_shared<BragSpario>(this, data, mPlayer));
			}
		}
		break;
	}
}

/*------------------------------------------------------------------------------
NPC�̃t�@�C����ǂݍ���
------------------------------------------------------------------------------*/
void NPCManager::readNPCFile()
{
	// �ϐ��錾
	int num;
	int area, areaN;
	int input[64];
	char inputc[64];
	int fp[5];
	fp[0] = FileRead_open("res/file/NPCData_0.csv");
	fp[1] = FileRead_open("res/file/NPCData_1.csv");
	fp[2] = FileRead_open("res/file/NPCData_2.csv");
	fp[3] = FileRead_open("res/file/NPCData_3.csv");
	fp[4] = FileRead_open("res/file/NPCData_4.csv");
	// 5��t�@�C���ǂݍ���
	for (int i = 0; i < 5; i++) {
		num = 0;
		area = 0;
		areaN = 0;
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
					mNPCDataMax[i][area] = areaN;
					goto EXFILE;
				}
				// �X���b�V��������Ύ��̃G���A��
				if (inputc[j] == '/') {
					mNPCDataMax[i][area] = areaN;
					area++;
					while (FileRead_getc(fp[i]) != '\n');
					//�J�E���^���ŏ��ɖ߂�
					j = -1;
					areaN = 0;
					continue;
				}
			}
			switch (num) {
			case 0: // �G�̎��
				mNPCData[i][area][areaN].type = atoi(inputc);
				break;
			case 1: // �s���p�^�[��
				mNPCData[i][area][areaN].ptn = atoi(inputc);
				break;
			case 2: // x���W
				mNPCData[i][area][areaN].x = atoi(inputc);
				break;
			case 3: // y���W
				mNPCData[i][area][areaN].y = atoi(inputc);
				break;
			}
			// �o���������ǂ����m�F����t���O
			mNPCData[i][area][areaN].flag = false;
			num++;
			if (num == 4) {
				num = 0;
				areaN++;
			}
		}
		EXFILE:
		FileRead_close(fp[i]);
	}
}

/*------------------------------------------------------------------------------
�F�̕ω��A�j��
------------------------------------------------------------------------------*/
void NPCManager::colorAnime()
{
	mColorTimer++;
	if (mColorTimer <= 20 && mColorTimer % 5 == 0) {
		mColor--;
	}
	if (mColorTimer > 20 && mColorTimer <= 40 && mColorTimer % 5 == 0) {
		mColor++;
	}
	if (mColorTimer > 70) {
		mColorTimer = 0;
	}
}

/*------------------------------------------------------------------------------
���X�g�̒��g����ɂ���
------------------------------------------------------------------------------*/
void NPCManager::clearList()
{
	for (int i = 0; i < 2; i++) {
		for (auto npc = mNPCList[i].begin(); npc != mNPCList[i].end();) {
			if ((*npc)->checkActiveFlag() == true) {
				npc = mNPCList[i].erase(npc);
			}
			else {
				npc++;
			}
		}
	}
	// ���ׂĂ̓G�̏o���t���O���I�t��
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 16; j++) {
			for (int k = 0; k < mNPCDataMax[i][j]; k++) {
				mNPCData[i][j][k].flag = false;
			}
		}
	}
}

/*------------------------------------------------------------------------------
�J�����̍��W���Z�b�g
------------------------------------------------------------------------------*/
void NPCManager::setCamera(float aY)
{
	mCameraY = aY;
}

/*------------------------------------------------------------------------------
���݂̃G���A���Z�b�g
------------------------------------------------------------------------------*/
void NPCManager::setNowArea(int aNowArea)
{
	mNowArea = aNowArea;
}

/*------------------------------------------------------------------------------
���݂̓�Փx���Z�b�g
------------------------------------------------------------------------------*/
void NPCManager::setDifficulty(int aDifficulty)
{
	mDifficulty = aDifficulty;
}

/*------------------------------------------------------------------------------
�X�R�A�����Z
------------------------------------------------------------------------------*/
void NPCManager::addScore(int aScore)
{
	mTortalScore += aScore;
	if (mTortalScore > 9999990) {
		mTortalScore = 9999990;
	}
}

/*------------------------------------------------------------------------------
�c������Z
------------------------------------------------------------------------------*/
void NPCManager::addLeft(int aLeft)
{
	if (!mIsTestPlay) {
		mAddLeft += aLeft;
	}
	// �G�f�B�^�[���Ȃ�X�R�A���Z
	else {
		mTortalScore += aLeft;
	}
}

/*------------------------------------------------------------------------------
���v�X�R�A��Ԃ�
------------------------------------------------------------------------------*/
int NPCManager::getScore() const
{
	return mTortalScore;
}

/*------------------------------------------------------------------------------
�e�X�g�v���C���[�h
------------------------------------------------------------------------------*/
void NPCManager::testPlayModeOn()
{
	mIsTestPlay = true;
}

/*------------------------------------------------------------------------------
NPC���X�g�̐���Ԃ�
------------------------------------------------------------------------------*/
int NPCManager::getNPCListSize(int a) const
{
	return mNPCList[a].size();
}

/*------------------------------------------------------------------------------
�X�R�A���Z�b�g
------------------------------------------------------------------------------*/
void NPCManager::resetScore()
{
	mTortalScore = 0;
}

/*------------------------------------------------------------------------------
���Z����c���Ԃ�
------------------------------------------------------------------------------*/
int NPCManager::getAddLeft() const
{
	return mAddLeft;
}

/*------------------------------------------------------------------------------
�c����Z�������Z�b�g
------------------------------------------------------------------------------*/
void NPCManager::resetAddLeft()
{
	mAddLeft = 0;
}