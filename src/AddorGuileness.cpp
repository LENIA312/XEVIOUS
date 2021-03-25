#include <DxLib.h>
#include "AddorGuileness.h"
#include "Image.h"
#include "Define.h"
#include "SE.h"
#include "BGM.h"

/*------------------------------------------------------------------------------
�R���X�g���N�^
------------------------------------------------------------------------------*/
AddorGuileness::AddorGuileness(InterfaceNPCEvent* aNPCEvent, NPCDATA aData, Player* aPlayer) : BaseNPC(aNPCEvent)
{
	mPtn = aData.ptn;
	mX = (float)aData.x;
	mY = (float)aData.y - 96;
	mPlayer = aPlayer;

	mSayonaraTimer = 0;
	mSparioTimer[0] = 0;
	mSparioTimer[1] = 0;
	mSparioTimer[2] = 0;
	mSparioTimer[3] = 0;
	mEscapeY = 0;
	mEscapeAnime = 0;
	mBrokenAnime[0] = 0;   // ����
	mBrokenAnime[1] = 0;   // �E��
	mBrokenAnime[2] = 0;   // ����
	mBrokenAnime[3] = 0;   // �E��
	mBrokenFlg[0] = false; // ����
	mBrokenFlg[1] = false; // �E��
	mBrokenFlg[2] = false; // ����
	mBrokenFlg[3] = false; // �E��
}

/*------------------------------------------------------------------------------
�f�X�g���N�^
------------------------------------------------------------------------------*/
AddorGuileness::~AddorGuileness()
{

}

/*------------------------------------------------------------------------------
�X�V
------------------------------------------------------------------------------*/
void AddorGuileness::update()
{
	// �|����Ă��Ȃ���
	if (!mIsStriken) {
		// ������
		if (mSayonaraTimer == 0) {
			// �G�f�B�^�[�p�̏��� (��ʊO�Ńe�X�g�v���C�����Ƃ��ɐ�������Ȃ��Ȃ�)
			if (mY > mCameraY) {
				// BGM��~
				BGM::getIns()->stopBGM(BGM_Boss);
				mActiveFlag = false;
			} // �G�f�B�^�[�̏����I��
			mY -= mCameraY;
		}
		// BGM
		if (!BGM::getIns()->checkIsPlay(BGM_Boss) &&
			!mIsStriken && !BGM::getIns()->checkIsPlay(BGM_Start) &&
			mPlayer->checkActiveFlag() && mActiveFlag) {
			BGM::getIns()->playBGM(BGM_Boss, true, false);
		}
		// �ړ��C�x���g
		{
			// �K��ʒu�܂ŉ�����
			if (mY <= 48 && mSayonaraTimer < 660) {
				mY++;
			}
			if (mY > 48) {
				mY = 48;
			}
			// 11�b������A��
			if (mSayonaraTimer >= 660) {
				mY--;
			}
			mSayonaraTimer++;
		}
		// �p�[�c�j��
		for (int i = 0; i < 4; i++) {
			if (mBrokenFlg[i] && mBrokenAnime[i] < 6) {
				mBrokenAnime[i] += (float)0.2;
			}
		}
		// �X�p���I����
		{
			if (!mBrokenFlg[0] && mSparioTimer[0] > 30 && GetRand(100) == 0) {
				mSparioTimer[0] = 0;
				mNPCEvent->generateEnemyBullet(0, 0, (int)mX + 32, (int)mY + 32 + (int)mCameraY);
			}
			if (!mBrokenFlg[1] && mSparioTimer[1] > 30 && GetRand(100) == 0) {
				mSparioTimer[1] = 0;
				mNPCEvent->generateEnemyBullet(0, 0, (int)mX + 64, (int)mY + 32 + (int)mCameraY);
			}
			if (!mBrokenFlg[2] && mSparioTimer[2] > 30 && GetRand(100) == 0) {
				mSparioTimer[2] = 0;
				mNPCEvent->generateEnemyBullet(0, 0, (int)mX + 32, (int)mY + 64 + (int)mCameraY);
			}
			if (!mBrokenFlg[3] && mSparioTimer[3] > 30 && GetRand(100) == 0) {
				mSparioTimer[3] = 0;
				mNPCEvent->generateEnemyBullet(0, 0, (int)mX + 64, (int)mY + 64 + (int)mCameraY);
			}
			for (int i = 0; i < 4; i++) {
				mSparioTimer[i]++;
			}
		}
		// �����蔻��
		collision();
	}
	// �|���ꂽ��
	else {
		strikenEvent();
	}
	// ��ʊO�ɏo����t���O��܂�
	if (mY + 96 < 0 && !mIsStriken ||
		mY > mCameraY + Define::GameScreenHeight && mIsStriken) {
		mActiveFlag = false;
		// BGM��~
		BGM::getIns()->stopBGM(BGM_Boss);
	}
}

