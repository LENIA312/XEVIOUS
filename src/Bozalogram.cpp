#include <DxLib.h>
#include "Bozalogram.h"
#include "Image.h"
#include "Define.h"
#include "SE.h"

/*------------------------------------------------------------------------------
�萔
------------------------------------------------------------------------------*/
const static int LEFT = 0; // �{�U���O�����̍���
const static int RIGHT = 1; // �{�U���O�����̉E��
const static int UP = 2; // �{�U���O�����̏㕔
const static int DOWN = 3; // �{�U���O�����̉���

/*------------------------------------------------------------------------------
�R���X�g���N�^
------------------------------------------------------------------------------*/
Bozalogram::Bozalogram(InterfaceNPCEvent* aNPCEvent, NPCDATA aData, Player* aPlayer) : BaseNPC(aNPCEvent)
{
	mPtn = aData.ptn;
	mX = (float)aData.x;
	mY = (float)aData.y - 37;
	mPlayer = aPlayer;

	for (int i = 0; i < 4; i++) {
		mAtkFlg[i] = 0;
		lAtkMode[i] = 0;
		mPartAnime[i] = 0;
		mSparioTimer[i] = 0;
		mBrokenAnime[i] = 0;
		mBrokenFlg[i] = false;
	}

}

/*------------------------------------------------------------------------------
�f�X�g���N�^
------------------------------------------------------------------------------*/
Bozalogram::~Bozalogram()
{

}

/*------------------------------------------------------------------------------
�X�V
------------------------------------------------------------------------------*/
void Bozalogram::update()
{
	// �X�p���I����
	if(mY <= mCameraY + Define::GameScreenHeight / 2){
		for (int i = 0; i < 4; i++) {
			if (!mBrokenFlg[i] && mSparioTimer[0] > 30 && GetRand(100) == 0 && !mAtkFlg[i]) {
				mSparioTimer[i] = 0;
				mAtkFlg[i] = true;
			}
		}
	}
	// ���E�㉺
	for (int i = 0; i < 4; i++) {
		// �ʏ�
		if (!mBrokenFlg[i]) {
			if (mAtkFlg[i] == true && lAtkMode[i] == 0) {
				lAtkMode[i] = 1;
			}

			if (lAtkMode[i] == 1) {
				mPartAnime[i] += (float)0.3;
			}

			if (lAtkMode[i] == 2) {
				mPartAnime[i] -= (float)0.3;
				if (mPartAnime[i] <= 0) {
					mPartAnime[i] = 0;
					lAtkMode[i] = 0;
					mAtkFlg[i] = false;
				}
			}

			if (mPartAnime[i] > 4) {
				mPartAnime[i] = 3.5;
				switch (i) {
				case 0:
					mNPCEvent->generateEnemyBullet(0, 0, (int)mX - 4, (int)mY + 8);
					break;
				case 1:
					mNPCEvent->generateEnemyBullet(0, 0, (int)mX + 20, (int)mY + 8);
					break;
				case 2:
					mNPCEvent->generateEnemyBullet(0, 0, (int)mX + 8, (int)mY - 4);
					break;
				case 3:
					mNPCEvent->generateEnemyBullet(0, 0, (int)mX + 8, (int)mY + 20);
					break;
				}
				lAtkMode[i] = 2;
			}
			// �^�C�}�[���Z
			mSparioTimer[i]++;
		}
		// �j��
		else {
			mBrokenAnime[i] += (float)0.2;
			if (mBrokenAnime[i] >= 8) {
				mBrokenAnime[i] = 6;
			}
		}
	}
	// �|����Ă��Ȃ���
	if (!mIsStriken) {
		collision();
	}
	// �|���ꂽ
	else {
		strikenEvent();
	}
	// ��ʊO�ɏo����t���O��܂�
	if (mY > mCameraY + Define::GameScreenHeight) {
		mActiveFlag = false;
	}
}

