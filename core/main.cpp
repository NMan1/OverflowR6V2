#include <Windows.h>
#include <iostream>
#include "driver/driver.h"
#include "menu/menu.h"
#include "helpers/settings.h"
#include "game/game.h"
#include "game/cheat.h"

DWORD64 base_address = NULL;

int main()
{
	ShowWindow(GetConsoleWindow(), SW_HIDE);

	get_process_id("RainbowSix");
	base_address = get_module_base_address("RainbowSix.exe");
	
	std::thread keys_loop(Keys);
	std::thread do_cheat(cheat::main_cheat);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	menu::setup_menu();
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
		menu::be();
		menu::RenderMenu();
		menu::EndDraw();

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

	menu::MenuShutDown();  // End Menu Allocations and Handles

	std::cin.get();
}