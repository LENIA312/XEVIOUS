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


	float mX[2], mY[2]; // ���W
	float mTheta[2];
	float mVx[2], mVy[2]; // �x�N�g��
	float mSpeed;          // �ړ��X�s�[�h
	float mAnimSpeed;      // �A�j���Đ��X�s�[�h

	int mAnimP[2]; // �A�j���[�V�����Đ��|�C���g
	int mBetX;
	int mAnimMax;          // �A�j���Đ���
	int mTT;               // �^�C�}�[
	int mMotionNum;		   // ���݂̃��[�V�����ԍ�
	int mFlyCnt;

	bool mTransFlg[2];	// ���@�ƍ��̂������ǂ���

	void init();

};