/*------------------------------------------------------------------------------
 �`��
------------------------------------------------------------------------------*/
void AddorGuileness::draw() const
{
	// �ʏ�
	if (!mIsStriken) {
		// �{��
		DrawGraph((int)mX, (int)mY, Image::getIns()->getImage(Image_Boss, mColor), true);
		DrawGraph((int)mX + 40, (int)mY + 40, Image::getIns()->getImage(Image_BossParts, mColor), true);
		// ����
		{
			if (!mBrokenFlg[0]) {
				DrawGraph((int)mX + 24, (int)mY + 24, Image::getIns()->getImage(Image_BossParts, 5 + mColor), true);
			}
			// ����
			else {
				if (mBrokenAnime[0] < 6) {
					DrawGraph((int)mX + 16, (int)mY + 16, Image::getIns()->getImage(Image_EnemyStriken, 6 + (int)mBrokenAnime[0]), true);
				}
			}
		}
		// �E��
		{
			if (!mBrokenFlg[1]) {
				DrawGraph((int)mX + 56, (int)mY + 24, Image::getIns()->getImage(Image_BossParts, 10 + mColor), true);
			}
			// ����
			else {
				if (mBrokenAnime[1] < 6) {
					DrawGraph((int)mX + 48, (int)mY + 16, Image::getIns()->getImage(Image_EnemyStriken, 6 + (int)mBrokenAnime[1]), true);
				}
			}
		}
		// ����
		{
			if (!mBrokenFlg[2]) {
				DrawGraph((int)mX + 24, (int)mY + 56, Image::getIns()->getImage(Image_BossParts, 15 + mColor), true);
			}
			// ����
			else {
				if (mBrokenAnime[2] < 6) {
					DrawGraph((int)mX + 16, (int)mY + 48, Image::getIns()->getImage(Image_EnemyStriken, 6 + (int)mBrokenAnime[2]), true);
				}
			}
		}
		// �E��
		{
			if (!mBrokenFlg[3]) {
				DrawGraph((int)mX + 56, (int)mY + 56, Image::getIns()->getImage(Image_BossParts, 20 + mColor), true);
			}
			// ����
			else {
				if (mBrokenAnime[3] < 6) {
					DrawGraph((int)mX + 48, (int)mY + 48, Image::getIns()->getImage(Image_EnemyStriken, 6 + (int)mBrokenAnime[3]), true);
				}
			}
		}
	}
	// �|���ꂽ�Ƃ�
	else {
		// �{��
		if (mAnime < 1) {
			DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Boss, 5), true);
		}
		else {
			DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Boss, 4), true);
		}
		// �����G�t�F�N�g
		{
			if (mAnime < 6) {
				DrawGraph((int)mX + 32, (int)mY + 32 - (int)mCameraY, Image::getIns()->getImage(Image_EnemyStriken, 6 + (int)mAnime), true);
			}
			// �E�o���邠�̋�
			else {
				DrawGraph((int)mX + 40, (int)mEscapeY + 40 - (int)mCameraY, Image::getIns()->getImage(Image_EnemyBullet, 20 + (int)mEscapeAnime), true);
			}
			if (mBrokenFlg[0] && mBrokenAnime[0] < 6) {
				DrawGraph((int)mX + 16, (int)mY + 16 - (int)mCameraY, Image::getIns()->getImage(Image_EnemyStriken, 6 + (int)mBrokenAnime[0]), true);
			}
			if (mBrokenFlg[1] && mBrokenAnime[1] < 6) {
				DrawGraph((int)mX + 48, (int)mY + 16 - (int)mCameraY, Image::getIns()->getImage(Image_EnemyStriken, 6 + (int)mBrokenAnime[1]), true);
			}
			if (mBrokenFlg[2] && mBrokenAnime[2] < 6) {
				DrawGraph((int)mX + 16, (int)mY + 48 - (int)mCameraY, Image::getIns()->getImage(Image_EnemyStriken, 6 + (int)mBrokenAnime[2]), true);
			}
			if (mBrokenFlg[3] && mBrokenAnime[3] < 6) {
				DrawGraph((int)mX + 48, (int)mY + 48 - (int)mCameraY, Image::getIns()->getImage(Image_EnemyStriken, 6 + (int)mBrokenAnime[3]), true);
			}
		}
	}
	// �f�o�b�O
	//DrawFormatString(0, 0, GetColor(255, 255, 255), "\ny:%0.1f s:%d col:%d", mY, mIsStriken, getCollisionPoint(1));
}

