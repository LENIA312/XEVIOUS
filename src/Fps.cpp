#include "Fps.h"

//-------------------------------------------------------------------------------------------------
#include <DxLib.h>
#include <math.h>

//-------------------------------------------------------------------------------------------------
// コンストラクタ
Fps::Fps()
{
	mStartTime = 0;
	mCount = 0;
	mFps = 60;
}

//-------------------------------------------------------------------------------------------------
// 更新
bool Fps::update()
{
	// 1フレーム目なら時刻を記憶
	if (mCount == 0) {
		mStartTime = GetNowCount();
	}
	// 60フレーム目なら平均を計算する
	if (mCount == N) {
		int t = GetNowCount();
		mFps = 1000.f / ((t - mStartTime) / (float)N);
		mCount = 0;
		mStartTime = t;
	}
	mCount++;
	return true;
}

//-------------------------------------------------------------------------------------------------
// 現在のFPSの表示
void Fps::draw()
{
	DrawFormatString(0, 0, GetColor(0, 255, 0), "%2.0ffps", mFps);
}

//-------------------------------------------------------------------------------------------------
// 60FPSを超えていたら待つ
void Fps::wait()
{
	// かかった時間
	int tookTime = GetNowCount() - mStartTime;
	// 待つべき時間
	int waitTime = mCount * 1000 / FPS - tookTime;
	// 待機
	if (waitTime > 0) {
		Sleep(waitTime);
	}
}