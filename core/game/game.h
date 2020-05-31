#pragma once
#include "driver.h"
#include "Settings.h"

extern DWORD64 base_address;

namespace game
{
	uint64_t get_round()
	{
		uint64_t gameMode = Read<uint64_t>(base_address + 0x60A2838);
		return Read<uint64_t>(gameMode + 0x230);
	}

	auto rainbow() //proper rainbow
	{
		static uint32_t cnt = 0;
		float freq = 0.06f;

		if (cnt++ >= (uint32_t)-1)
			cnt = 0;

		return std::make_tuple(std::sin(freq * cnt + 0) * 0.5f + 0.5f,
			std::sin(freq * cnt + 2) * 0.5f + 0.5f,
			std::sin(freq * cnt + 4) * 0.5f + 0.5f);
	}

	bool rapid_fire()
	{
		UINT64 ProfileListener = Read<UINT64>(Read<UINT64>(0x53ac118 + 0x68)); //deref
		UINT64 LocalPlayer = Read<UINT64>(ProfileListener + 0x28);
		uint64_t rapidFire = Read<uint64_t>(LocalPlayer + 0x90);
		rapidFire = Read<uint64_t>(rapidFire + 0xC8);
		if (g_Settings::rapid_fire)
			Write<BYTE>(rapidFire + 0x108, 0);
		else
			Write<BYTE>(rapidFire + 0x108, 1);
		return true;
	}

	bool no_animations()
	{
		uint64_t iAnims = Read<uint64_t>(base_address + 0x53beff0);
		iAnims = Read<uint64_t>(iAnims + 0x38);
		iAnims = Read<uint64_t>(iAnims + 0xC8);
		iAnims = Read<uint64_t>(iAnims + 0x0);
		iAnims = Read<uint64_t>(iAnims + 0x90);
		iAnims = Read<uint64_t>(iAnims + 0x118);
		iAnims = Read<uint64_t>(iAnims + 0xC8);
		if (g_Settings::no_animations)
			Write<BYTE>(iAnims + 0x36B, 0);
		else
			Write<BYTE>(iAnims + 0x36B, 1);
		return true;
	}

	bool speed_changer()
	{
		int ValueOfDefault = 100; //still clueless
		uintptr_t zoomer = Read<uintptr_t>(base_address + 0x53beff0);
		zoomer = Read<uintptr_t>(zoomer + 0xC8);
		zoomer = Read<uintptr_t>(zoomer + 0x0);
		zoomer = Read<uintptr_t>(zoomer + 0x30);
		zoomer = Read<uintptr_t>(zoomer + 0x30);
		zoomer = Read<uintptr_t>(zoomer + 0x38);
		if (g_Settings::speed)
			Write<int>(zoomer + 0x58, g_Settings::speed_value);
		else
			Write<int>(zoomer + 0x58, 100);
		return true;
	}

	bool no_clip(bool on = true)
	{
		uint64_t NC = Read<uint64_t>(base_address + 0x53ae880);
		NC = Read<uint64_t>(NC + 0xF8);
		NC = Read<uint64_t>(NC + 0x8);
		if (g_Settings::no_clip && !on)
		{
			Write<float>(NC + 0x7F0, -1.0f);
			Write<float>(NC + 0x7F4, -1.0f);
			Write<float>(NC + 0x7F8, -1.0f);
			Write<float>(NC + 0x7FC, -1.0f);
		}
		else
		{
			Write<float>(NC + 0x7F0, 1.f);
			Write<float>(NC + 0x7F4, 1.f);
			Write<float>(NC + 0x7F8, 1.f);
			Write<float>(NC + 0x7FC, 1.f);
		}
		return true;
	}

	bool no_spread()
	{
		uintptr_t noSpread = Read<uintptr_t>(base_address + 0x53beff0);
		noSpread = Read<uintptr_t>(noSpread + 0xC8);
		noSpread = Read<uintptr_t>(noSpread + 0x0);
		noSpread = Read<uintptr_t>(noSpread + 0x90);
		noSpread = Read<uintptr_t>(noSpread + 0xC8);
		noSpread = Read<uintptr_t>(noSpread + 0x278);

		if (!noSpread)
			return false;

		Write<bool>(noSpread + 0x58, g_Settings::no_spread);
		return true;
	}

	bool no_recoil() //not the best way
	{
		uintptr_t noRecoil = Read<uintptr_t>(base_address + 0x53beff8);
		noRecoil = Read<uintptr_t>(noRecoil + 0x110);
		noRecoil = Read<uintptr_t>(noRecoil + 0x0);
		if (!g_Settings::no_recoil)
			return false;

		Write<bool>(noRecoil + 0xe2d, g_Settings::no_recoil);
		return true;
	}

	bool set_glow()
	{
		float strength = -1.5f;
		uint64_t Glow = Read<uint64_t>(base_address + 0x60a2628);
		Glow = Read<uint64_t>(Glow + 0xB8);
		if (g_Settings::chams)
		{
			Write(Glow + 0xD0, Rainbow());
			Write<Vector2>(Glow + 0x110, { 0, 0 });
			Write<Vector2>(Glow + 0x110, { 1, 1 });
		}
		else
		{
			Write<Vector3>(Glow + 0xD0, { 0, 0, 0 });
			Write<Vector2>(Glow + 0x110, { 0, 0 });
			Write(Glow + 0x11c, strength);
		}
		return true;
	}
}
