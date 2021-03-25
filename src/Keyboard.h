#pragma once

//-------------------------------------------------------------------------------------------------
#include "Singleton.h"
#include <array>

//-------------------------------------------------------------------------------------------------
// �L�[�{�[�h�N���X
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
	// keyCode���L���ȃL�[�ԍ����₤
	bool isAvailableCode(int aKeyCode);

	// �L�[����
	static const int KEY_NUM = 256;
	// ������J�E���^
    std::array<int, KEY_NUM> mPressingCount;
	// ������J�E���^
    std::array<int, KEY_NUM> mReleasingCount;

};