/*------------------------------------------------------------------------------
 �`��
------------------------------------------------------------------------------*/
void Bozalogram::draw() const
{
	// ��
	{
		// �ʏ�
		if (!mBrokenFlg[LEFT]) {
			switch (mColor) {
			case 0:
				DrawGraph((int)mX - 12, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_a, 36 + (int)mPartAnime[0]), true);
				break;
			case 1:
				DrawGraph((int)mX - 12, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_b, 36 + (int)mPartAnime[0]), true);
				break;
			case 2:
				DrawGraph((int)mX - 12, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_c, 36 + (int)mPartAnime[0]), true);
				break;
			case 3:
				DrawGraph((int)mX - 12, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_d, 36 + (int)mPartAnime[0]), true);
				break;
			case 4:
				DrawGraph((int)mX - 12, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_e, 36 + (int)mPartAnime[0]), true);
				break;
			}
		}
		// �j��
		else {
			if (mBrokenAnime[LEFT] < 6) {
				DrawGraph((int)mX - 8 - 12, ((int)mY - 8) - (int)mCameraY, Image::getIns()->getImage(Image_EnemyStriken, 6 + (int)mBrokenAnime[0]), true);
			}
			if (mBrokenAnime[LEFT] >= 6) {
				DrawGraph((int)mX - 12, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Destructed, (int)mBrokenAnime[0] - 6), true);
			}
		}
	}
	// �E
	{
		// �ʏ�
		if (!mBrokenFlg[RIGHT]) {
			switch (mColor) {
			case 0:
				DrawGraph((int)mX + 12, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_a, 36 + (int)mPartAnime[1]), true);
				break;
			case 1:
				DrawGraph((int)mX + 12, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_b, 36 + (int)mPartAnime[1]), true);
				break;
			case 2:
				DrawGraph((int)mX + 12, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_c, 36 + (int)mPartAnime[1]), true);
				break;
			case 3:
				DrawGraph((int)mX + 12, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_d, 36 + (int)mPartAnime[1]), true);
				break;
			case 4:
				DrawGraph((int)mX + 12, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_e, 36 + (int)mPartAnime[1]), true);
				break;
			}
		}
		// �j��
		else {
			if (mBrokenAnime[RIGHT] < 6) {
				DrawGraph((int)mX - 8 + 12, ((int)mY - 8) - (int)mCameraY, Image::getIns()->getImage(Image_EnemyStriken, 6 + (int)mBrokenAnime[1]), true);
			}
			if (mBrokenAnime[RIGHT] >= 6) {
				DrawGraph((int)mX + 12, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Destructed, (int)mBrokenAnime[1] - 6), true);
			}
		}
	}
	// ��
	{
		// �ʏ�
		if (!mBrokenFlg[UP]) {
			switch (mColor) {
			case 0:
				DrawGraph((int)mX, (int)mY - 12 - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_a, 36 + (int)mPartAnime[2]), true);
				break;
			case 1:
				DrawGraph((int)mX, (int)mY - 12 - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_b, 36 + (int)mPartAnime[2]), true);
				break;
			case 2:
				DrawGraph((int)mX, (int)mY - 12 - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_c, 36 + (int)mPartAnime[2]), true);
				break;
			case 3:
				DrawGraph((int)mX, (int)mY - 12 - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_d, 36 + (int)mPartAnime[2]), true);
				break;
			case 4:
				DrawGraph((int)mX, (int)mY - 12 - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_e, 36 + (int)mPartAnime[2]), true);
				break;
			}
		}
		// �j��
		else {
			if (mBrokenAnime[UP] < 6) {
				DrawGraph((int)mX - 8 , ((int)mY - 8) - 12 - (int)mCameraY, Image::getIns()->getImage(Image_EnemyStriken, 6 + (int)mBrokenAnime[2]), true);
			}
			if (mBrokenAnime[UP] >= 6) {
				DrawGraph((int)mX, (int)mY - 12 - (int)mCameraY, Image::getIns()->getImage(Image_Destructed, (int)mBrokenAnime[2] - 6), true);
			}
		}
	}
	// ��
	{
		// �ʏ�
		if (!mBrokenFlg[DOWN]) {
			switch (mColor) {
			case 0:
				DrawGraph((int)mX, (int)mY + 12 - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_a, 36 + (int)mPartAnime[3]), true);
				break;
			case 1:
				DrawGraph((int)mX, (int)mY + 12 - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_b, 36 + (int)mPartAnime[3]), true);
				break;
			case 2:
				DrawGraph((int)mX, (int)mY + 12 - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_c, 36 + (int)mPartAnime[3]), true);
				break;
			case 3:
				DrawGraph((int)mX, (int)mY + 12 - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_d, 36 + (int)mPartAnime[3]), true);
				break;
			case 4:
				DrawGraph((int)mX, (int)mY + 12 - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_e, 36 + (int)mPartAnime[3]), true);
				break;
			}
		}
		// �j��
		else {
			if (mBrokenAnime[DOWN] < 6) {
				DrawGraph((int)mX - 8, ((int)mY - 8) + 12 - (int)mCameraY, Image::getIns()->getImage(Image_EnemyStriken, 6 + (int)mBrokenAnime[3]), true);
			}
			if (mBrokenAnime[DOWN] >= 6) {
				DrawGraph((int)mX, (int)mY + 12 - (int)mCameraY, Image::getIns()->getImage(Image_Destructed, (int)mBrokenAnime[3] - 6), true);
			}
		}
	}
	// �^��
	{
		// �ʏ�
		if (!mIsStriken) {
			switch (mColor) {
			case 0:
				DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_a, 62 + (int)mAnime), true);
				break;
			case 1:
				DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_b, 62 + (int)mAnime), true);
				break;
			case 2:
				DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_c, 62 + (int)mAnime), true);
				break;
			case 3:
				DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_d, 62 + (int)mAnime), true);
				break;
			case 4:
				DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_e, 62 + (int)mAnime), true);
				break;
			}
		}
		// �j��
		else {
			if (mAnime < 6) {
				DrawGraph((int)mX - 8, ((int)mY - 8) - (int)mCameraY, Image::getIns()->getImage(Image_EnemyStriken, 6 + (int)mAnime), true);
			}
			if (mAnime >= 6) {
				DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_Destructed, (int)mAnime - 6), true);
			}
		}
	}
	// �f�o�b�O
}

