#pragma once

enum eFadeState {
	FadeState_Stop,
	FadeState_Play,
	FadeState_End,
	FadeState_ReversePlay,
};
enum eFadeMode {
	FadeMode_Up,
	FadeMode_Down,
	FadeMode_Left,
	FadeMode_Right,
};
void FadeEfect_Init();
void FadeEfect_Mode(eFadeMode aFadeMode);
eFadeState FadeEfect_GetState();
void FadeEfect_Play();
void FadeEfect_Update();
void FadeEfect_Draw();