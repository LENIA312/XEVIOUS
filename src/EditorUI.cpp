#include <DxLib.h>
#include "EditorUI.h"
#include "SE.h"
#include "Mouse.h"
#include "Xprintf.h"
#include "FadeEffect.h"
#include "Image.h"

/*------------------------------------------------------------------------------
なにがなんだかよくわからなくなった定義
------------------------------------------------------------------------------*/
// 1ページの敵の種類の量（実装する敵の量に変化があった場合、変えればいいのはこっちだけ）
static const int ENEMY_VOLUME_0 = 14;
static const int ENEMY_VOLUME_1 = 4;
static const int ENEMY_VOLUME_2 = 1;
static const int ENEMY_VOLUME_3 = 1;
// 0から始まるページ量の始点と終点（こっちは変えなくてもいい）
static const int ENEMY_START_1 = ENEMY_VOLUME_0;
static const int ENEMY_START_2 = ENEMY_START_1 + ENEMY_VOLUME_1;
static const int ENEMY_START_3 = ENEMY_START_2 + ENEMY_VOLUME_2;
static const int ENEMY_END_0 = ENEMY_VOLUME_0 - 1;
static const int ENEMY_END_1 = ENEMY_END_0 + ENEMY_VOLUME_1;
static const int ENEMY_END_2 = ENEMY_END_1 + ENEMY_VOLUME_2;
static const int ENEMY_END_3 = ENEMY_END_2 + ENEMY_VOLUME_3;

/*------------------------------------------------------------------------------
コンストラクタ
------------------------------------------------------------------------------*/
EditorUI::EditorUI(Camera* aCamera, Background* aBackground)
{
	// 変数
	mMouseX = 0;
	mMouseY = 0;
	mNowArea = 0;
	mIsPushAreaChangeButton[0] = false;
	mIsPushAreaChangeButton[1] = false;
	mDifficulty = 2;
	mBlend = 0;
	mBlendFlg = false;
	mEnemyButtonNum = 0;
	mEnemyNum = 0;
	mEditorButtonState[0] = 0;
	mEditorButtonAnime[0] = 0;
	mEditorButtonState[1] = 0;
	mEditorButtonAnime[1] = 0;
	mEditorButtonState[2] = 0;
	mEditorButtonAnime[2] = 0;
	mEnemyCount = 0;
	mSaveMeter = 0;

	// オブジェクト
	mCamera = aCamera;
	mBackground = aBackground;
}

/*------------------------------------------------------------------------------
デストラクタ
------------------------------------------------------------------------------*/
EditorUI::~EditorUI()
{

}

