#pragma once

//-------------------------------------------------------------------------------------------------
// fps����
class Fps
{
private:
	int mStartTime;			   // ����J�n����
	int mCount;				   // �J�E���^
	float mFps;				   // fps
	static const int N = 60;   // ���ς����T���v����
	static const int FPS = 60; // �ݒ肵��FPS

public:
	void draw();
	void wait();

	Fps();
	bool update();

};