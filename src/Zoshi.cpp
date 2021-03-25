#include <DxLib.h>
#include "Zoshi.h"
#include "Image.h"
#include "Define.h"
#include "SE.h"

/*------------------------------------------------------------------------------
��`
------------------------------------------------------------------------------*/
static const float SPEED = (float)1.4;

/*------------------------------------------------------------------------------
�R���X�g���N�^
------------------------------------------------------------------------------*/
Zoshi::Zoshi(InterfaceNPCEvent* aNPCEvent, NPCDATA aData, Player* aPlayer) : BaseNPC(aNPCEvent)
{
	mPtn = aData.ptn;
	// �o���ʒu�̓����_��
	mX = (float)(GetRand(256) + 48);
	mY = (float)aData.y - 16;
	mPlayer = aPlayer;

	mAngleChangeTime = 0;
	mSparioTimer = 0;
	mInitFlg = false;
	mTheta = 0;
}

/*------------------------------------------------------------------------------
�f�X�g���N�^
------------------------------------------------------------------------------*/
Zoshi::~Zoshi()
{

}

/*------------------------------------------------------------------------------
�X�V
------------------------------------------------------------------------------*/
void Zoshi::update()
{
	// ������
	if (!mInitFlg) {
		mInitFlg = true;
		// �G�f�B�^�[�p�̏��� (��ʊO�Ńe�X�g�v���C�����Ƃ��ɐ�������Ȃ��Ȃ�)
		if (mY - Define::GameScreenHeight > mCameraY) {
			mActiveFlag = false;
		} // �G�f�B�^�[�̏����I��
		// �p�^�[����2�ȏ�Ȃ牺����o��
		if (mPtn >= 2) {
			mY += Define::GameScreenHeight + 16;
		}
		mY -= mCameraY;
		if (mY > 270) {
			mY = 270;
		}
		// �p�x
		mTheta = atan2(mPlayer->getXY(2) - (mY + (int)mCameraY), mPlayer->getXY(0) - mX);
	}
	// �|����Ă��Ȃ���
	if (!mIsStriken) {
		// �ړ�
		mX += cos(mTheta) * SPEED;
		mY += sin(mTheta) * SPEED;
		// �p�^�[��1�ȏ�Ȃ�X�p���I����
		if (mPtn >= 1) {
			int r = GetRand(60);
			// 60����1�̊m���Œe������
			if (mSparioTimer >= 60 && r == 0) {
				mNPCEvent->generateEnemyBullet(0, 0, (int)mX + 8, (int)mY + 8 + (int)mCameraY);
				mSparioTimer = 0;
			}
			mSparioTimer++;
		}
		// �ړ��p�x��ς���
		{
			if (mAngleChangeTime >= 90) {
				mAngleChangeTime = 0;
				int r = 0;
				if (mPtn == 0) {
					r = GetRand(5);
				}
				else if (mPtn == 1) {
					r = GetRand(3);
				}
				else if (mPtn >= 2) {
					r = GetRand(2);
				}
				if (r == 0) {
					mTheta = atan2(mPlayer->getXY(2) - (mY + (int)mCameraY), mPlayer->getXY(0) - mX);
				}
			}
			mAngleChangeTime++;
		}
		// �A�j��
		if (!mIsStriken) {
			mAnime += (float)0.8;
			if (mAnime >= 4) {
				mAnime = 0;
			}
		}
		collision();
	}
	// �|���ꂽ��
	else {
		strikenEvent();
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
void Zoshi::draw() const
{
	// �ʏ�
	if (!mIsStriken) {
		switch (mColor) {
		case 0:
			DrawGraph((int)mX, (int)mY, Image::getIns()->getImage(Image_Enemy_a, 32 + (int)mAnime), true);
			break;
		case 1:
			DrawGraph((int)mX, (int)mY, Image::getIns()->getImage(Image_Enemy_b, 32 + (int)mAnime), true);
			break;
		case 2:
			DrawGraph((int)mX, (int)mY, Image::getIns()->getImage(Image_Enemy_c, 32 + (int)mAnime), true);
			break;
		case 3:
			DrawGraph((int)mX, (int)mY, Image::getIns()->getImage(Image_Enemy_d, 32 + (int)mAnime), true);
			break;
		case 4:
			DrawGraph((int)mX, (int)mY, Image::getIns()->getImage(Image_Enemy_e, 32 + (int)mAnime), true);
			break;
		}
	}
	// �j��
	else {
		DrawGraph((int)mX - 8, (int)mY - 8, Image::getIns()->getImage(Image_EnemyStriken, (int)mAnime), true);
	}
	//clsDx();
	//printfDx("%0.1f %0.1f", mX, mY);
}

/*------------------------------------------------------------------------------
�|���ꂽ�Ƃ���
------------------------------------------------------------------------------*/
void Zoshi::strikenEvent()
{
	mAnime += (float)0.8;
	if (mAnime > 5) {
		mActiveFlag = false;
	}
}

/*------------------------------------------------------------------------------
�U�b�p�[�ɓ���������
------------------------------------------------------------------------------*/
void Zoshi::collisionGameObject(eTag aTag)
{
	if (aTag == Tag_Zapper && !mIsStriken) {
		// �|���ꂽ�t���O�𗧂Ă�
		mIsStriken = true;
		// �A�j���ʒu��������
		mAnime = 0;
		// �X�R�A�����Z����
		if (mPtn == 0) {
			mNPCEvent->addScore(70);
		}
		else if (mPtn == 1) {
			mNPCEvent->addScore(80);
		}
		else if (mPtn >= 2) {
			mNPCEvent->addScore(100);
		}
		// ���ʉ��Đ�
		SE::getIns()->playSE(SE_EnemyStrikenZapper);
	}
}

/*------------------------------------------------------------------------------
�U�b�p�[�Ƃ̓����蔻��
------------------------------------------------------------------------------*/
void Zoshi::collision()
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
}

/*------------------------------------------------------------------------------
�����蔻����W��Ԃ�
------------------------------------------------------------------------------*/
int Zoshi::getCollisionPoint(int aPos) const
{
	switch (aPos) {
	case 0: // x
		return (int)mX + 1;
	case 1: // y
		return (int)mY + 1 + (int)mCameraY;
	case 2: // w
		return 14;
	case 3: // h
		return 14;
	}
	return -1;
}

/*------------------------------------------------------------------------------
�G���A���ς��u�ԂɎ��s�����
------------------------------------------------------------------------------*/
void Zoshi::areaChange()
{
	// �������Ȃ�
}

/*------------------------------------------------------------------------------
�^�O��Ԃ�
------------------------------------------------------------------------------*/
eTag Zoshi::getTag() const
{
	return Tag_Enemy_Sky;
}