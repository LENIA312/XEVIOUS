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
	// マップモード 0:普通 1:エディター
	int mMode;
	// y座標
	float mCameraY;
	float mTmpCameraY;
	int mForestY;
	int mBackgroundY[2];
	// 森表示フラグ
	bool mIsForest;
	// エリア毎のx座標配列
	int mAreaPoint[16];
	// 現在のエリア
	int mNowArea[2];
	// 再開するエリア
	int mRestartArea;
	bool mCanAddRestarArea;

};