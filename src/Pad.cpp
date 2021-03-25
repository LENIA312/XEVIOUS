#pragma warning(disable:26812)

//-------------------------------------------------------------------------------------------------
#include "Pad.h"

//-------------------------------------------------------------------------------------------------
#include <DxLib.h>
#include "Keyboard.h"
#include "Define.h"
#include <string>
#include <algorithm>

//-------------------------------------------------------------------------------------------------
using namespace std;

//-------------------------------------------------------------------------------------------------
// コンストラクタ
Pad::Pad()
	: mPad()
	, mIdArray()
{
}

//-------------------------------------------------------------------------------------------------
// 初期化
void Pad::initialize()
{
	mPad[0] = 0;
	mIdArray[ePad::Pad_Up] = 3;
	mIdArray[ePad::Pad_Down] = 0;
	mIdArray[ePad::Pad_Left] = 1;
	mIdArray[ePad::Pad_Right] = 2;
	mIdArray[ePad::Pad_LShift] = 11;
	mIdArray[ePad::Pad_RShift] = 10;
	mIdArray[ePad::Pad_Z] = 6;
	mIdArray[ePad::Pad_X] = 4;
	mIdArray[ePad::Pad_Space] = 13;
	mIdArray[ePad::Pad_ESC] = 12;
}

//-------------------------------------------------------------------------------------------------
// 更新
void Pad::update()
{
    int padInput;
	// パッドの入力状態を取得
	padInput = GetJoypadInputState(DX_INPUT_PAD1);
	for (int i = 0; i<16; i++) {
		if (padInput & (1<<i)) {
			mPad[i]++;
		}
		else {
			mPad[i] = 0;
		}
	}
    merge();
}

//-------------------------------------------------------------------------------------------------
// パッドとそれに対応するキーボードの入力状態をマージする
void Pad::merge()
{
	mPad[mIdArray[ePad::Pad_Up]] = max(mPad[mIdArray[ePad::Pad_Up]], Keyboard::getIns()->getPressingCount(KEY_INPUT_UP));
	mPad[mIdArray[ePad::Pad_Down]] = max(mPad[mIdArray[ePad::Pad_Down]], Keyboard::getIns()->getPressingCount(KEY_INPUT_DOWN));
	mPad[mIdArray[ePad::Pad_Left]] = max(mPad[mIdArray[ePad::Pad_Left]], Keyboard::getIns()->getPressingCount(KEY_INPUT_LEFT));
	mPad[mIdArray[ePad::Pad_Right]] = max(mPad[mIdArray[ePad::Pad_Right]], Keyboard::getIns()->getPressingCount(KEY_INPUT_RIGHT));
	mPad[mIdArray[ePad::Pad_Space]] = max(mPad[mIdArray[ePad::Pad_Space]], Keyboard::getIns()->getPressingCount(KEY_INPUT_SPACE));
	mPad[mIdArray[ePad::Pad_LShift]] = max(mPad[mIdArray[ePad::Pad_LShift]], Keyboard::getIns()->getPressingCount(KEY_INPUT_LSHIFT));
	mPad[mIdArray[ePad::Pad_RShift]] = max(mPad[mIdArray[ePad::Pad_RShift]], Keyboard::getIns()->getPressingCount(KEY_INPUT_RSHIFT));
	mPad[mIdArray[ePad::Pad_Z]] = max(mPad[mIdArray[ePad::Pad_Z]], Keyboard::getIns()->getPressingCount(KEY_INPUT_Z));
	mPad[mIdArray[ePad::Pad_X]] = max(mPad[mIdArray[ePad::Pad_X]], Keyboard::getIns()->getPressingCount(KEY_INPUT_X));
	mPad[mIdArray[ePad::Pad_ESC]] = max(mPad[mIdArray[ePad::Pad_ESC]], Keyboard::getIns()->getPressingCount(KEY_INPUT_ESCAPE));
}

//-------------------------------------------------------------------------------------------------
// 渡されたパッドキー番号の入力フレーム数を返す
int Pad::get(ePad aID) const
{
	return mPad[mIdArray[aID]];
}
