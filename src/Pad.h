#pragma once

//-------------------------------------------------------------------------------------------------
#include "Singleton.h"
#include <array>

//-------------------------------------------------------------------------------------------------
// ����̗񋓌^
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
// �p�b�h���� / �L�[�{�[�h���܂�
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
	// �ǂ̃{�^�����ǂ̃{�^���Ɋ��蓖�����Ă��邩������
    std::array<int, PAD_KEY_NUM> mIdArray;
	// pad���͏�Ԋi�[
    std::array<int, PAD_KEY_NUM> mPad;


};
