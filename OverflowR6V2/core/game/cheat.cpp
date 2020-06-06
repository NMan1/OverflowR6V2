#include <thread>
#include "cheat.h"
#include "../helpers/settings.h"
#include "../menu/menu.h"
#include "game.h"
#include "aimbot.h"
//#pragma comment(lib, "winmm.lib")

void features();
bool cheat::can_draw = false;

void cheat::cheat_thread()
{
	while (true)
	{
		static auto old_round = -1;
		auto cur_round = game::get_round();
		old_round != cur_round ? old_round = cur_round : true;

		if (settings::unlock_all)
			game::unlock_all();

		if (cur_round == 3)
		{
			features();
			can_draw = true;
		}
		else
			can_draw = false;

		if (settings::end)
		{
			settings::cheat_thread = true;
			break;
		}

		Sleep(1);
	}

	return;
}

void features()
{
	if (settings::aimbot && GetAsyncKeyState(settings::aim_key))
		game::run_aimbot();
	
	if (settings::chams)
		game::set_glow();

	if (settings::no_clip || (settings::no_clip && GetAsyncKeyState(settings::no_clip_key) & 0x8000))
		game::no_clip();

	if (settings::speed)
		game::speed_changer();
	
	if (settings::rapid_fire)
		game::rapid_fire();
		
	if (settings::no_animations)
		game::no_animations();

	if (settings::no_spread)
		game::no_spread();

	if (settings::no_recoil)
		game::no_recoil();	

	if (settings::wep_fov)
		game::weapon_fov();

	if (settings::player_fov)
		game::player_fov();

	if (settings::cav_esp)
		game::cav_esp();
}
