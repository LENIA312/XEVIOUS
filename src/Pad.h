#pragma once

//-------------------------------------------------------------------------------------------------
#include "Singleton.h"
#include <array>

//-------------------------------------------------------------------------------------------------
// 操作の列挙型
enum ePad
{
	Pad_Left,
	Pad_Up,
	Pad_Right,
	Pad_Down,
	Pad_Space,
	Pad_LShift,
	Pad_RShift,
	Pad_Z,
	Pad_X,
	Pad_ESC,
};

//-------------------------------------------------------------------------------------------------
// パッド操作 / キーボードも含む
class Pad final : public Singleton<Pad>
{
public:
	Pad();
	~Pad() = default;

	void initialize();
	void update();

	int get(ePad aID) const;

private:
	void merge();

    const static int PAD_KEY_NUM = 16;
	// どのボタンがどのボタンに割り当たっているかを示す
    std::array<int, PAD_KEY_NUM> mIdArray;
	// pad入力状態格納
    std::array<int, PAD_KEY_NUM> mPad;


};