/*------------------------------------------------------------------------------
�����蔻��`��
------------------------------------------------------------------------------*/
void Bozalogram::drawCollision(int r, int g, int b) const
{
	int a = 12;
	DrawBox((int)mX, (int)mY - (int)mCameraY, (int)mX + 16, (int)mY + 16 - (int)mCameraY, GetColor(r, g, b), FALSE);
	DrawBox((int)mX - a, (int)mY - (int)mCameraY, (int)mX - a + 16, (int)mY + 16 - (int)mCameraY, GetColor(r, g, b), FALSE);
	DrawBox((int)mX + a, (int)mY - (int)mCameraY, (int)mX + a + 16, (int)mY + 16 - (int)mCameraY, GetColor(r, g, b), FALSE);
	DrawBox((int)mX, (int)mY - a - (int)mCameraY, (int)mX + 16, (int)mY - a + 16 - (int)mCameraY, GetColor(r, g, b), FALSE);
	DrawBox((int)mX, (int)mY + a - (int)mCameraY, (int)mX + 16, (int)mY + a + 16 - (int)mCameraY, GetColor(r, g, b), FALSE);
}

/*------------------------------------------------------------------------------
�|���ꂽ�Ƃ���
------------------------------------------------------------------------------*/
void Bozalogram::strikenEvent()
{
	mAnime += (float)0.2;
	if (mAnime >= 8) {
		mAnime = 6;
	}
}

