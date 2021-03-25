#pragma once

class Background
{
public:
	Background();
	~Background();
	void update();
	void draw() const;
	void reset();
	int getArea();
	void setCameraY(float aY);
	void setMode(int aMode);
	void addArea(int aAdd);

private:
	// �}�b�v���[�h 0:���� 1:�G�f�B�^�[
	int mMode;
	// y���W
	float mCameraY;
	float mTmpCameraY;
	int mForestY;
	int mBackgroundY[2];
	// �X�\���t���O
	bool mIsForest;
	// �G���A����x���W�z��
	int mAreaPoint[16];
	// ���݂̃G���A
	int mNowArea[2];
	// �ĊJ����G���A
	int mRestartArea;
	bool mCanAddRestarArea;

};