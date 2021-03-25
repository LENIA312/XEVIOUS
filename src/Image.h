#pragma once

//-------------------------------------------------------------------------------------------------
#include "Singleton.h"
#include <vector>
#include <array>

//-------------------------------------------------------------------------------------------------
// 画像の種類
enum eImage
{
	Image_Mouse,
	Image_Font_a,
	Image_Font_b,
	Image_Fade,

	Image_TitleRogo,
	Image_Namco,
	Image_LevelEditorButton,
	Image_TitleRogoEditor,

	Image_Player,
	Image_Aiming,
	Image_Forest,
	Image_Background,
	Image_Zapper,
	Image_Blaster,

	Image_Enemy_a,
	Image_Enemy_b,
	Image_Enemy_c,
	Image_Enemy_d,
	Image_Enemy_e,
	Image_EnemyBig_a,
	Image_EnemyBig_b,
	Image_EnemyBig_c,
	Image_EnemyBig_d,
	Image_Boss,
	Image_BossParts,
	Image_SpecialFlag,
	Image_EnemyBullet,
	Image_ZakatoEfect,

	Image_EnemyStriken,
	Image_Destructed,
	Image_PlayerBroken,	

	Image_AreaChangeButton,
	Image_DifficultyMeter,
	Image_EnemyBox,
	Image_EnemyButton,
	Image_EditorButton,
	Image_EnemyIcon,
	Image_BossIcon,
	Image_EnemyBigIcon,
	Image_Select,
	Image_Number,
	Image_Saved,

	Image_None,
};

//-------------------------------------------------------------------------------------------------
// イメージクラス
class Image final : public Singleton<Image>
{
public:
    Image();
    ~Image();

	void initialize();

	int getImage(eImage aImg, int aNum = 0) const;

private:
	int myLoadGraph(const char* aFileName);
	int myLoadDivGraph(const char* aFileName, int aN, int aXN, int aYN, int aW, int aH, int* aBuf);

    std::vector<int> mImages;

	int mMouse[4];
	int mFont_a[126];
	int mFont_b[126];
	int mFade[15];

	int mTitleRogo;
	int mNamco;
	int mForest;
	int mBackground;
	int mPlayer;
	int mSelect;
	int mSpecialFlag;
	int mEnemyBox;
	int mTitleRogoEditor;

	int mAiming[5];
	int mPlayerShot[27];
	int mEnemyA[80];
	int mEnemyB[80];
	int mEnemyC[80];
	int mEnemyD[80];
	int mEnemyE[80];
	int mEnemyBigA[24];
	int mEnemyBigB[24];
	int mEnemyBigC[24];
	int mEnemyBigD[24];
	int mBoss[6];
	int mBossParts[25];
	int mEnemyBullet[40];
	int mZakatoEfect[30];

	int mEnemyStriken[12];
	int mDestructed[2];
	int mPlayerBroken[7];

	int mLevelEditorButton[3];
	int mAreaChangeButton[4];
	int mDifficultyMeter[15];
	int mEnemyButton[7];
	int mEditorButton[15];
	int mEnemyIcon[18];
	int mBossIcon;
	int mEnemyBigIcon[4];
	int mNumber[10];
	int mSaved;

};
