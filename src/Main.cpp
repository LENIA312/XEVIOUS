#pragma warning(disable:28251)

//-------------------------------------------------------------------------------------------------
#include <DxLib.h>
#include "SystemMain.h"

//-------------------------------------------------------------------------------------------------
// エントリー関数
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	SystemMain system;
	if (system.initialize()) {
		system.loop();
	}

	return 0;
}