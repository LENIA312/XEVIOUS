#include <DxLib.h>
#include "GidoSpario.h"
#include "Image.h"
#include "Define.h"
#include "SE.h"

/*------------------------------------------------------------------------------
�R���X�g���N�^
------------------------------------------------------------------------------*/
GidoSpario::GidoSpario(InterfaceNPCEvent* aNPCEvent, NPCDATA aData, Player* aPlayer) : BaseNPC(aNPCEvent)
{
	mPtn = aData.ptn;
	mPlayer = aPlayer;
	// �o���ʒu�̓����_��
	mX = (float)(GetRand(256) + 48);
	mY = (float)aData.y - 16;
	// �p�x
	mTheta = atan2(mPlayer->getXY(2) - 32 + GetRand(48) - mY, mPlayer->getXY(0) - 16 + GetRand(48) - mX);
	// ���ʉ�
	SE::getIns()->playSE(SE_ZakatoDethout);
}

/*------------------------------------------------------------------------------
�f�X�g���N�^
------------------------------------------------------------------------------*/
GidoSpario::~GidoSpario()
{

}

/*------------------------------------------------------------------------------
�X�V
------------------------------------------------------------------------------*/
void GidoSpario::update()
{
	// �ړ�
	mX += cos(mTheta) * 3;
	mY += sin(mTheta) * 3;
	mAnime++;
	if (mAnime >= 16) {
		mAnime = 0;
	}
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
		mY > mCameraY + Define::GameScreenHeight) {
		mActiveFlag = false;
	}
}

/*------------------------------------------------------------------------------
 �`��
------------------------------------------------------------------------------*/
void GidoSpario::draw() const
{
	// �ʏ�
	if (!mIsStriken) {
		DrawGraph((int)mX, (int)mY - (int)mCameraY, Image::getIns()->getImage(Image_EnemyBullet, 4 + (int)mAnime), true);
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
void GidoSpario::strikenEvent()
{
	mAnime += (float)0.8;
	if (mAnime > 5) {
		mActiveFlag = false;
	}
}

/*------------------------------------------------------------------------------
�U�b�p�[�ɓ���������
------------------------------------------------------------------------------*/
void GidoSpario::collisionGameObject(eTag aTag)
{
	// ���������I�u�W�F�N�g���U�b�p�[�Ȃ�
	if (aTag == Tag_Zapper && !mIsStriken) {
		// �|���ꂽ�t���O�𗧂Ă�
		mIsStriken = true;
		// �A�j���ʒu��������
		mAnime = 0;
		// �X�R�A�����Z����
		mNPCEvent->addScore(10);
		// ���ʉ��Đ�
		SE::getIns()->playSE(SE_EnemyStrikenZapper);
	}
}

/*------------------------------------------------------------------------------
�U�b�p�[�Ƃ̓����蔻��
------------------------------------------------------------------------------*/
void GidoSpario::collision()
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
int GidoSpario::getCollisionPoint(int aPos) const
{
	switch (aPos) {
	case 0: // x
		return (int)mX + 5;
	case 1: // y
		return (int)mY + 5;
	case 2: // w
		return 6;
	case 3: // h
		return 6;
	}
	return -1;
}

/*------------------------------------------------------------------------------
�^�O��Ԃ�
------------------------------------------------------------------------------*/
eTag GidoSpario::getTag() const
{
	return Tag_Enemy_Sky;
}
