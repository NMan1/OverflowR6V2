#include <Windows.h>
#include <iostream>
#include "driver.h"
#include "menu.h"
#include "Settings.h"
#include "Config.h"
#include "Cheat.h"

DWORD64 base_address = NULL;

int main()
{
	get_process_id("RainbowSix");
	base_address = get_module_base_address("RainbowSix.exe");
	
	std::thread DoCheat(Cheat::MainCheat);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	Menu::SetupMenu();
	while (true)
	{
		// Setup Input
		if (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			continue;
		}

		// Render main cheat menu if in the game
		Menu::BeginDraw();
		Menu::RenderMenu();
		Menu::EndDraw();

		// End UM & Driver
		if (g_Settings::bCheatThread)
		{
			//Driver.End();
			break;
		}

		if (g_Settings::bEndUM)
			break;
	}

	DefualtCfg->close();
	LegitCfg->close();
	RageCfg->close();

	Menu::MenuShutDown();  // End Menu Allocations and Handles

	std::cin.get();
}