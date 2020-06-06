#pragma once
#include <string>
#include <d3d9.h>
#include "../menu/helpers/color.hpp"

namespace settings
{
	// Login
	extern bool start_cheat;

	// Menu
	extern bool menu_opened;
	extern bool update_settings;

	// Esp
	extern bool should_draw;

	// Visual
	extern bool esp;
	extern bool team;
	extern bool outline;
	extern bool chams;
	extern bool cav_esp;

	// Weapon
	extern int  aim_selection;
	extern int  aim_bone;
	extern int  aim_key_2;
	extern int  aim_key;
	extern int  silent_key;
	extern bool aimbot;
	extern bool run_shoot;
	extern bool damage;
	extern bool no_recoil;
	extern bool no_spread;

	// Misc
	extern bool unlock_all;
	extern bool no_clip;
	extern int no_clip_key;
	extern bool wep_fov;
	extern bool player_fov;
	extern bool speed;
	extern bool no_flash;
	extern bool no_animations;
	extern bool rapid_fire;

	// Settings
	extern int  rage_key;
	extern int  legit_key;
	extern bool end;
	extern bool cheat_thread;

	/* VALUES */

	// Misc
	extern float spread_value;
	extern float wep_fov_value;
	extern float player_fov_value;
	extern int   speed_value;
	extern float   speed_value_f;
	extern int  bullet_value;
	extern float bullet_value_f;

	// Chams
	extern bool rainbow_chams;
	extern clr chams_clr;
	extern float chams_oppacity;
	extern float chams_oppacity_f;

	// Aimbot
	extern bool smoothing;
	extern bool	 silent;
	extern int	 aim_fov;
	extern float smoothing_value;
}