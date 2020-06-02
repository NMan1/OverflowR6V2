#pragma once
#include "../driver/driver.h"
#include "../helpers/settings.h"

extern DWORD64 base_address;

namespace game
{
	static uintptr_t get_local()
	{
		uintptr_t local_player = driver::read<uintptr_t>(base_address + profile_off);
		local_player = driver::read<uintptr_t>(local_player + 0x68);
		local_player = driver::read<uintptr_t>(local_player);
		return driver::read<uintptr_t>(local_player + 0x28);
	}	

	static bool is_enemy(uintptr_t enemy)
	{
		auto team_enemy = driver::read<uintptr_t>(enemy + 0xc8);
		team_enemy = driver::read<byte>(team_enemy + 0x1A2);

		auto team_local = driver::read<uintptr_t>(get_local() + 0xc8);
		team_local = driver::read<byte>(team_local + 0x1A2);

		return team_enemy != team_local;
	}

	static uintptr_t get_round()
	{
		uintptr_t round = driver::read<uintptr_t>(base_address + round_off);
		return driver::read<uintptr_t>(round + 0x230);
	}

	static auto rainbow() //proper rainbow
	{
		static uint32_t cnt = 0;
		float freq = 0.06f;

		if (cnt++ >= (uint32_t)-1)
			cnt = 0;

		return vec3_t(std::sin(freq * cnt + 0) * 0.5f + 0.5f,
			std::sin(freq * cnt + 2) * 0.5f + 0.5f,
			std::sin(freq * cnt + 4) * 0.5f + 0.5f);
	}

	static bool rapid_fire()
	{
		auto rapid_fire = driver::read<uintptr_t>(get_local() + 0x90);
		rapid_fire = driver::read<uintptr_t>(rapid_fire + 0xC8);

		if (!rapid_fire)
			return false;

		driver::write<int>(rapid_fire + 0x108, !settings::rapid_fire);
		return true;
	}

	static bool no_animations()
	{
		uint64_t no_anim = driver::read<uint64_t>(base_address + game_off);
		no_anim = driver::read<uint64_t>(no_anim + 0x38);
		no_anim = driver::read<uint64_t>(no_anim + 0xC8);
		no_anim = driver::read<uint64_t>(no_anim + 0x0);
		no_anim = driver::read<uint64_t>(no_anim + 0x90);
		no_anim = driver::read<uint64_t>(no_anim + 0x118);
		no_anim = driver::read<uint64_t>(no_anim + 0xC8);

		if (!no_anim)
			return false;

		driver::write<BYTE>(no_anim + 0x36B, !settings::no_animations);
		return true;
	}

	static bool speed_changer()
	{
		uintptr_t speed = driver::read<uintptr_t>(base_address + game_off);
		speed = driver::read<uintptr_t>(speed + 0xC8);
		speed = driver::read<uintptr_t>(speed);
		speed = driver::read<uintptr_t>(speed + 0x30);
		speed = driver::read<uintptr_t>(speed + 0x30);
		speed = driver::read<uintptr_t>(speed + 0x38);

		if (!speed)
			return false;

		if (settings::speed)
			driver::write<int>(speed + 0x58, settings::speed_value);
		else
			driver::write<int>(speed + 0x58, 110);
		return true;
	}

	static bool no_clip(bool on = true)
	{
		uint64_t no_clip = driver::read<uint64_t>(base_address + net_off);
		no_clip = driver::read<uint64_t>(no_clip + 0xF8);
		no_clip = driver::read<uint64_t>(no_clip + 0x8);

		if (!no_clip)
			return false;

		if (settings::no_clip)
		{
			driver::write<float>(no_clip + 0x7F0, -1.0f);
			driver::write<float>(no_clip + 0x7F4, -1.0f);
			driver::write<float>(no_clip + 0x7F8, -1.0f);
			driver::write<float>(no_clip + 0x7FC, -1.0f);
		}
		else
		{
			driver::write<float>(no_clip + 0x7F0, 1.f);
			driver::write<float>(no_clip + 0x7F4, 1.f);
			driver::write<float>(no_clip + 0x7F8, 1.f);
			driver::write<float>(no_clip + 0x7FC, 1.f);
		}
		return true;
	}

	static bool no_spread()
	{
		auto no_spread = driver::read<uintptr_t>(get_local() + 0x90);
		no_spread = driver::read<uintptr_t>(no_spread + 0xC8);
		no_spread = driver::read<uintptr_t>(no_spread + 0x278);

		if (!no_spread)
			return false;

		driver::write<float>(no_spread + 0x38, !settings::no_spread);
		return true;
	}

	static bool no_recoil() 
	{
		uintptr_t no_recoil = driver::read<uintptr_t>(base_address + fov_off);
		no_recoil = driver::read<uintptr_t>(no_recoil + 0x110);
		no_recoil = driver::read<uintptr_t>(no_recoil + 0x0);

		if (!no_recoil)
			return false;

		driver::write<bool>(no_recoil + 0xe2d, !settings::no_recoil);
		return true;
	}
	
