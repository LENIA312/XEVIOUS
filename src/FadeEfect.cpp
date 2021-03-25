#pragma warning(disable:26812)

#include <DxLib.h>
#include "FadeEffect.h"
#include "Define.h"
#include "Image.h"

/*------------------------------------------------------------------------------
定義
------------------------------------------------------------------------------*/
const static int FADE_SPEED    =  2;
const static int FADE_WIDTH    = Define::GameScreenWidth / 16;
const static int FADE_HEIGHT   = Define::GameScreenHeight / 16 + 1;
const static int ANIME_MAX     = 35;

/*------------------------------------------------------------------------------
変数
------------------------------------------------------------------------------*/
static eFadeState fadeState;
static eFadeMode  fadeMode;
static int fadeAnime;
static int timer;

/*------------------------------------------------------------------------------
定義
------------------------------------------------------------------------------*/
void FadeEfect_Init()
{
	// 変数セット
	fadeState = FadeState_Stop;
	FadeEfect_Mode(FadeMode_Up);
	fadeAnime = 0;
	timer = 0;
}

/*------------------------------------------------------------------------------
フェードモードをセット
------------------------------------------------------------------------------*/
void FadeEfect_Mode(eFadeMode aFadeMode)
{
	fadeMode = aFadeMode;
}

/*------------------------------------------------------------------------------
現在のフェード状況を返す
------------------------------------------------------------------------------*/
eFadeState FadeEfect_GetState()
{
	return fadeState;
}

/*------------------------------------------------------------------------------
再生
------------------------------------------------------------------------------*/
void FadeEfect_Play()
{
	switch (fadeState) {
	case FadeState_Stop: // 停止中なら
		fadeState = FadeState_Play;
		break;

	case FadeState_End:  // 真っ暗なら
		fadeState = FadeState_ReversePlay;
		break;

	default: // エラー処理
		DrawFormatString(0, 0, GetColor(255, 255, 255), "フェードを再生し続けているよ");
		break;
	}
}

/*------------------------------------------------------------------------------
更新
------------------------------------------------------------------------------*/
void FadeEfect_Update()
{
	int tmpMAX = ANIME_MAX;
	if (fadeMode == FadeMode_Left || fadeMode == FadeMode_Right) {
		tmpMAX += 10;
	}
	switch (fadeState) {
	case FadeState_Play: // 通常再生
		if (timer % FADE_SPEED == 0 && fadeAnime < tmpMAX) {
			fadeAnime++;
		}
		if (fadeAnime < tmpMAX) {
			timer++;
		}
		if (fadeAnime >= tmpMAX) {
			fadeState = FadeState_End;
		}
		break;

	case FadeState_ReversePlay: // 逆再生
		if (timer % FADE_SPEED == 0 && fadeAnime > 0) {
			fadeAnime--;
		}
		if (fadeAnime > 0) {
			timer++;
		}
		if (fadeAnime <= 0) {
			fadeState = FadeState_Stop;
		}
		break;

	case FadeState_End: // 通常再生終わり後
		fadeAnime = 36;
		break;
	}
}

/*------------------------------------------------------------------------------
描画
------------------------------------------------------------------------------*/
void FadeEfect_Draw()
{
	if (fadeState != FadeState_Stop) 
	{
		// 描画するスタート地点の座標をセット
		int start_x = 0;
		int start_y = 0;
		switch (fadeMode) {
		case FadeMode_Up:
			start_x = 0;
			start_y = FADE_HEIGHT;
			break;
		case FadeMode_Down:
			
			start_x = 0;
			start_y = 0;
			break;
		case FadeMode_Left:
			start_x = FADE_WIDTH;
			start_y = 0;
			break;
		case FadeMode_Right:
			start_x = 0;
			start_y = 0;
			break;
		}
		int x = start_x;
		int y = start_y;

		// ここから描画開始
		int order = 0;
		int tmpAnime = 0;
		for (int i = 0; i < FADE_WIDTH * FADE_HEIGHT + (FADE_WIDTH + FADE_HEIGHT) + 1; i++) {
			tmpAnime = fadeAnime - order;
			if (tmpAnime > 14) {
				tmpAnime = 14;
			}
			if (tmpAnime >= 0) {
				DrawGraph(x * 16 - 8, y * 16 - 8, Image::getIns()->getImage(Image_Fade, tmpAnime), TRUE);
			}

			switch (fadeMode) {
			case FadeMode_Up:
				x++;
				if (x > FADE_WIDTH) {
					x = start_x;
					y--;
					order++;
				}
				break;

			case FadeMode_Down:
				x++;
				if (x > FADE_WIDTH) {
					x = start_x;
					y++;
					order++;
				}
				break;

			case FadeMode_Left:
				y++;
				if (y > FADE_HEIGHT) {
					y = start_y;
					x--;
					order++;
				}
				break;

			case FadeMode_Right:
				y++;
				if (y > FADE_HEIGHT) {
					y = start_y;
					x++;
					order++;
				}
				break;

			}
		}
		// デバッグ
		//DrawFormatString(0, 0, GetColor(255, 255, 255), "Mode:%d State:%d Anime:%d\ntmpAnime:%d order:%d",
		//	fadeMode, fadeState, fadeAnime, tmpAnime, order);
	}
}