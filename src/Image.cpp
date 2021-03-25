#pragma warning(disable:26812)

//-------------------------------------------------------------------------------------------------
#include "Image.h"

//-------------------------------------------------------------------------------------------------
#include <DxLib.h>

//-------------------------------------------------------------------------------------------------
// コンストラクタ
Image::Image()
{
}

//-------------------------------------------------------------------------------------------------
// デストラクタ
Image::~Image()
{
	// 今までロードした画像を解放する
	for (int i = 0; i < mImages.size(); i++) {
		DeleteGraph(mImages[i]);
	}
	mImages.clear();
}

//-------------------------------------------------------------------------------------------------
// 読み込み
void Image::initialize()
{
	myLoadDivGraph("res/img/mouse.png", 4, 4, 1, 16, 16, mMouse);
	myLoadDivGraph("res/img/xev_font_a.png", 126, 18, 7, 8, 8, mFont_a);
	myLoadDivGraph("res/img/xev_font_b.png", 126, 18, 7, 8, 8, mFont_b);
	myLoadDivGraph("res/img/fade.png", 15, 15, 1, 16, 16, mFade);

	mTitleRogo = myLoadGraph("res/img/title_logo.png");
	mNamco = myLoadGraph("res/img/namco.png");
	mForest = myLoadGraph("res/img/forest.png");
	mBackground = myLoadGraph("res/img/background.png");
	mPlayer = myLoadGraph("res/img/player.png");
	mSpecialFlag = myLoadGraph("res/img/special_flag.png");
	mEnemyBox = myLoadGraph("res/img/enemy_box.png");
	mSelect = myLoadGraph("res/img/select.png");
	mBossIcon = myLoadGraph("res/img/boss_icon.png");
	mSaved = myLoadGraph("res/img/saved.png");
	mTitleRogoEditor = myLoadGraph("res/img/title_logo_editor.png");

	myLoadDivGraph("res/img/aiming.png", 5, 5, 1, 16, 16, mAiming);
	myLoadDivGraph("res/img/player_shot.png", 27, 9, 3, 16, 16, mPlayerShot);
	myLoadDivGraph("res/img/enemy_a.png", 80, 8, 10, 16, 16, mEnemyA);
	myLoadDivGraph("res/img/enemy_b.png", 80, 8, 10, 16, 16, mEnemyB);
	myLoadDivGraph("res/img/enemy_c.png", 80, 8, 10, 16, 16, mEnemyC);
	myLoadDivGraph("res/img/enemy_d.png", 80, 8, 10, 16, 16, mEnemyD);
	myLoadDivGraph("res/img/enemy_e.png", 80, 8, 10, 16, 16, mEnemyE);
	myLoadDivGraph("res/img/enemy_big_a.png", 24, 8, 3, 32, 32, mEnemyBigA);
	myLoadDivGraph("res/img/enemy_big_b.png", 24, 8, 3, 32, 32, mEnemyBigB);
	myLoadDivGraph("res/img/enemy_big_c.png", 24, 8, 3, 32, 32, mEnemyBigC);
	myLoadDivGraph("res/img/enemy_big_d.png", 24, 8, 3, 32, 32, mEnemyBigD);
	myLoadDivGraph("res/img/boss.png", 6, 3, 2, 96, 96, mBoss);
	myLoadDivGraph("res/img/boss_parts.png", 25, 5, 5, 16, 16, mBossParts);
	myLoadDivGraph("res/img/zakato_efect.png", 30, 5, 6, 32, 32, mZakatoEfect);
	myLoadDivGraph("res/img/enemy_striken.png", 12, 6, 2, 32, 32, mEnemyStriken);
	myLoadDivGraph("res/img/destructed.png", 2, 2, 1, 16, 16, mDestructed);
	myLoadDivGraph("res/img/player_broken.png", 7, 7, 1, 32, 32, mPlayerBroken);
	myLoadDivGraph("res/img/level_editor_button.png", 3, 1, 3, 96, 16, mLevelEditorButton);
	myLoadDivGraph("res/img/enemy_bullet.png", 40, 10, 4, 16, 16, mEnemyBullet);
	myLoadDivGraph("res/img/area_change_button.png", 4, 4, 1, 15, 15, mAreaChangeButton);
	myLoadDivGraph("res/img/difficulty_meter.png", 15, 5, 3, 15, 15, mDifficultyMeter);
	myLoadDivGraph("res/img/editor_button.png", 15, 5, 3, 38, 23, mEditorButton);
	myLoadDivGraph("res/img/enemy_button.png", 7, 7, 1, 23, 23, mEnemyButton);
	myLoadDivGraph("res/img/enemy_icon.png", 18, 3, 6, 16, 16, mEnemyIcon);
	myLoadDivGraph("res/img/enemy_big_icon.png", 4, 2, 2, 32, 32, mEnemyBigIcon);
	myLoadDivGraph("res/img/number.png", 10, 5, 2, 5, 7, mNumber);
}