	static bool weapon_fov()
	{
		uintptr_t weapon_fov = driver::read<uintptr_t>(base_address + fov_off);
		weapon_fov = driver::read<uintptr_t>(weapon_fov + 0x28);
		weapon_fov = driver::read<uintptr_t>(weapon_fov);

		if (!weapon_fov)
			return false;

		driver::write<float>(weapon_fov + 0x3c, settings::wep_fov_value);
		return true;
	}

	static bool player_fov()
	{
		uintptr_t player_fov = driver::read<uintptr_t>(base_address + fov_off);
		player_fov = driver::read<uintptr_t>(player_fov + 0x28);
		player_fov = driver::read<uintptr_t>(player_fov);

		if (!player_fov)
			return false;

		driver::write<float>(player_fov + 0x38, settings::player_fov_value);
		return true;
	}


	static bool unlock_all() 
	{
		static auto do_once = true;
		if (!do_once)
			return false;

		if (!base_address)
			return false;

		unsigned char shell [89] = {
			0x53, 0x48, 0x83, 0xEC, 0x20, 0x48, 0xB8, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
			0xD0, 0x48, 0x8D, 0x54, 0x24, 0x28, 0x48, 0x89, //
			0xC3, 0x48, 0x8B, 0x42, 0x30, 0x48, 0x89, 0xC1,
			0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x48, 0x29, 0xC1, 0x48, 0x81, 0xF9,
			0xD1, 0xD5, 0xE0, 0x00, 0x75, 0x1A, 0x48, 0x8B,
			0x42, 0x30, 0x48, 0x8D, 0x48, 0x68, 0x48, 0x89,
			0x4A, 0x30, 0x48, 0x89, 0xF8, 0xC6, 0x40, 0x52,
			0x00, 0x48, 0x89, 0xF8, 0xC6, 0x40, 0x51, 0x00,
			0x48, 0x89, 0xD8, 0x48, 0x83, 0xC4, 0x20, 0x5B,
			0xC3
		};

		uintptr_t allocted_memory = driver::virtual_alloc(0, PAGE_EXECUTE_READWRITE, 0x1000);

		*(UINT_PTR*)(&shell[0x7]) = driver::read <uintptr_t>(base_address + 0x5AFF3F0);
		*(UINT_PTR*)(&shell[0x22]) = base_address;
		std::cout << "1" << std::endl;
		Sleep(3000);

		driver::write(allocted_memory, shell);
		std::cout << "2" << std::endl;
		Sleep(3000);

		driver::change_protection(base_address + 0x5AFF3F0, PAGE_EXECUTE_READWRITE, sizeof(uintptr_t));
		std::cout << "3" << std::endl;
		Sleep(3000);
		driver::write(base_address + 0x5AFF3F0, allocted_memory);
		std::cout << "4" << std::endl;
		Sleep(3000);
		//driver::change_protection(base_address + 0x5AFF3F0, PAGE_READONLY, sizeof(uintptr_t));

		do_once = false;
		return true;
	}

	static bool set_glow()
	{
		static float strength = -.5;
		uint64_t glow = driver::read<uint64_t>(base_address + glow_off);
		glow = driver::read<uint64_t>(glow + 0xB8);

		if (!glow)
			return false;

		if (settings::chams)
		{
			driver::write(glow + 0xD0, settings::rainbow_chams ? rainbow() : vec3_t(settings::chams_r, settings::chams_g, settings::chams_b));
			driver::write<vec_t>(glow + 0x110, { 0, 0 });
			driver::write<float>(glow + 0x11c, settings::rainbow_chams ? strength : settings::chams_oppacity);
		}
		else
		{
			driver::write<vec3_t>(glow + 0xD0, { 0, 0, 0 });
			driver::write<vec_t>(glow + 0x110, { 0, 0 });
			driver::write<float>(glow + 0x11c, 0);
		}
		return true;
	}

	static bool cav_esp()
	{
		BYTE spotted = settings::cav_esp;

		uintptr_t game_manager = driver::read<uintptr_t>(base_address + game_off);
		if (!game_manager)
			return NULL;

		uintptr_t entity_list = driver::read<uintptr_t>(game_manager + entity_list_off);
		int entity_count = driver::read<DWORD>(game_manager + entity_count_off) & 0x3fffffff;
		if (entity_count == NULL) 
			return false;

		for (int i = 0; i < entity_count; i++)
		{
			uintptr_t entity_object = driver::read<uintptr_t>(entity_list + i * 0x8);

			uintptr_t entity = driver::read<uintptr_t>(entity_object + 0x28);
			if (entity == NULL)
				continue;

			if (!is_enemy(entity))
				continue;

			entity = driver::read<uintptr_t>(entity + 0xD8);
			if (entity == NULL)
				continue;

			for (auto current_component = 0x80; current_component < 0xf0; current_component += sizeof(std::uintptr_t))
			{
				uintptr_t entity_component = driver::read<uintptr_t>(entity + current_component);
				if (entity_component == NULL)
					continue;

				if (driver::read<uintptr_t>(entity_component) != (base_address + vt_marker_off)) //vt marker
					continue;

				driver::write(entity_component + 0x552, spotted);
				driver::write(entity_component + 0x554, spotted);
			}
		}
		return true;
	}
}