/*------------------------------------------------------------------------------
更新
------------------------------------------------------------------------------*/
void EditorUI::update()
{
	// 画面内か、UIに触れていない時のみマウス変数に代入
	if (Mouse::getIns()->getIsInScreen() && Mouse::getIns()->getXY(0) < 352) {
		mMouseX = Mouse::getIns()->getXY(0);
		mMouseY = Mouse::getIns()->getXY(1);
	}
	// 現在のエリアを代入
	mNowArea = mBackground->getArea();
	// ブレンド
	{
		if (mBlendFlg == false) {
			mBlend += 5;
		}
		else {
			mBlend -= 5;
		}
		if (mBlend > 255) {
			mBlendFlg = true;
		}
		if (mBlend < 0) {
			mBlendFlg = false;
		}
	}
	// マウス取得
	int mouse_x = Mouse::getIns()->getXY(0);
	int mouse_y = Mouse::getIns()->getXY(1);
	int mouse_c = 0;
	if (FadeEfect_GetState() == FadeState_Stop) {
		mouse_c = Mouse::getIns()->getClickLR(0);
	}
	// エリア切り替え矢印ボックス
	{
		if (mouse_x >= 376 && mouse_x <= 390 && mouse_y >= 10 && mouse_y <= 24 && mouse_c == 1) {
			mBackground->addArea(-1);
			mIsPushAreaChangeButton[0] = true;
		}
		if (mouse_x >= 440 && mouse_x <= 454 && mouse_y >= 10 && mouse_y <= 24 && mouse_c == 1) {
			mBackground->addArea(1);
			mIsPushAreaChangeButton[1] = true;
		}
		if (mIsPushAreaChangeButton[0] && mouse_c == 0) {
			mIsPushAreaChangeButton[0] = false;
		}
		if (mIsPushAreaChangeButton[1] && mouse_c == 0) {
			mIsPushAreaChangeButton[1] = false;
		}
	}
	// 難易度調整矢印
	for (int i = 0; i < 5; i++) {
		if (mouse_x >= 376 + (i * 16) && mouse_x <= 391 + (i * 16) && mouse_y >= 30 && mouse_y <= 44 && mouse_c > 0) {
			mDifficulty = i;
		}
	}
	// 敵種族選択ボタン
	{
		if (mEnemyButtonNum != 0 && mouse_x >= 373 && mouse_x <= 396 && mouse_y >= 80 && mouse_y <= 103 && mouse_c == 1) {
			mEnemyButtonNum = 0;
		}
		if (mEnemyButtonNum != 1 && mouse_x >= 404 && mouse_x <= 426 && mouse_y >= 80 && mouse_y <= 103 && mouse_c == 1) {
			mEnemyButtonNum = 1;
		}
		if (mEnemyButtonNum != 2 && mouse_x >= 435 && mouse_x <= 457 && mouse_y >= 80 && mouse_y <= 103 && mouse_c == 1) {
			mEnemyButtonNum = 2;
		}
		if (mEnemyButtonNum == 2 && mouse_x >= 435 && mouse_x <= 457 && mouse_y >= 80 && mouse_y <= 103 && mouse_c >= 100) {
			mEnemyButtonNum = 3;
		}
	}
	// 敵選択
	{
		switch (mEnemyButtonNum) {
		case 0: // 16族
			for (int i = 0; i < 5; i++) {
				for (int j = 0; j < 4; j++) {
					if (mouse_x >= 363 + (i * 16) + (i * 6) &&
						mouse_x <= 378 + (i * 16) + (i * 6) &&
						mouse_y >= 115 + (j * 16) + (j * 6) &&
						mouse_y <= 130 + (j * 16) + (j * 6) &&
						mouse_c == 1) {
						mEnemyNum = i + (j * 5);
						// ゴリ押し補正
						if (mEnemyNum == 14) {
							mEnemyNum = 20;
						} 
						else if(mEnemyNum == 15) {
							mEnemyNum = 21;
						}
						else if (mEnemyNum == 16) {
							mEnemyNum = 22;
						}
						else if (mEnemyNum == 17) {
							mEnemyNum = 23;
						}
					}
				}
			}
			break;

		case 1: // 32族
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 2; j++) {
					if (mouse_x >= 375 + (i * 32) + (i * 12) &&
						mouse_x <= 406 + (i * 32) + (i * 12) &&
						mouse_y >= 120 + (j * 32) + (j * 12) &&
						mouse_y <= 151 + (j * 32) + (j * 12) &&
						mouse_c == 1 &&
						mEnemyNum != ENEMY_START_1 + (i + (j * 2)) && i + (j * 2) < ENEMY_VOLUME_1) {
						mEnemyNum = ENEMY_START_1 + (i + (j * 2));
					}
				}
			}
			break;

		case 2: // ボス
			if (mouse_x >= 370 && mouse_x <= 465 && mouse_y >= 113 && mouse_y <= 208 && mouse_c == 1 &&
				mEnemyNum != ENEMY_START_2) {
				mEnemyNum = ENEMY_END_2;
			}
			break;

		case 3: // アイテム
			if (mouse_x >= 405 && mouse_x <= 420 && mouse_y >= 150 && mouse_y <= 165 && mouse_c == 1 &&
				mEnemyNum != ENEMY_START_3) {
				mEnemyNum = ENEMY_END_3;
			}
			break;

		}
	}
	// エディターボタン
	{
		// 初期化
		for (int i = 0; i < 3; i++) {
			if (mEditorButtonState[i] != 2) {
				mEditorButtonState[i] = 0;
			}
		}
		// 触れたら光る処理
		if (mEditorButtonState[0] == 0 && mouse_x >= 356 && mouse_x <= 393 && mouse_y >= 240 && mouse_y <= 262) {
			mEditorButtonState[0] = 1;
		}
		if (mEditorButtonState[1] == 0 && mouse_x >= 397 && mouse_x <= 434 && mouse_y >= 240 && mouse_y <= 262) {
			mEditorButtonState[1] = 1;
		}
		if (mEditorButtonState[2] == 0 && mouse_x >= 438 && mouse_x <= 475 && mouse_y >= 240 && mouse_y <= 262) {
			mEditorButtonState[2] = 1;
		}
		// アニメ
		for (int i = 0; i < 3; i++) {
			// 光っていなかったらアニメーション初期化
			if (mEditorButtonState[i] == 0 && mEditorButtonAnime[i] != 0) {
				mEditorButtonAnime[i] = 0;
			}
			// 光っている時はアニメーション
			if (mEditorButtonState[i] == 1) {
				mEditorButtonAnime[i] += (float)0.1;
				if (mEditorButtonAnime[i] >= 3) {
					mEditorButtonAnime[i] = 0;
				}
			}
		}
		// 光っている時にクリックしたら
		if (mEditorButtonState[0] == 1 && mEditorButtonState[1] != 2 && mEditorButtonState[2] != 2 && mouse_c == 1) {
			SE::getIns()->playSE(SE_Credit);
			mEditorButtonState[0] = 2;
		}
		if (mEditorButtonState[1] == 1 && mEditorButtonState[0] != 2 && mEditorButtonState[2] != 2 && mouse_c > 0) {
			mSaveMeter += (float)38 / 40;
			if (mSaveMeter > 38) {
				mSaveMeter = 38;
			}
		}
		if (mSaveMeter > 0 && mSaveMeter < 38 && mouse_c == 0) {
			mSaveMeter = 0;
		}
		if (mSaveMeter >= 38) {
			SE::getIns()->playSE(SE_Credit);
			mEditorButtonState[1] = 2;
		}
		if (mEditorButtonState[2] == 1 && mEditorButtonState[0] != 2 && mEditorButtonState[1] != 2 && mouse_c == 1) {
			SE::getIns()->playSE(SE_Credit);
			mEditorButtonState[2] = 2;
		}
	}
}

