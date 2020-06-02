#include <Windows.h>
#include <iostream>
#include "driver/driver.h"
#include "menu/menu.h"
#include "helpers/settings.h"
#include "game/game.h"
#include "game/cheat.h"
#include "menu/config.h"

DWORD64 base_address = NULL;

int main()
{
	//ShowWindow(GetConsoleWindow(), SW_HIDE);

	driver::get_process_id("RainbowSix");
	base_address = driver::get_module_base_address("RainbowSix.exe");
	if (!base_address)
	{
		std::cout << "Failed to aquire base address..." << std::endl;
		std::cin.get();
		return 0;
	}

	std::cout << "Aquired base addres: " << std::hex << base_address << std::endl;

	std::thread do_cheat(cheat::cheat_thread);

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
		menu::begin_draw();
		menu::render_menu();
		menu::end_draw();

		if (settings::end)
			break;
	}

	DefualtCfg->close();
	LegitCfg->close();
	RageCfg->close();

	menu::menu_shutdown();  // End Menu Allocations and Handles

	return 1;
}