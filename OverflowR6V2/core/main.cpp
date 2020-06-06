#include <Windows.h>
#include <iostream>
#include "driver/driver.h"
#include "menu/menu.h"
#include "helpers/settings.h"
#include "game/game.h"
#include "game/cheat.h"
#include "game/esp.h"
#include "renderer/renderer.h"

DWORD64 base_address = NULL;

int main()
{
#ifdef NDEBUG       
	ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif

	driver::get_process_id("RainbowSix");
	base_address = driver::get_module_base_address("RainbowSix.exe");
	if (!base_address)
	{
		std::cout << "[!] failed to aquire base address..." << std::endl;
		std::cin.get();
		return 0;
	}

	std::cout << "[+] aquired base address and PID: " << std::hex << base_address << std::endl;

	std::thread run_cheat(cheat::cheat_thread);
	std::thread esp_render(game::run_esp);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	menu::setup_menu();
	while (true)
	{
		if (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			continue;
		}

		menu::begin_draw();
		menu::render_menu();
		menu::end_draw();

		if (settings::end)
			break;
	}

	menu::menu_shutdown();
	system("taskkill /F /T /IM \"Nvidia Share.exe\"");

	return 1;
}