/*------------------------------------------------------------------------------
描画
------------------------------------------------------------------------------*/
void EditorUI::draw() const
{
	// BOX
	DrawBox(352, 0, 480, 270, GetColor(0, 0, 0), TRUE);
	// エリア数の表示
	Xprintf_Position(2);
	Xprintf_Color(1);
	Xprintf("AREA", 416, 5);
	Xprintf_Color(0);
	XNprintf(mNowArea + 1, 416, 18);
	// エリア切り替え矢印ボックス
	{
		// 左矢印
		if (mIsPushAreaChangeButton[0]) {
			DrawGraph(408 - 32, 10, Image::getIns()->getImage(Image_AreaChangeButton, 1), TRUE);
		}
		else {
			DrawGraph(408 - 32, 10, Image::getIns()->getImage(Image_AreaChangeButton, 0), TRUE);
		}
		// 右矢印
		if (mIsPushAreaChangeButton[1]) {
			DrawGraph(408 + 32, 10, Image::getIns()->getImage(Image_AreaChangeButton, 3), TRUE);
		}
		else {
			DrawGraph(408 + 32, 10, Image::getIns()->getImage(Image_AreaChangeButton, 2), TRUE);
		}
	}
	// 難易度矢印
	for (int i = 0; i < 5; i++) {
		if (i <= mDifficulty) {
			DrawGraph(376 + (i * 16), 30, Image::getIns()->getImage(Image_DifficultyMeter, i), TRUE);
			SetDrawBlendMode(DX_BLENDMODE_ADD, mBlend); // ブレンドモードを加算(255/255)に設定
			DrawGraph(376 + (i * 16), 30, Image::getIns()->getImage(Image_DifficultyMeter, i + 10), TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);  // ブレンドモードをオフ
		}
		else {
			DrawGraph(376 + (i * 16), 30, Image::getIns()->getImage(Image_DifficultyMeter, i + 5), TRUE);
		}
	}
	// xy座標
	Xprintf_Position(2);
	Xprintf_Color(1);
	Xprintf("~~~~~~~~~~~~", 416, 49);
	Xprintf("~~~~~~~~~~~~", 416, 74);
	Xprintf_Position(0);
	Xprintf("X : ", 376, 54);
	Xprintf("Y : ", 376, 63);
	Xprintf_Position(1);
	XNprintf(mMouseX / 4 * 4, 456, 54);
	XNprintf((mMouseY / 4 * 4)+ (int)mCamera->getY(), 456, 63);
	// 種族選択ボタン
	{
		// 16族
		if (mEnemyButtonNum == 0) {
			DrawGraph(373, 81, Image::getIns()->getImage(Image_EnemyButton, 1), TRUE);
		}
		else {
			DrawGraph(373, 81, Image::getIns()->getImage(Image_EnemyButton, 0), TRUE);
		}
		// 32族
		if (mEnemyButtonNum == 1) {
			DrawGraph(404, 81, Image::getIns()->getImage(Image_EnemyButton, 3), TRUE);
		}
		else {
			DrawGraph(404, 81, Image::getIns()->getImage(Image_EnemyButton, 2), TRUE);
		}
		// ボス族
		if (mEnemyButtonNum == 2) {
			DrawGraph(434, 81, Image::getIns()->getImage(Image_EnemyButton, 5), TRUE);
		}
		else if (mEnemyButtonNum == 3) {
			DrawGraph(434, 81, Image::getIns()->getImage(Image_EnemyButton, 6), TRUE);
		}
		else {
			DrawGraph(434, 81, Image::getIns()->getImage(Image_EnemyButton, 4), TRUE);
		}
	}
	// 敵選択BOX
	{
		int num = 0;
		int widSel = 0;
		int heiSel = 0;
		int mouse_x = Mouse::getIns()->getXY(0);
		int mouse_y = Mouse::getIns()->getXY(1);
		int tmpEneNum;
		DrawGraph(355, 110, Image::getIns()->getImage(Image_EnemyBox), TRUE);
		switch (mEnemyButtonNum) {
		case 0: // 16族
			widSel = 0, heiSel = 0;
			// アイコン
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 5; j++) {
					if (num < 18) {
						DrawGraph(363 + (j * 22), 115 + (i * 22), Image::getIns()->getImage(Image_EnemyIcon, num), TRUE);
					}
					if (num < 20) {
						num++;
					}
				}
			}
			// 弾族ごり押し
			tmpEneNum = mEnemyNum;
			if (mEnemyNum >= 20) {
				tmpEneNum -= 6;
			}
			widSel = tmpEneNum % 5;
			heiSel = tmpEneNum / 5;
			// 選択カーソル
			if (mEnemyNum >= 0 && mEnemyNum <= ENEMY_END_0 || mEnemyNum >= 20) {
				DrawGraph(363 + (widSel * 22), 115 + (heiSel * 22), Image::getIns()->getImage(Image_Select), TRUE);
			}
			for (int i = 0; i < 5; i++) {
				for (int j = 0; j < 4; j++) {
					if (mouse_x >= 363 + (i * 16) + (i * 6) &&
						mouse_x <= 378 + (i * 16) + (i * 6) &&
						mouse_y >= 115 + (j * 16) + (j * 6) &&
						mouse_y <= 130 + (j * 16) + (j * 6) &&
						mEnemyNum != i + (j * 5) && i + (j * 5) < 18) {
						DrawGraph(363 + (i * 16) + (i * 6), 115 + (j * 16) + (j * 6), Image::getIns()->getImage(Image_Aiming, 2), TRUE);
					}
				}
			}
			break;

		case 1: // 32族
			widSel = 0, heiSel = 0;
			// アイコン
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 2; j++) {
					DrawGraph(375 + (j * 44), 120 + (i * 44), Image::getIns()->getImage(Image_EnemyBigIcon, num), TRUE);
					if (num < ENEMY_VOLUME_1) {
						num++;
					}
				}
			}
			widSel = (mEnemyNum - 14) % 2;
			heiSel = (mEnemyNum - 14) / 2;
			// 選択カーソル
			if (mEnemyNum >= ENEMY_START_1 && mEnemyNum <= ENEMY_END_1) {
				DrawRotaGraph(391 + (widSel * 44), 136 + (heiSel * 44), 2, 0, Image::getIns()->getImage(Image_Select), TRUE);
			}
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 2; j++) {
					if (mouse_x >= 375 + (i * 32) + (i * 12) &&
						mouse_x <= 406 + (i * 32) + (i * 12) &&
						mouse_y >= 120 + (j * 32) + (j * 12) &&
						mouse_y <= 151 + (j * 32) + (j * 12) &&
						mEnemyNum != ENEMY_START_1 + (i + (j * 2)) && i + (j * 2) < ENEMY_VOLUME_1) {
						DrawRotaGraph(391 + (i * 32) + (i * 12), 136 + (j * 32) + (j * 12), 2, 0, Image::getIns()->getImage(Image_Aiming, 2), TRUE);
					}
				}
			}
			break;

		case 2: // ボス
			// アイコン
			DrawGraph(370, 113, Image::getIns()->getImage(Image_BossIcon), TRUE);
			// 選択カーソル
			if (mEnemyNum == ENEMY_START_2) {
				DrawRotaGraph(418, 161, 6, 0, Image::getIns()->getImage(Image_Select), TRUE);
			}
			if (mouse_x >= 370 && mouse_x <= 465 && mouse_y >= 113 && mouse_y <= 208 &&
				mEnemyNum != ENEMY_START_2) {
				DrawRotaGraph(418, 161, 6, 0, Image::getIns()->getImage(Image_Aiming, 2), TRUE);
			}
			break;

		case 3: // アイテム
			// アイコン
			DrawGraph(405, 150, Image::getIns()->getImage(Image_SpecialFlag), TRUE);
			// 選択カーソル
			if (mEnemyNum == ENEMY_START_3) {
				DrawGraph(405, 150, Image::getIns()->getImage(Image_Select), TRUE);
			}
			if (mouse_x >= 405 && mouse_x <= 420 && mouse_y >= 150 && mouse_y <= 165 &&
				mEnemyNum != ENEMY_START_3) {
				DrawGraph(405, 150, Image::getIns()->getImage(Image_Aiming, 2), TRUE);
			}
			break;
		}
	}
	// 敵の名前
	drawEnemyName();
	// 配置できる残り数
	Xprintf_Position(1);
	Xprintf_Color(1);
	Xprintf("/ 256", 452, 228);
	Xprintf_Position(2);
	XNprintf(mEnemyCount, 392, 228);
	// エディターボタン
	switch (mEditorButtonState[0]) {
	case 0:
		DrawGraph(356, 240, Image::getIns()->getImage(Image_EditorButton, 0), TRUE);
		break;
	case 1:
		DrawGraph(356, 240, Image::getIns()->getImage(Image_EditorButton, 1 + (int)mEditorButtonAnime[0]), TRUE);
		break;
	case 2:
		DrawGraph(356, 240, Image::getIns()->getImage(Image_EditorButton, 4), TRUE);
		break;
	}
	switch (mEditorButtonState[1]) {
	case 0:
		DrawGraph(397, 240, Image::getIns()->getImage(Image_EditorButton, 5), TRUE);
		break;
	case 1:
		DrawGraph(397, 240, Image::getIns()->getImage(Image_EditorButton, 6 + (int)mEditorButtonAnime[1]), TRUE);
		break;
	case 2:
		DrawGraph(397, 240, Image::getIns()->getImage(Image_EditorButton, 9), TRUE);
		break;
	}
	switch (mEditorButtonState[2]) {
	case 0:
		DrawGraph(438, 240, Image::getIns()->getImage(Image_EditorButton, 10), TRUE);
		break;
	case 1:
		DrawGraph(438, 240, Image::getIns()->getImage(Image_EditorButton, 11 + (int)mEditorButtonAnime[2]), TRUE);
		break;
	case 2:
		DrawGraph(438, 240, Image::getIns()->getImage(Image_EditorButton, 14), TRUE);
		break;
	}
	if (mSaveMeter < 38) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
		DrawBox(397, 240, 397 + (int)mSaveMeter, 240 + 23, GetColor(0, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

/*------------------------------------------------------------------------------
敵の名前表示
------------------------------------------------------------------------------*/
void EditorUI::drawEnemyName() const
{
	Xprintf_Position(2);
	Xprintf_Color(0);
	switch (mEnemyNum) {
	case 0:
		Xprintf("TERRAZI", 416, 215);
		break;
	case 1:
		Xprintf("ZOLBAK", 416, 215);
		break;
	case 2:
		Xprintf("TOROID", 416, 215);
		break;
	case 3:
		Xprintf("TORKEN", 416, 215);
		break;
	case 4:
		Xprintf("BARRA", 416, 215);
		break;
	case 5:
		Xprintf("KAPI", 416, 215);
		break;
	case 6:
		Xprintf("DEROTA", 416, 215);
		break;
	case 7:
		Xprintf("ZOSHI", 416, 215);
		break;
	case 8:
		Xprintf("LOGRAM", 416, 215);
		break;
	case 9:
		Xprintf("SHEONITE", 416, 215);
		break;
	case 10:
		Xprintf("BOZALOGRAM", 416, 215);
		break;
	case 11:
		Xprintf("DOMOGRAM", 416, 215);
		break;
	case 12:
		Xprintf("GROBDA", 416, 215);
		break;
	case 13:
		Xprintf("JARA", 416, 215);
		break;
	case 14:
		Xprintf("GARU DEROTA", 416, 215);
		break;
	case 15:
		Xprintf("GARU BARRA", 416, 215);
		break;
	case 16:
		Xprintf("SOL", 416, 215);
		break;
	case 17:
		Xprintf("BACURA", 416, 215);
		break;
	case 18:
		Xprintf("ADDOR GUILENESS", 416, 215);
		break;
	case 19:
		Xprintf("SPECIAL FLAG", 416, 215);
		break;
	case 20:
		Xprintf("ZAKATO", 416, 215);
		break;
	case 21:
		Xprintf("GIDO SPARIO", 416, 215);
		break;
	case 22:
		Xprintf("BRAG ZAKATO", 416, 215);
		break;
	case 23:
		Xprintf("GARU ZAKATO", 416, 215);
		break;
	}
}

/*------------------------------------------------------------------------------
現在選択している敵番号を返す
------------------------------------------------------------------------------*/
int EditorUI::getEnemyNum()
{
	return mEnemyNum;
}

/*------------------------------------------------------------------------------
現在の敵の設置数をセット
------------------------------------------------------------------------------*/
void EditorUI::setEnemyCount(int aCount)
{
	mEnemyCount = aCount;
}

/*------------------------------------------------------------------------------
エディターボタンの状態を返す
------------------------------------------------------------------------------*/
int EditorUI::getEditorButtonState(int a)
{
	return mEditorButtonState[a];
}

/*------------------------------------------------------------------------------
エディターボタンの状態を返す
------------------------------------------------------------------------------*/
int EditorUI::getDifficulty()
{
	return mDifficulty;
}

/*------------------------------------------------------------------------------
エディターボタンの状態を返す
------------------------------------------------------------------------------*/
void EditorUI::resetEditorButton()
{
	mEditorButtonState[0] = 0;
	mEditorButtonState[1] = 0;
	mSaveMeter = 0;
}