/*------------------------------------------------------------------------------
�����蔻��`��
------------------------------------------------------------------------------*/
void AddorGuileness::drawCollision(int r, int g, int b) const
{
	if (!mIsStriken) {
		DrawBox((int)mX + 26, (int)mY + 26, (int)mX + 24 + 14, (int)mY + 24 + 14, GetColor(r, g, b), FALSE);
		DrawBox((int)mX + 58, (int)mY + 26, (int)mX + 56 + 14, (int)mY + 24 + 14, GetColor(r, g, b), FALSE);
		DrawBox((int)mX + 26, (int)mY + 58, (int)mX + 24 + 14, (int)mY + 56 + 14, GetColor(r, g, b), FALSE);
		DrawBox((int)mX + 58, (int)mY + 58, (int)mX + 56 + 14, (int)mY + 56 + 14, GetColor(r, g, b), FALSE);
		DrawBox((int)mX + 40, (int)mY + 40, (int)mX + 40 + 16, (int)mY + 40 + 16, GetColor(r, g, b), FALSE);
	}
}

/*------------------------------------------------------------------------------
�G���A�̕ς��ڂɓ����蔻����ړ�����
------------------------------------------------------------------------------*/
void AddorGuileness::areaChange()
{
	// �|���ꂽ��
	if (mIsStriken) {
		mY += Define::MapHeight;
	}
}

/*------------------------------------------------------------------------------
�|���ꂽ�Ƃ���
------------------------------------------------------------------------------*/
void AddorGuileness::strikenEvent()
{	
	// �����A�j��
	if (mAnime < 6) {
		mAnime += (float)0.2;
	}
	// �E�o�{�[��
	else {
		mEscapeY -= 5;
		mEscapeAnime += 0.5;
		if (mEscapeAnime >= 8) {
			mEscapeAnime = 0;
		}
	}
	// �p�[�c�����A�j��
	for (int i = 0; i < 4; i++) {
		if (mBrokenFlg[i] && mBrokenAnime[i] < 6) {
			mBrokenAnime[i] += (float)0.2;
		}
	}
}

