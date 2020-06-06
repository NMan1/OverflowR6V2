#pragma once
#include "../driver/driver.h"
#include "../helpers/settings.h"
#include "../renderer/renderer.h"
#include "game.h"
#include "aimbot.h"
#include <dwmapi.h>

namespace thread = std::this_thread;
using ms = std::chrono::milliseconds;

#define M_PI 3.14159265358979323846
#define M_RADPI		57.295779513082f
#define M_PI_F		((float)(M_PI))	// Shouldn't collide with anything.
#define RAD2DEG( x  )  ( (float)(x) * (float)(180.f / M_PI_F) )
#define DEG2RAD( x  )  ( (float)(x) * (float)(M_PI_F / 180.f) )

namespace game
{
	HWND find_window()
	{
		auto exterior_window_handle = FindWindowW(L"CEF-OSC-WIDGET", L"NVIDIA GeForce Overlay DT");
		if (!exterior_window_handle)
		{
			printf("[!] can't get exterior window's handle\n");
			std::cin.get();
			return 0;
		}
		std::cout << "[+] found hijackwindow: " << std::hex << exterior_window_handle << std::endl;
		return exterior_window_handle;
	}
	
	void run_esp()
	{
		d2d_window_t window{ };
		_renderer renderer{ window._handle, find_window() };
		window.~d2d_window_t();

		auto target_game = driver::read<HWND>(base_address + 0x51c3070);	
		std::cout << "	[+] target window found: " << std::hex << target_game << std::endl; 

		while (!GetAsyncKeyState(VK_END))
		{
			renderer.begin_scene();
			if (target_game == GetForegroundWindow())
			{
				if (!settings::cheat_thread && cheat::can_draw)
				{
					static uintptr_t game_manager = driver::read<uintptr_t>(base_address + game_off);
					if (!game_manager)
						continue;

					uintptr_t entity_list = driver::read<uintptr_t>(game_manager + entity_list_off);
					int entity_count = driver::read<DWORD>(game_manager + entity_count_off) & 0x3fffffff;
					if (entity_count == NULL)
						continue;

					for (int i = 0; i < entity_count; i++)
					{
						uintptr_t entity = driver::read<uintptr_t>(entity_list + i * 0x8);
						if (!entity)
							continue;

						uintptr_t local = get_local();
						if (!local)
							continue;

						if (!is_enemy(entity))
							continue;

						auto health = get_health(entity);

						vec_t foot2d, head_top2d, head2d;
						if (w2s(get_bone(entity, BONE_FEET), foot2d) && w2s(get_bone(entity, BONE_HEAD, true), head_top2d) && w2s(get_bone(entity, BONE_HEAD), head2d))
						{
							int height = foot2d.y - head_top2d.y;
							int width = height / 2.4;

							if (health > 0)
								renderer.draw_health_bar(head_top2d.x - width / 2 - 6, head_top2d.y, 2, height, health, 100, clr(255, 50, 50));

							renderer.draw_rect(head_top2d.x - width / 2, head_top2d.y, width, height, clr(82, 0, 148));
							renderer.draw_line(wnd_hjk::screen_resolution.first / 2, wnd_hjk::screen_resolution.second, foot2d.x, foot2d.y, clr(0, 255, 255, 255));
							renderer.draw_circle(head2d.x, head2d.y+2, height / 12.5, clr(223, 62, 239));
						}
					}
				}
				renderer.draw_filled_rect(50, 50, 10, 10, clr(223, 62, 239));
			}
			renderer.end_scene();
			thread::sleep_for(ms(1));
		}
	}
} 