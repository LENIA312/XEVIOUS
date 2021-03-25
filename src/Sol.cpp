#include <DxLib.h>
#include "Sol.h"
#include "Image.h"
#include "Define.h"
#include "SE.h"

/*------------------------------------------------------------------------------
�R���X�g���N�^
------------------------------------------------------------------------------*/
Sol::Sol(InterfaceNPCEvent* aNPCEvent, NPCDATA aData, Player* aPlayer) : BaseNPC(aNPCEvent)
{
	mPtn = aData.ptn;
	mX = (float)aData.x;
	mY = (float)aData.y - 16;
	mPlayer = aPlayer;
	hitCnt = 0;

	appearFlg = false;
	strikeOn = false;

}

/*------------------------------------------------------------------------------
�f�X�g���N�^
------------------------------------------------------------------------------*/
Sol::~Sol()
{

}

/*------------------------------------------------------------------------------
�X�V
------------------------------------------------------------------------------*/
void Sol::update()
{
	// �|����Ă��Ȃ���
	if (!mIsStriken) {
		collision();
	}
	// �|���ꂽ��
	else {
		strikenEvent();
	}

	if (appearFlg) {
		if (!mIsStriken) {
			mAnime += (float)0.1;
			if (mAnime >= 7) {
				mAnime = 7;
				strikeOn = true;
			}
		}
	}


	// ��ʊO�ɏo����t���O��܂�
	if (mY > mCameraY + Define::GameScreenHeight) {
		mActiveFlag = false;
	}


}

/*------------------------------------------------------------------------------
 �`��
------------------------------------------------------------------------------*/
void Sol::draw() const
{
	// �ʏ�
	if (!mIsStriken) {
		switch (mColor) {
		case 0:
			DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_EnemyBig_a,7 + (int)mAnime), true);
			break;
		case 1:
			DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_EnemyBig_b, 7 + (int)mAnime), true);
			break;
		case 2:
			DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_EnemyBig_c, 7 + (int)mAnime), true);
			break;
		case 3:
			DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_EnemyBig_d, 7 + (int)mAnime), true);
			break;
		case 4:
			DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_EnemyBig_d, 7 + (int)mAnime), true);
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

	//DrawFormatString(0, 0, GetColor(255, 255, 255), "nyoki:%d so:%d", appearFlg, strikeOn);
}

/*------------------------------------------------------------------------------
�|���ꂽ�Ƃ���
------------------------------------------------------------------------------*/
void Sol::strikenEvent()
{
	mAnime += (float)0.2;
	if (mAnime >= 8) {
		mAnime = 6;
	}
}

/*------------------------------------------------------------------------------
�u���X�^�[�ɓ���������
------------------------------------------------------------------------------*/
void Sol::collisionGameObject(eTag aTag)
{
	//������ꂽ�Ƃ�
	if (aTag == Tag_Blaster && !mIsStriken &&!appearFlg) {
		appearFlg = true;
		mNPCEvent->addScore(1000);
	}
	if (aTag == Tag_Blaster && !mIsStriken && strikeOn) {
			// �|���ꂽ�t���O�𗧂Ă�
			mIsStriken = true;
			// �A�j���ʒu��������
			mAnime = 0;
			// �X�R�A�����Z����
			mNPCEvent->addScore(2000);
			// ���ʉ��Đ�
			SE::getIns()->playSE(SE_EnemyStrikenBlaster);	
	}
}

/*------------------------------------------------------------------------------
�u���X�^�[�Ƃ̓����蔻��
------------------------------------------------------------------------------*/
void Sol::collision()
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
int Sol::getCollisionPoint(int aPos) const
{
	switch (aPos) {
	case 0: // x
		return (int)mX;
	case 1: // y
		return (int)mY;
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
eTag Sol::getTag() const
{
	return Tag_Enemy_Ground;
}