/*------------------------------------------------------------------------------
�u���X�^�[�ɓ���������
------------------------------------------------------------------------------*/
void AddorGuileness::collisionGameObject(eTag aTag)
{
	if (aTag == Tag_Blaster && !mIsStriken) {
		// �u���X�^�[���W
		Blaster* blaster = mPlayer->getBlaster();
		int bx = blaster->getCollisionPoint(0);
		int by = blaster->getCollisionPoint(1);
		int bw = blaster->getCollisionPoint(2);
		int bh = blaster->getCollisionPoint(3);
		// �����蔻�� �� / ����
		int w = 14;
		int h = 14;
		int x, y;
		// �^�񒆂ɓ���������
		{
			x = (int)mX + 40;
			y = (int)mY + 40 + (int)mCameraY;
			if (bx + bw >= x && bx <= x + w &&
				by + bh >= y && by <= y + h) {
				// y���W���Z
				mY += mCameraY;
				mEscapeY = mY;
				// �|���ꂽ�t���O�𗧂Ă�
				mIsStriken = true;
				for (int i = 0; i < 4; i++) {
					mBrokenFlg[i] = true;
				}
				// �X�R�A�����Z����
				mNPCEvent->addScore(4800);
				// ���ʉ��Đ�
				SE::getIns()->playSE(SE_EnemyStrikenBlaster);
				// BGM��~
				BGM::getIns()->stopBGM(BGM_Boss);
			}
		}
		// ����ɓ���������
		{
			x = (int)mX + 26;
			y = (int)mY + 26 + (int)mCameraY;
			if (!mBrokenFlg[0] &&
				bx + bw >= x && bx <= x + w &&
				by + bh >= y && by <= y + h) {
				mBrokenFlg[0] = true;
				// �X�R�A�����Z����
				mNPCEvent->addScore(1000);
				// ���ʉ��Đ�
				SE::getIns()->playSE(SE_EnemyStrikenBlaster);
			}
		}
		// �E��ɓ���������
		{
			x = (int)mX + 58;
			y = (int)mY + 26 + (int)mCameraY;
			if (!mBrokenFlg[1] &&
				bx + bw >= x && bx <= x + w &&
				by + bh >= y && by <= y + h) {
				mBrokenFlg[1] = true;
				// �X�R�A�����Z����
				mNPCEvent->addScore(1000);
				// ���ʉ��Đ�
				SE::getIns()->playSE(SE_EnemyStrikenBlaster);
			}
		}
		// �����ɓ���������
		{
			x = (int)mX + 26;
			y = (int)mY + 58 + (int)mCameraY;
			if (!mBrokenFlg[2] &&
				bx + bw >= x && bx <= x + w &&
				by + bh >= y && by <= y + h) {
				mBrokenFlg[2] = true;
				// �X�R�A�����Z����
				mNPCEvent->addScore(1000);
				// ���ʉ��Đ�
				SE::getIns()->playSE(SE_EnemyStrikenBlaster);
			}
		}
		// �E��ɓ���������
		{
			x = (int)mX + 58;
			y = (int)mY + 58 + (int)mCameraY;
			if (!mBrokenFlg[3] &&
				bx + bw >= x && bx <= x + w &&
				by + bh >= y && by <= y + h) {
				mBrokenFlg[3] = true;
				// �X�R�A�����Z����
				mNPCEvent->addScore(1000);
				// ���ʉ��Đ�
				SE::getIns()->playSE(SE_EnemyStrikenBlaster);
			}
		}
	}
}

/*------------------------------------------------------------------------------
�u���X�^�[�Ƃ̓����蔻��
------------------------------------------------------------------------------*/
void AddorGuileness::collision()
{
	// �G�̍��W
	int ex = getCollisionPoint(0);
	int ey = getCollisionPoint(1);
	int ew = getCollisionPoint(2);
	int eh = getCollisionPoint(3);
	// �u���X�^�[���G�ɓ���������
	{
		Blaster* blaster = mPlayer->getBlaster();
		if (blaster->getHitFlag()) {
			int bx = blaster->getCollisionPoint(0);
			int by = blaster->getCollisionPoint(1);
			int bw = blaster->getCollisionPoint(2);
			int bh = blaster->getCollisionPoint(3);
			if (bx + bw >= ex && bx <= ex + ew &&
				by + bh >= ey && by <= ey + eh) {
				collisionGameObject(blaster->getTag());
				blaster->collisionGameObject(getTag());
			}
		}
	}
	// �Ə��`�J�`�J
	{
		// �Ə����W
		Blaster* blaster = mPlayer->getBlaster();
		int bx = mPlayer->getAimngXY(0);
		int by = mPlayer->getAimngXY(1);
		int bw = blaster->getCollisionPoint(2);
		int bh = blaster->getCollisionPoint(3);
		// �Ə����G�ɓ���������
		if (bx + bw >= ex && bx <= ex + ew &&
			by + bh >= ey && by <= ey + eh && mPlayer->checkActiveFlag()) {
			mPlayer->setTikaTikaFlg(true);
		}
	}
}

/*------------------------------------------------------------------------------
�����蔻����W��Ԃ�
------------------------------------------------------------------------------*/
int AddorGuileness::getCollisionPoint(int aPos) const
{
	switch (aPos) {
	case 0: // x
		return (int)mX;
	case 1: // y
		if (!mIsStriken) {
			return (int)mY + (int)mCameraY;
		}
		else {
			return (int)mY;
		}		
	case 2: // w
		return 96;
	case 3: // h
		return 96;
	}
	return -1;
}

/*------------------------------------------------------------------------------
�^�O��Ԃ�
------------------------------------------------------------------------------*/
eTag AddorGuileness::getTag() const
{
	return Tag_Enemy_Ground;
}
