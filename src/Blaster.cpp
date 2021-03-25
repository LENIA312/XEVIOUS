#include <DxLib.h>
#include "Blaster.h"
#include "Image.h"
#include "SE.h"
#include "Define.h"

/*
// �G���A�̕ς��ڂɓ����蔻����ړ�
if (by <= Define::GAME_H && mCameraY > Define::MAP_HEIGHT - Define::GAME_H) {
	by += Define::MAP_HEIGHT;
}
*/

/*------------------------------------------------------------------------------
��`
------------------------------------------------------------------------------*/
const static float BLASTER_SPEED = (float)0.15; // memo:0.15
const static float BLASTER_ANIME_SPEED = (float)0.26;

/*------------------------------------------------------------------------------
�R���X�g���N�^
------------------------------------------------------------------------------*/
Blaster::Blaster()
{
	mBlasterSpeed = 0;
	mTargetY = 0;

	mIsHit = false;
	mActiveFlag = false;
}

/*------------------------------------------------------------------------------
�f�X�g���N�^
------------------------------------------------------------------------------*/
Blaster::~Blaster()
{

}

/*------------------------------------------------------------------------------
�X�V
------------------------------------------------------------------------------*/
void Blaster::update()
{
	// �q�b�g������t���O���I�t��
	if (mIsHit) {
		mIsHit = false;
		mActiveFlag = false;
	}
	if (mActiveFlag) {
		// �^�[�Q�b�g�𒴂���܂�
		if (mY > mTargetY) {
			// �ړ�
			{
				mY -= mBlasterSpeed;
				mBlasterSpeed += BLASTER_SPEED;
				// �␳
				if (mY < mTargetY) {
					mY = mTargetY;
				}
			}
			// �A�j��
			{
				mAnime += BLASTER_ANIME_SPEED;
				// �␳
				if (mAnime > 8) {
					mAnime = 8;
				}
			}
			// �G���A�̕ς��ڂɍ��W��␳����
			if (mTargetY <= Define::GameScreenHeight && mCameraY > Define::MapHeight - Define::GameScreenHeight) {
				mTargetY += Define::MapHeight;
			}
			if (mY <= Define::GameScreenHeight && mCameraY > Define::MapHeight - Define::GameScreenHeight) {
				mY += Define::MapHeight;
			}
		}
		// �^�[�Q�b�g�𒴂�����
		else {
			mY = mTargetY;
			mIsHit = true;
		}
	}
}

/*------------------------------------------------------------------------------
�`��
------------------------------------------------------------------------------*/
void Blaster::draw() const
{
	if (mActiveFlag) {
		// �^�[�Q�b�g�Ə�
		DrawGraph((int)mX, (int)mTargetY - (int)mCameraY,
			Image::getIns()->getImage(Image_Aiming, 4), TRUE);
		// �u���X�^�[
		DrawGraph((int)mX, (int)mY - (int)mCameraY,
			Image::getIns()->getImage(Image_Blaster, (int)mAnime), TRUE);
	}
}

/*------------------------------------------------------------------------------
�����蔻����W��Ԃ�
------------------------------------------------------------------------------*/
int Blaster::getCollisionPoint(int aPos) const
{
	switch (aPos) {
	case 0: // x
		return (int)mX;
	case 1: // y
		return (int)mY;
	case 2: // w
		return 12;
	case 3: // h
		return 12;
	}
	return -1;
}

/*------------------------------------------------------------------------------
���̃Q�[���I�u�W�F�N�g�Ƃ̓����蔻��
------------------------------------------------------------------------------*/
void Blaster::collisionGameObject(eTag aTag)
{

}

/*------------------------------------------------------------------------------
�^�O��Ԃ�
------------------------------------------------------------------------------*/
eTag Blaster::getTag() const
{
	return Tag_Blaster;
}

/*------------------------------------------------------------------------------
�u���X�^�[�𐶐�����
------------------------------------------------------------------------------*/
void Blaster::generate(int aX, int aY)
{
	// ���W���Z�b�g
	mX = (float)aX;
	mY = (float)aY + mCameraY;
	mTargetY = mY - 96;
	// �t���O���I��
	mBlasterSpeed = 0;
	mActiveFlag = true;
	mIsHit = false;
	mAnime = 0;
	// ���ʉ��Đ�
	SE::getIns()->playSE(SE_Blaster);
}

/*------------------------------------------------------------------------------
�q�b�g�t���O��Ԃ�
------------------------------------------------------------------------------*/
bool Blaster::getHitFlag()
{
	return mIsHit;
}