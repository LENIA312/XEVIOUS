#include <DxLib.h>
#include "GaruZakato.h"
#include "Image.h"
#include "Define.h"
#include "SE.h"

/*------------------------------------------------------------------------------
��`
------------------------------------------------------------------------------*/
static const float SPEED = (float)3;

/*------------------------------------------------------------------------------
�R���X�g���N�^
------------------------------------------------------------------------------*/
GaruZakato::GaruZakato(InterfaceNPCEvent* aNPCEvent, NPCDATA aData, Player* aPlayer) : BaseNPC(aNPCEvent)
{
	mPtn = aData.ptn;
	// �o���ʒu�̓����_��
	mX = (float)(GetRand(256) + 48);
	mY = (float)aData.y - 16;
	mPlayer = aPlayer;

	mTimer = 0;
	mBakuhatuTimer = GetRand(30) + 10;
}

/*------------------------------------------------------------------------------
�f�X�g���N�^
------------------------------------------------------------------------------*/
GaruZakato::~GaruZakato()
{

}

/*------------------------------------------------------------------------------
�X�V
------------------------------------------------------------------------------*/
void GaruZakato::update()
{
	mY += SPEED;
	if (mTimer >= mBakuhatuTimer) {
		// �X�p���I����
		SE::getIns()->playSE(SE_GaruZakatoExplosion);
		mNPCEvent->generateEnemyBullet(2, 0, (int)mX + 8, (int)mY + 8);
		mActiveFlag = false;
	}
	mTimer++;
	// �|����Ă��Ȃ���
	if (!mIsStriken) {
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
void GaruZakato::draw() const
{
	// �ʏ�
	if (!mIsStriken) {
		DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_EnemyBullet, 35 + (int)mColor), true);
	}
	// �j��
	else {
		DrawGraph((int)mX - 8, (int)mY - 8 - (int)mCameraY, Image::getIns()->getImage(Image_EnemyStriken, (int)mAnime), true);
	}
	//clsDx();
	//printfDx("%0.1f %0.1f", mX, mY);
}

/*------------------------------------------------------------------------------
�|���ꂽ�Ƃ���
------------------------------------------------------------------------------*/
void GaruZakato::strikenEvent()
{
	mAnime += (float)0.8;
	if (mAnime > 5) {
		mActiveFlag = false;
	}
}

/*------------------------------------------------------------------------------
�U�b�p�[�ɓ���������
------------------------------------------------------------------------------*/
void GaruZakato::collisionGameObject(eTag aTag)
{
	if (aTag == Tag_Zapper && !mIsStriken) {
		// �|���ꂽ�t���O�𗧂Ă�
		mIsStriken = true;
		// �A�j���ʒu��������
		mAnime = 0;
		// �X�R�A�����Z����
		mNPCEvent->addScore(1000);
		// ���ʉ��Đ�
		SE::getIns()->playSE(SE_EnemyStrikenZapper);
	}
}

/*------------------------------------------------------------------------------
�U�b�p�[�Ƃ̓����蔻��
------------------------------------------------------------------------------*/
void GaruZakato::collision()
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
int GaruZakato::getCollisionPoint(int aPos) const
{
	switch (aPos) {
	case 0: // x
		return (int)mX + 2;
	case 1: // y
		return (int)mY + 2;
	case 2: // w
		return 12;
	case 3: // h
		return 12;
	}
	return -1;
}

/*------------------------------------------------------------------------------
�G���A���ς��u�ԂɎ��s�����
------------------------------------------------------------------------------*/
void GaruZakato::areaChange()
{
	// �������Ȃ�
}

/*------------------------------------------------------------------------------
�^�O��Ԃ�
------------------------------------------------------------------------------*/
eTag GaruZakato::getTag() const
{
	return Tag_Enemy_Sky;
}