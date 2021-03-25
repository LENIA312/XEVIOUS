#include "Mouse.h"

//-------------------------------------------------------------------------------------------------
#include <DxLib.h>
#include "Define.h"
#include "Image.h"

//-------------------------------------------------------------------------------------------------
// コンストラクタ
Mouse::Mouse()
	: mX(0)
	, mY(0)
	, mAnime(0)
	, mWheel(0)
	, mClickL(0)
	, mClickR(0)
	, mScreenMagnification(0)
	, mIsInScreen(false)
{
}

//-------------------------------------------------------------------------------------------------
// 初期化処理
void Mouse::initialize(int aScreenMagnification)
{
	mScreenMagnification = aScreenMagnification;
}

//-------------------------------------------------------------------------------------------------
// 更新
void Mouse::update()
{
	// マウスの座標取得
	GetMousePoint(&mX, &mY);
	mX /= mScreenMagnification;
	mY /= mScreenMagnification;

	// クリック取得
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
		mClickL++;
	} else {
		mClickL = 0;
	}
	if ((GetMouseInput() & MOUSE_INPUT_RIGHT) != 0) {
		mClickR++;
	} else {
		mClickR = 0;
	}

	// ホイール取得
	mWheel = GetMouseWheelRotVol();

	// 画面内にマウスが存在するかどうか
	if (mX >= 0 && mX <= Define::GameScreenWidth && mY >= 0 && mY <= Define::GameScreenHeight) {
		mIsInScreen = true;
	} else {
		mIsInScreen = false;
	}

	// アニメ位置初期化
	mAnime = 0;
}

//-------------------------------------------------------------------------------------------------
// 描画
void Mouse::draw() const
{
	if (mIsInScreen) {
		DrawGraph(mX, mY, Image::getIns()->getImage(Image_Mouse, mAnime), true);
	}
}

//-------------------------------------------------------------------------------------------------
// 座標を返す
int Mouse::getXY(int aXY)
{
	switch (aXY) {
	case 0:
		return mX;
	case 1:
		return mY;
	}
	return -1;
}

//-------------------------------------------------------------------------------------------------
// クリック状態を返す
int Mouse::getClickLR(int aLR)
{
	switch (aLR) {
	case 0:
		return mClickL;
	case 1:
		return mClickR;
	}
	return -1;
}

//-------------------------------------------------------------------------------------------------
// ホイール数値を返す
int Mouse::getWheel()
{
	return mWheel;
}

//-------------------------------------------------------------------------------------------------
// 画面内にあるかのフラグを返す
bool Mouse::getIsInScreen()
{
	return mIsInScreen;
}

//-------------------------------------------------------------------------------------------------
// アニメ位置をセット
void Mouse::setAnime(int aAnime)
{
	mAnime = aAnime;
}