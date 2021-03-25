#include <DxLib.h>
#include "SpecialFlag.h"
#include "Image.h"
#include "Define.h"
#include "SE.h"

/*------------------------------------------------------------------------------
�R���X�g���N�^
------------------------------------------------------------------------------*/
SpecialFlag::SpecialFlag(InterfaceNPCEvent* aNPCEvent, NPCDATA aData, Player* aPlayer) : BaseNPC(aNPCEvent)
{
	mPtn = aData.ptn;
	mX = (float)aData.x;
	mY = (float)aData.y - 16;
	mPlayer = aPlayer;

	mSpeedX = -1;
	mSpeedY = -1;
	mLRFlg = false;
	mAcceleFlg = false;

	mGetOn = false;
	mGetFlg = false;

	// �ʒu������
	init();
}

/*------------------------------------------------------------------------------
�f�X�g���N�^
------------------------------------------------------------------------------*/
SpecialFlag::~SpecialFlag()
{

}

/*------------------------------------------------------------------------------
�X�V
------------------------------------------------------------------------------*/
void SpecialFlag::update()
{

	// �v���C���[���W
	int px = mPlayer->getCollisionPoint(0);
	int pw = mPlayer->getCollisionPoint(2);
	
	// �|����Ă��Ȃ���
	if (!mGetFlg) {
		collision();
	}
	// �|���ꂽ��
	else {
		strikenEvent();
	}
	// ��ʊO�ɏo����t���O��܂�
	if (mX + 16 < 0 ||
		mX > Define::GameScreenWidth - 128 ||
		mY + 16 < mCameraY ||
		mY > mCameraY + Define::GameScreenHeight) {
		mActiveFlag = false;
	}
}

/*------------------------------------------------------------------------------
 �`��
------------------------------------------------------------------------------*/
void SpecialFlag::draw() const
{
	if (mGetOn) {

		DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_SpecialFlag), true);

	}
	// �f�o�b�O
	//DrawLine(mPlayerX, 0, mPlayerX, 300, GetColor(0, 0, 0));
}

/*------------------------------------------------------------------------------
�o���ʒu�̏�����
------------------------------------------------------------------------------*/
void SpecialFlag::init()
{
	mX = (float)GetRand(325) - 16;
}

/*------------------------------------------------------------------------------
�|���ꂽ�Ƃ���
------------------------------------------------------------------------------*/
void SpecialFlag::strikenEvent()
{

	mActiveFlag = false;

}


/*------------------------------------------------------------------------------
�u���X�^�[�ɓ���������
------------------------------------------------------------------------------*/
void SpecialFlag::collisionGameObject(eTag aTag)
{
	if (aTag == Tag_Blaster && !mIsStriken && !mGetOn) {
		// ������ꂽ�t���O�𗧂Ă�
		mGetOn = true;
		// �X�R�A�����Z����
		mNPCEvent->addScore(1000);
		// ���ʉ��Đ�
		//SE::getIns()->playSE(SE_EnemyStrikenBlaster);
	}
}

/*------------------------------------------------------------------------------
�����蔻��
------------------------------------------------------------------------------*/
void SpecialFlag::collision()
{
	// �G�̍��W
	int ex = getCollisionPoint(0);
	int ey = getCollisionPoint(1);
	int ew = getCollisionPoint(2);
	int eh = getCollisionPoint(3);

	// �U�b�p�[���G�ɓ���������
	Zapper* zapper = mPlayer->getZapper();
	for (int i = 0; i < Define::ZapperMax; i++) {
		if (zapper[i].checkActiveFlag()) {
			int zx = zapper[i].getCollisionPoint(0);
			int zy = zapper[i].getCollisionPoint(1);
			int zw = zapper[i].getCollisionPoint(2);
			int zh = zapper[i].getCollisionPoint(3);
			if (zx + zw >= ex && zx <= ex + ew &&
				zy + zh >= ey && zy <= ey + eh) {
				collisionGameObject(zapper[i].getTag());
				zapper[i].collisionGameObject(getTag());
			}
		}
	}

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

	// �v���C���[���G�ꂽ��
	if (mPlayer->getCollisionPoint(0) + mPlayer->getCollisionPoint(2)
		>= ex && mPlayer->getCollisionPoint(0) <= ex + ew &&
		mPlayer->getCollisionPoint(1) + mPlayer->getCollisionPoint(3) >= ey
		&& mPlayer->getCollisionPoint(1) <= ey + eh && mGetOn) {

		mNPCEvent->addLeft(1);
		strikenEvent();
		SE::getIns()->playSE(SE_SpecialFlagGet);

	}

}

/*------------------------------------------------------------------------------
�����蔻����W��Ԃ�
------------------------------------------------------------------------------*/
int SpecialFlag::getCollisionPoint(int aPos) const
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
eTag SpecialFlag::getTag() const
{
	return Tag_Item;
}


