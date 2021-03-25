#include <DxLib.h>
#include "AbstractGameObject.h"

/*------------------------------------------------------------------------------
�R���X�g���N�^
------------------------------------------------------------------------------*/
AbstractGameObject::AbstractGameObject()
{
	mX = 0;
	mY = 0;
	mCameraY = 0;
	mAnime = 0;
	mLayer = 0;
	mActiveFlag = true;
}

/*------------------------------------------------------------------------------
�f�X�g���N�^
------------------------------------------------------------------------------*/
AbstractGameObject::~AbstractGameObject()
{
}

/*------------------------------------------------------------------------------
�����蔻��`��
------------------------------------------------------------------------------*/
void AbstractGameObject::drawCollision(int r,int g,int b) const
{
	int x = getCollisionPoint(0);
	int y = getCollisionPoint(1) - (int)mCameraY;
	int w = getCollisionPoint(2);
	int h = getCollisionPoint(3);
	DrawBox(x, y, x + w, y + h, GetColor(r, g, b), FALSE);
}

/*------------------------------------------------------------------------------
x,y���W��Ԃ�
------------------------------------------------------------------------------*/
int AbstractGameObject::getXY(int aXY)
{
	switch (aXY) {
	case 0:
		return (int)mX;
	case 1:
		return (int)mY;
	case 2:
		return (int)mY + (int)mCameraY;
	}
	return -1;
}

/*------------------------------------------------------------------------------
�����Ă��邩�ǂ����̃t���O��Ԃ�
------------------------------------------------------------------------------*/
bool AbstractGameObject::checkActiveFlag()
{
	return mActiveFlag;
}

/*------------------------------------------------------------------------------
�J�����̍��W���Z�b�g����
------------------------------------------------------------------------------*/
void AbstractGameObject::setCameraY(float aY)
{
	mCameraY = aY;
}

/*------------------------------------------------------------------------------
------------------------------------------------------------------------------*/
void AbstractGameObject::collisionGameObject(eTag aTag)
{
}
int AbstractGameObject::getCollisionPoint(int aPos) const
{
	return -1;
}