#pragma warning(disable:28251)

//-------------------------------------------------------------------------------------------------
#include <DxLib.h>
#include "SystemMain.h"

//-------------------------------------------------------------------------------------------------
// ÉGÉìÉgÉäÅ[ä÷êî
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	SystemMain system;
	if (system.initialize()) {
		system.loop();
	}

	return 0;
}