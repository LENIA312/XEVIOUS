#include <DxLib.h>
#include "Keyboard.h"

//-------------------------------------------------------------------------------------------------
// コンストラクタ
Keyboard::Keyboard()
{
}

//-------------------------------------------------------------------------------------------------
// 初期化処理
void Keyboard::initialize()
{
	mPressingCount[0] = 0;
	mReleasingCount[0] = 0;
	for (int i = 0; i < 256; i++) {
		mPressingCount[i] = 0;
		mReleasingCount[i] = 0;
	}
}

//-------------------------------------------------------------------------------------------------
// 更新
bool Keyboard::update() {
	char nowKeyStatus[KEY_NUM];
	//今のキーの入力状態を取得
	GetHitKeyStateAll(nowKeyStatus);
	for (int i = 0; i<KEY_NUM; i++) {
		//i番のキーが押されていたら
		if (nowKeyStatus[i] != 0) {
			if (mReleasingCount[i] > 0) {
				// 離されカウンタが0より大きければ0に戻す
				mReleasingCount[i] = 0;
			}
			// 押されカウンタを増やす
			mPressingCount[i]++;
		}
		//i番のキーが離されていたら
		else {
			if (mPressingCount[i] > 0) {
				// 押されカウンタが0より大きければ0に戻す
				mPressingCount[i] = 0;
			}
			//離されカウンタを増やす
			mReleasingCount[i]++;
		}
	}
	return true;
}

//-------------------------------------------------------------------------------------------------
// keyCodeのキーが押されているフレーム数を返す
int Keyboard::getPressingCount(int aKeyCode) {
	if (!isAvailableCode(aKeyCode)) {
		return -1;
	}
	return mPressingCount[aKeyCode];
}

//-------------------------------------------------------------------------------------------------
// keyCodeのキーが離されているフレーム数を返す
int Keyboard::getReleasingCount(int aKeyCode) {
	if (!isAvailableCode(aKeyCode)) {
		return -1;
	}
	return mReleasingCount[aKeyCode];
}

//-------------------------------------------------------------------------------------------------
// keyCodeが有効な値か否かを返す
bool Keyboard::isAvailableCode(int aKeyCode) {
	if (!(0 <= aKeyCode && aKeyCode<KEY_NUM)) {
		return false;
	}
	return true;
}
