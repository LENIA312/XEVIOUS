#pragma warning(disable:26812)

#include <DxLib.h>
#include "Camera.h"
#include "Define.h"
#include "Mouse.h"
#include "Pad.h"

/*------------------------------------------------------------------------------
コンストラクタ
------------------------------------------------------------------------------*/
Camera::Camera()
{
	mMode = 0;
	mY = (float)Define::MapHeight + (float)Define::ForestHeight - (float)Define::GameScreenHeight;
}

/*------------------------------------------------------------------------------
デストラクタ
------------------------------------------------------------------------------*/
Camera::~Camera()
{
}

/*------------------------------------------------------------------------------
更新
------------------------------------------------------------------------------*/
void Camera::update()
{
	switch (mMode) {
	case 0: // ゲーム
		mY -= Define::ScrollSpeed;
		if (mY < 0) {
			mY += Define::MapHeight;
		}
		break;

	case 1: // エディター
		// ホイール
		if (Mouse::getIns()->getWheel() > 0) {
			mY -= 8;
		}
		if (Mouse::getIns()->getWheel() < 0) {
			mY += 8;
		}
		if (Pad::getIns()->get(Pad_Up)) {
			mY -= 32;
		}
		if (Pad::getIns()->get(Pad_Down)) {
			mY += 32;
		}
		// マップ内にカメラを抑える
		if (mY < 0) {
			mY = 0;
		}
		if (mY > Define::MapHeight - Define::GameScreenHeight + 6) {
			mY = (float)Define::MapHeight - (float)Define::GameScreenHeight + 6;
		}
		break;
	}
}

/*------------------------------------------------------------------------------
描画
------------------------------------------------------------------------------*/
void Camera::draw() const
{
	DrawFormatString(0, 0, GetColor(255, 255, 255), "\n\nCameraY:%0.1f", mY);
}

/*------------------------------------------------------------------------------
カメラのyを返す
------------------------------------------------------------------------------*/
float Camera::getY() const
{
	return mY;
}

/*------------------------------------------------------------------------------
リセット
------------------------------------------------------------------------------*/
void Camera::reset()
{
	mY = (float)Define::MapHeight + (float)Define::ForestHeight - (float)Define::GameScreenHeight;
}

/*------------------------------------------------------------------------------
モードセット
------------------------------------------------------------------------------*/
void Camera::setMode(int aMode)
{
	mMode = aMode;
	if (aMode == 1) {
		int tmp = (int)mY / 8 * 8;
  		mY = (float)tmp;
	}
}
