#pragma once

//-------------------------------------------------------------------------------------------------
#include "Singleton.h"
#include <array>

//-------------------------------------------------------------------------------------------------
// キーボードクラス
class Keyboard final : public Singleton<Keyboard>
{
	Keyboard();
	friend Singleton< Keyboard >;

public:
	void initialize();
	bool update();

	int getPressingCount(int aKeyCode);
	int getReleasingCount(int aKeyCode);

private:
	// keyCodeが有効なキー番号か問う
	bool isAvailableCode(int aKeyCode);

	// キー総数
	static const int KEY_NUM = 256;
	// 押されカウンタ
    std::array<int, KEY_NUM> mPressingCount;
	// 離されカウンタ
    std::array<int, KEY_NUM> mReleasingCount;

};
