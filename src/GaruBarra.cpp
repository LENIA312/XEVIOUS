#include <DxLib.h>
#include "GaruBarra.h"
#include "Image.h"
#include "Define.h"
#include "SE.h"

/*------------------------------------------------------------------------------
�R���X�g���N�^
------------------------------------------------------------------------------*/
GaruBarra::GaruBarra(InterfaceNPCEvent* aNPCEvent, NPCDATA aData, Player* aPlayer) : BaseNPC(aNPCEvent)
{
	mPtn = aData.ptn;
	mX = (float)aData.x;
	mY = (float)aData.y - 32;
	mPlayer = aPlayer;
}

/*------------------------------------------------------------------------------
�f�X�g���N�^
------------------------------------------------------------------------------*/
GaruBarra::~GaruBarra()
{

}

/*------------------------------------------------------------------------------
�X�V
------------------------------------------------------------------------------*/
void GaruBarra::update()
{
	// �|����Ă��Ȃ���
	if (!mIsStriken) {
		collision();
	}
	// �|���ꂽ��
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
void GaruBarra::draw() const
{
	// �K���o�[��
	switch (mColor) {
	case 0:
		DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_EnemyBig_a, 1), true);
		break;
	case 1:
		DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_EnemyBig_b, 1), true);
		break;
	case 2:
		DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_EnemyBig_c, 1), true);
		break;
	case 3:
		DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_EnemyBig_d, 1), true);
		break;
	case 4:
		DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_EnemyBig_d, 1), true);
		break;
	}
	// �o�[��
	if (!mIsStriken) {
		switch (mColor) {
		case 0:
			DrawGraph((int)mX + 8, (int)mY + 8 - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_a, 23), true);
			break;
		case 1:
			DrawGraph((int)mX + 8, (int)mY + 8 - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_b, 23), true);
			break;
		case 2:
			DrawGraph((int)mX + 8, (int)mY + 8 - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_c, 23), true);
			break;
		case 3:
			DrawGraph((int)mX + 8, (int)mY + 8 - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_d, 23), true);
			break;
		case 4:
			DrawGraph((int)mX + 8, (int)mY + 8 - (int)mCameraY, Image::getIns()->getImage(Image_Enemy_e, 23), true);
			break;
		}
	}
	// �j��
	if (mIsStriken) {
		if (mAnime < 6) {
			DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_EnemyStriken, 6 + (int)mAnime), true);
		}
	}

	//DrawFormatString(0, 0, GetColor(255, 255, 255), "nyoki:%d so:%d", nyokiFlg, strikeOn);
}

/*------------------------------------------------------------------------------
�|���ꂽ�Ƃ���
------------------------------------------------------------------------------*/
void GaruBarra::strikenEvent()
{
	mAnime += (float)0.2;
	if (mAnime >= 8) {
		mAnime = 6;
	}
}

/*------------------------------------------------------------------------------
�u���X�^�[�ɓ���������
------------------------------------------------------------------------------*/
void GaruBarra::collisionGameObject(eTag aTag)
{
	if (aTag == Tag_Blaster && !mIsStriken) {
		// �|���ꂽ�t���O�𗧂Ă�
		mIsStriken = true;
		// �A�j���ʒu��������
		mAnime = 0;
		// �X�R�A�����Z����
		mNPCEvent->addScore(300);
		// ���ʉ��Đ�
		SE::getIns()->playSE(SE_EnemyStrikenBlaster);
	}
}

/*------------------------------------------------------------------------------
�u���X�^�[�Ƃ̓����蔻��
------------------------------------------------------------------------------*/
void GaruBarra::collision()
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
int GaruBarra::getCollisionPoint(int aPos) const
{
	switch (aPos) {
	case 0: // x
		return (int)mX + 8;
	case 1: // y
		return (int)mY + 8;
	case 2: // w
		return 16;
	case 3: // h
		return 16;
	}
	return -1;
}

/*------------------------------------------------------------------------------
�^�O��Ԃ�
------------------------------------------------------------------------------*/
eTag GaruBarra::getTag() const
{
	return Tag_Enemy_Ground;
}