//-------------------------------------------------------------------------------------------------
// 画像を取得する
int Image::getImage(eImage aImg, int aNum) const
{
	switch (aImg) {
	case Image_Mouse:
		return mMouse[aNum];
	case Image_Font_a:
		return mFont_a[aNum];
	case Image_Font_b:
		return mFont_b[aNum];
	case Image_Fade:
		return mFade[aNum];

	case Image_TitleRogo:
		return mTitleRogo;
	case Image_Namco:
		return mNamco;
	case Image_LevelEditorButton:
		return mLevelEditorButton[aNum];
	case Image_TitleRogoEditor:
		return mTitleRogoEditor;

	case Image_Player:
		return mPlayer;
	case Image_Aiming:
		return mAiming[aNum];
	case Image_Forest:
		return mForest;
	case Image_Background:
		return mBackground;
	case Image_Zapper:
		return mPlayerShot[aNum];
	case Image_Blaster:
		return mPlayerShot[18 + aNum];

	case Image_Enemy_a:
		return mEnemyA[aNum];
	case Image_Enemy_b:
		return mEnemyB[aNum];
	case Image_Enemy_c:
		return mEnemyC[aNum];
	case Image_Enemy_d:
		return mEnemyD[aNum];
	case Image_Enemy_e:
		return mEnemyE[aNum];
	case Image_EnemyBig_a:
		return mEnemyBigA[aNum];
	case Image_EnemyBig_b:
		return mEnemyBigB[aNum];
	case Image_EnemyBig_c:
		return mEnemyBigC[aNum];
	case Image_EnemyBig_d:
		return mEnemyBigD[aNum];
	case Image_Boss:
		return mBoss[aNum];
	case Image_BossParts:
		return mBossParts[aNum];
	case Image_SpecialFlag:
		return mSpecialFlag;
	case Image_EnemyBullet:
		return mEnemyBullet[aNum];
	case Image_ZakatoEfect:
		return mZakatoEfect[aNum];

	case Image_EnemyStriken:
		return mEnemyStriken[aNum];
	case Image_Destructed:
		return mDestructed[aNum];
	case Image_PlayerBroken:
		return mPlayerBroken[aNum];

	case Image_AreaChangeButton:
		return mAreaChangeButton[aNum];
	case Image_DifficultyMeter:
		return mDifficultyMeter[aNum];
	case Image_EnemyBox:
		return mEnemyBox;
	case Image_EnemyButton:
		return mEnemyButton[aNum];
	case Image_EditorButton:
		return mEditorButton[aNum];
	case Image_EnemyIcon:
		return mEnemyIcon[aNum];
	case Image_BossIcon:
		return mBossIcon;
	case Image_EnemyBigIcon:
		return mEnemyBigIcon[aNum];
	case Image_Select:
		return mSelect;
	case Image_Number:
		return mNumber[aNum];
	case Image_Saved:
		return mSaved;
	}
	return Image_None;
}

//-------------------------------------------------------------------------------------------------
// LoadGraphをして、かつそのハンドルをメンバ変数に追加する
int Image::myLoadGraph(const char* aFileName)
{
    int ret = LoadGraph(aFileName);
    mImages.push_back(ret);
    return ret;
}

//-------------------------------------------------------------------------------------------------
// LoadDivGraphをして、かつそのハンドルをメンバ変数に追加する
int Image::myLoadDivGraph(const char* aFileName, int aN, int aXN, int aYN, int aW, int aH, int* aBuf)
{
    int ret = LoadDivGraph(aFileName, aN, aXN, aYN, aW, aH, aBuf);
    for (int i = 0; i < aN; i++) {
        mImages.push_back(aBuf[i]);
    }
    return ret;
}