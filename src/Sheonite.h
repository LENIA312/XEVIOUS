#pragma once

#include "BaseNPC.h"

class Sheonite : public BaseNPC
{
public:
	Sheonite(InterfaceNPCEvent* aNPCEvent, NPCDATA aData, Player* aPlayer);
	~Sheonite();
	void update() override;
	void draw() const override;
	int getCollisionPoint(int aPos) const;
	eTag getTag() const override;

private:


	float mX[2], mY[2]; // 座標
	float mTheta[2];
	float mVx[2], mVy[2]; // ベクトル
	float mSpeed;          // 移動スピード
	float mAnimSpeed;      // アニメ再生スピード

	int mAnimP[2]; // アニメーション再生ポイント
	int mBetX;
	int mAnimMax;          // アニメ再生数
	int mTT;               // タイマー
	int mMotionNum;		   // 現在のモーション番号
	int mFlyCnt;

	bool mTransFlg[2];	// 自機と合体したかどうか

	void init();

};