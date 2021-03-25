#include <DxLib.h>
#include "Spario.h"
#include "Image.h"
#include "Define.h"
#include "SE.h"

/*------------------------------------------------------------------------------
�R���X�g���N�^
------------------------------------------------------------------------------*/
Spario::Spario(InterfaceNPCEvent* aNPCEvent, NPCDATA aData, Player* aPlayer) : BaseNPC(aNPCEvent)
{
	mPtn = aData.ptn;
	mX = (float)aData.x - 8;
	mY = (float)aData.y - 8;
	mPlayer = aPlayer;

	mInitFlg = false;
	mSpeed = 0;

	// �v���C���[�Ƃ̊p�x
	float pai = (float)3.14;
	mTheta = 0;
	if (mPtn <= 5) {
		mTheta = atan2(mPlayer->getXY(2) - mY, mPlayer->getXY(0) - mX);
		switch (mPtn) {
		case 1:
			mTheta -= 60 * (pai / 180);
			break;
		case 2:
			mTheta -= 30 * (pai / 180);
			break;
		case 3:

			break;
		case 4:
			mTheta += 60 * (pai / 180);
			break;
		case 5:
			mTheta += 30 * (pai / 180);
			break;
		}
	}
	else {
		for (int i = 0; i < 16; i++) {
			if (mPtn - 6 == i) {
				mTheta = (i * (float)22.5) * (pai / 180);
				break;
			}
		}
	}
	if (mPtn == 0) {
		mSpeed = (float)1.9;
	}
	else if (mPtn >= 1) {
		mSpeed = (float)2.5;
	}
	else {
		mSpeed = 4;
	}
}

/*------------------------------------------------------------------------------
�f�X�g���N�^
------------------------------------------------------------------------------*/
Spario::~Spario()
{

}

/*------------------------------------------------------------------------------
�X�V
------------------------------------------------------------------------------*/
void Spario::update()
{
	// ������
	if (!mInitFlg) {
		mInitFlg = true;
		mY -= mCameraY;
	}
	// ����
	mX += cos(mTheta) * mSpeed;
	mY += sin(mTheta) * mSpeed;
	// �A�j���[�V����
	mAnime += (float)0.2;
	if (mAnime >= 4) {
		mAnime = 0;
	}
	// ��ʊO�ɏo����t���O��܂�
	if (mX + 16 < 0 ||
		mX > Define::GameScreenWidth - 128 ||
		mY + 16 < 0 ||
		mY > Define::GameScreenHeight) {
		mActiveFlag = false;
	}
}

/*------------------------------------------------------------------------------
 �`��
------------------------------------------------------------------------------*/
void Spario::draw() const
{
	DrawGraph((int)mX, (int)mY, Image::getIns()->getImage(Image_EnemyBullet, (int)mAnime), true);
	// �f�o�b�O
	//DrawFormatString(0, 0, GetColor(255, 255, 255), "theta:%0.1f px:%d", mTheta, mPlayerX);
}

/*------------------------------------------------------------------------------
�G���A�̕ς��ڂɓ����蔻����ړ�����
------------------------------------------------------------------------------*/
void Spario::areaChange()
{
	// �������Ȃ�
}

/*------------------------------------------------------------------------------
�����蔻����W��Ԃ�
------------------------------------------------------------------------------*/
int Spario::getCollisionPoint(int aPos) const
{
	switch (aPos) {
	case 0: // x
		return (int)mX + 6;
	case 1: // y
		return (int)mY + 6 + (int)mCameraY;
	case 2: // w
		return 4;
	case 3: // h
		return 4;
	}
	return -1;
}

/*------------------------------------------------------------------------------
�^�O��Ԃ�
------------------------------------------------------------------------------*/
eTag Spario::getTag() const
{
	return Tag_Enemy_Bullet;
}