/*------------------------------------------------------------------------------
�u���X�^�[�ɓ���������
------------------------------------------------------------------------------*/
void Bozalogram::collisionGameObject(eTag aTag)
{
	if (aTag == Tag_Blaster && !mIsStriken) {
		// �u���X�^�[���W
		Blaster* blaster = mPlayer->getBlaster();
		int bx = blaster->getCollisionPoint(0);
		int by = blaster->getCollisionPoint(1);
		int bw = blaster->getCollisionPoint(2);
		int bh = blaster->getCollisionPoint(3);
		// �����蔻�� �� / ����
		int w = 16;
		int h = 16;
		int x, y;
		// �^�񒆂ɓ���������
		{
			x = (int)mX;
			y = (int)mY;
			if (bx + bw >= x && bx <= x + w &&
				by + bh >= y && by <= y + h) {
				// �p�[�c�̓|���ꂽ�t���O�𗧂Ă�
				mIsStriken = true;
				for (int i = 0; i < 4; i++) {
					mBrokenFlg[i] = true;
				}
				// �X�R�A�����Z����
				mNPCEvent->addScore(10);
				// ���ʉ��Đ�
				SE::getIns()->playSE(SE_EnemyStrikenBlaster);
			}
		}
		// ���ɓ���������
		{
			x = (int)mX - 12;
			y = (int)mY;
			if (!mBrokenFlg[0] &&
				bx + bw >= x && bx <= x + w &&
				by + bh >= y && by <= y + h) {
				mBrokenFlg[0] = true;
				// �X�R�A�����Z����
				mNPCEvent->addScore(1);
				// ���ʉ��Đ�
				SE::getIns()->playSE(SE_EnemyStrikenBlaster);
			}
		}
		// �E�ɓ���������
		{
			x = (int)mX + 12;
			y = (int)mY;
			if (!mBrokenFlg[1] &&
				bx + bw >= x && bx <= x + w &&
				by + bh >= y && by <= y + h) {
				mBrokenFlg[1] = true;
				// �X�R�A�����Z����
				mNPCEvent->addScore(1);
				// ���ʉ��Đ�
				SE::getIns()->playSE(SE_EnemyStrikenBlaster);
			}
		}
		// ��ɓ���������
		{
			x = (int)mX;
			y = (int)mY - 12;
			if (!mBrokenFlg[2] &&
				bx + bw >= x && bx <= x + w &&
				by + bh >= y && by <= y + h) {
				mBrokenFlg[2] = true;
				// �X�R�A�����Z����
				mNPCEvent->addScore(1);
				// ���ʉ��Đ�
				SE::getIns()->playSE(SE_EnemyStrikenBlaster);
			}
		}
		// ���ɓ���������
		{
			x = (int)mX;
			y = (int)mY + 12;
			if (!mBrokenFlg[3] &&
				bx + bw >= x && bx <= x + w &&
				by + bh >= y && by <= y + h) {
				mBrokenFlg[3] = true;
				// �X�R�A�����Z����
				mNPCEvent->addScore(1);
				// ���ʉ��Đ�
				SE::getIns()->playSE(SE_EnemyStrikenBlaster);
			}
		}
	}
}

/*------------------------------------------------------------------------------
�u���X�^�[�Ƃ̓����蔻��
------------------------------------------------------------------------------*/
void Bozalogram::collision()
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
int Bozalogram::getCollisionPoint(int aPos) const
{
	switch (aPos) {
	case 0: // x
		return (int)mX - 10 + 2;
	case 1: // y
		return (int)mY - 10 + 2;
	case 2: // w
		return 37 - 3;
	case 3: // h
		return 37 - 3;
	}
	return -1;
}

/*------------------------------------------------------------------------------
�^�O��Ԃ�
------------------------------------------------------------------------------*/
eTag Bozalogram::getTag() const
{
	return Tag_Enemy_Ground;
}