#include <DxLib.h>
#include "Zakato.h"
#include "Image.h"
#include "Define.h"
#include "SE.h"

/*------------------------------------------------------------------------------
�R���X�g���N�^
------------------------------------------------------------------------------*/
Zakato::Zakato(InterfaceNPCEvent* aNPCEvent, NPCDATA aData, Player* aPlayer) : BaseNPC(aNPCEvent)
{
	mPtn = aData.ptn;
	mPlayer = aPlayer;
	// �o���ʒu�̓����_��
	mX = (float)(GetRand(256) + 48);
	mY = (float)aData.y;
	mY += GetRand(80);
	// �^�C�}�[
	mTimer = 0;
	mBakuhatuTimer = GetRand(80) + 10;
	// �p�x
	mTheta = atan2(mPlayer->getXY(2) - mY, mPlayer->getXY(0) -  mX);
	// �����t���O
	mBakuhatuFlg = false;
	// ���ʉ�
	SE::getIns()->playSE(SE_ZakatoDethout);
}

/*------------------------------------------------------------------------------
�f�X�g���N�^
------------------------------------------------------------------------------*/
Zakato::~Zakato()
{

}

/*------------------------------------------------------------------------------
�X�V
------------------------------------------------------------------------------*/
void Zakato::update()
{
	// �ړ�
	{
		// �������Ă��Ȃ��Ƃ�
		if (!mBakuhatuFlg) {
			if (mAnime <= 5) {
				mAnime += (float)0.3;
			}
			else {
				mX += cos(mTheta);
				mY += sin(mTheta);
				if (mTimer >= mBakuhatuTimer) {
					// �X�p���I����
					mNPCEvent->generateEnemyBullet(0, 0, (int)mX + 8, (int)mY + 8);
					mBakuhatuFlg = true;
					mAnime = 5;
				}
				mTimer++;
			}
		}
		// ���������Ƃ�
		else {
			if (mAnime <= 10) {
				mAnime += (float)0.3;
			}
			else {
				mActiveFlag = false;
			}
		}
	}
	// �|����Ă��Ȃ���
	if (!mIsStriken) {
		if (!mBakuhatuFlg) {
			collision();
		}
	}
	// �|���ꂽ��
	else {
		strikenEvent();
	}
	// ��ʊO�ɏo����t���O��܂�
	if (mX + 16 < 0 ||
		mX > Define::GameScreenWidth - 128 ||
		mY > mCameraY + Define::GameScreenHeight) {
		mActiveFlag = false;
	}
}

/*------------------------------------------------------------------------------
 �`��
------------------------------------------------------------------------------*/
void Zakato::draw() const
{
	// �ʏ�
	if (!mIsStriken) {
		// �������Ă��Ȃ��Ƃ�
		if (!mBakuhatuFlg) {
			if (mAnime <= 5) {
				DrawGraph((int)mX - 8, (int)mY - 8 - (int)mCameraY, Image::getIns()->getImage(Image_ZakatoEfect, (int)mAnime), true);
			}
			else {
				DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_EnemyBullet, 29), true);
			}
		}
		// ���������Ƃ�
		else {
			if (mAnime <= 10) {
				DrawGraph((int)mX - 8, (int)mY - 8 - (int)mCameraY, Image::getIns()->getImage(Image_ZakatoEfect, (int)mAnime), true);
			}
		}
	}
	// �j��
	else {
		DrawGraph((int)mX - 8, ((int)mY - 8) - (int)mCameraY, Image::getIns()->getImage(Image_EnemyStriken, (int)mAnime), true);
	}
	// �f�o�b�O
	//clsDx();
	//printfDx("%0.1f", mAnime);
	//DrawLine(mPlayerX, 0, mPlayerX, 300, GetColor(0, 0, 0));
}

/*------------------------------------------------------------------------------
�|���ꂽ�Ƃ���
------------------------------------------------------------------------------*/
void Zakato::strikenEvent()
{
	mAnime += (float)0.8;
	if (mAnime > 5) {
		mActiveFlag = false;
	}
}

/*------------------------------------------------------------------------------
�U�b�p�[�ɓ���������
------------------------------------------------------------------------------*/
void Zakato::collisionGameObject(eTag aTag)
{
	// ���������I�u�W�F�N�g���U�b�p�[�Ȃ�
	if (aTag == Tag_Zapper && !mIsStriken) {
		// �|���ꂽ�t���O�𗧂Ă�
		mIsStriken = true;
		// �A�j���ʒu��������
		mAnime = 0;
		// �X�R�A�����Z����
		mNPCEvent->addScore((GetRand(2) + 1) * 100);
		// ���ʉ��Đ�
		SE::getIns()->playSE(SE_EnemyStrikenZapper);
	}
}

/*------------------------------------------------------------------------------
�U�b�p�[�Ƃ̓����蔻��
------------------------------------------------------------------------------*/
void Zakato::collision()
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
int Zakato::getCollisionPoint(int aPos) const
{
	switch (aPos) {
	case 0: // x
		return (int)mX + 4;
	case 1: // y
		return (int)mY + 4;
	case 2: // w
		return 8;
	case 3: // h
		return 8;
	}
	return -1;
}

/*------------------------------------------------------------------------------
�^�O��Ԃ�
------------------------------------------------------------------------------*/
eTag Zakato::getTag() const
{
	return Tag_Enemy_Sky;
}
