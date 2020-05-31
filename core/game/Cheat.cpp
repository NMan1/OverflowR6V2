#include "cheat.h"
#include "../helpers/settings.h"
#include "menu.h"
#include <thread>
#include "game.h"
#pragma comment(lib, "winmm.lib")

using namespace g_Settings;
using namespace g_Cheat;


void any_round();
void action_round();
void prep_round();


void cheat::cheat_thread()
{
	while (true)
	{
		if (g_Settings::start_cheat)
		{
			cur_round = game::get_round();
			if (old_round != cur_round)
			{
				switch (cur_round)
				{
				case 0: Status = "   Round Swap"; break;
				case 1: Status = "   OP Selection"; break;
				case 2: Status = "   Prep Phase"; break;
				case 3: Status = "   Action Phase"; break;
				case 4: Status = "   End Of Round"; break;
				case 5: Status = "   Main Menu"; break;
				}
				old_round = cur_round;
			}

			if (cur_round == 5 || cur_round == 3 || cur_round == 2 || cur_round == 1)
			{
				AnyRound();

			if (cur_round == 2 || cur_round == 3)
				Prep_Action();

			if (cur_round == 3)
			{
				Action();
			}

			// End Whole Cheat
			if (end)
			{
				cheat_thread = true;
				break;
			}

			Sleep(5);
		}
	}

	return;
}

void any_round()
{
	//if (unlock_all && bCanDoUnlockAll)
	//{
	//	Driver.SendMsg(("UNALL"));
	//	bCanDoUnlockAll = false;
	//}

	//if (web_fov && bCanDoWepFov)
	//{
	//	Driver.SendMsg(("FOVWEP"));
	//	bCanDoWepFov = false;
	//}

	//if (player_fov && bCanDoPlayerFov)
	//{
	//	Driver.SendMsg("FOVCAR");
	//	bCanDoPlayerFov = false;
	//}
}

void action_round()
{
	//Cheat::SetSettings(false, true, false, true);
	//if (aimbot)
	//{
	//	while (GetAsyncKeyState(aim_key) || GetAsyncKeyState(aim_key_2))
	//	{
	//		if (silent)
	//			Driver.StartAimbot(true);
	//		else
	//			Driver.StartAimbot(false);
	//	}
	//}

	//if (run_shoot && bCanDoRunShoot)
	//{
	//	Driver.SendMsg(("RUNSH"));
	//	bCanDoRunShoot = false;
	//}

	//if (color && bCanDoColor)
	//{
	//	Driver.SendMsg(("COLOR"));
	//	bCanDoColor = false;
	//}

	//if (damage && bCanDoDmg)
	//{
	//	Driver.SendMsg(("DAMAGE"));
	//	bCanDoDmg = false;
	//}

	//if (cav_esp && bCanDoCav)
	//{
	//	Driver.SendCavRequest();
	//	bCanDoCav = false;
	//}
}

void prep_round()
{
	if (chams)
		game::set_glow();

	if (no_clip || (no_clip && GetAsyncKeyState(no_clip_key) & 0x8000))
		game::no_recoil();

	if (no_flash)
	{
		/*Driver.SendMsg(("FLASH"));*/
		bCanDoNoFlash = false;
	}

	if (speed || (speed && GetAsyncKeyState(VK_SHIFT)))
		game::speed_changer();
	
	if (rapid_fire)
		game::rapid_fire();
		
	if (no_animations)
		game::no_animations();

	if (no_spread)
		game::no_spread();

	if (no_recoil)
		game::no_recoil();

	//if (damage && bCanDoDmg)
	//{
	//	Driver.SendMsg(("DAMAGE"));
	//	Driver.SendMsg(("DAMAGE"));
	//	bCanDoDmg = false;
	//}

	//if (GetAsyncKeyState(VK_UP))
	//{
	//	Driver.SendMsg(("DAMAGE"));
	//	Driver.SendMsg(("RECOIL"));
	//	Driver.SendMsg(("SPREAD"));
	//}

	//if (cav_esp && bCanDoCav)
	//{
	//	Sleep(3500);
	//	Driver.SendCavRequest();
	//	bCanDoCav = false;
	//}
}
