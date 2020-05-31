#pragma once
#include "driver.h"
#include "Settings.h"

extern DWORD64 base_address;

uint64_t GetCurrentGameMode() //ty c0le for the idea.
{
    uint64_t gameMode = Read<uint64_t>(base_address + 0x60A2838);
    gameMode = Read<uint64_t>(gameMode + 0x230);

    return gameMode; //for some reason with this check it lowers my fps unless i enable features, thonk...
}

auto Rainbow() //proper rainbow
{
    static uint32_t cnt = 0;
    float freq = 0.06f;

    if (cnt++ >= (uint32_t)-1)
        cnt = 0;

    return std::make_tuple(std::sin(freq * cnt + 0) * 0.5f + 0.5f,
        std::sin(freq * cnt + 2) * 0.5f + 0.5f,
        std::sin(freq * cnt + 4) * 0.5f + 0.5f);
}

bool noShotCooldown()
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

bool instantAnimations()
{
    uint64_t iAnims = Read<uint64_t>(base_address + 0x53beff0);
    iAnims = Read<uint64_t>(iAnims + 0x38);
    iAnims = Read<uint64_t>(iAnims + 0xC8);
    iAnims = Read<uint64_t>(iAnims + 0x0);
    iAnims = Read<uint64_t>(iAnims + 0x90);
    iAnims = Read<uint64_t>(iAnims + 0x118);
    iAnims = Read<uint64_t>(iAnims + 0xC8);
    if (g_Settings::no_animations)
    {
        Write<BYTE>(iAnims + 0x36B, 0);
    }
    else
    {
        Write<BYTE>(iAnims + 0x36B, 1);
    }
    return true;
}

bool speedMod()
{
    int ValueOfDefault = 100; //still clueless
    uintptr_t zoomer = Read<uintptr_t>(base_address + 0x53beff0);
    zoomer = Read<uintptr_t>(zoomer + 0xC8);
    zoomer = Read<uintptr_t>(zoomer + 0x0);
    zoomer = Read<uintptr_t>(zoomer + 0x30);
    zoomer = Read<uintptr_t>(zoomer + 0x30);
    zoomer = Read<uintptr_t>(zoomer + 0x38);
    if (g_Settings::bSpeed)
    {
        Write<int>(zoomer + 0x58, g_Settings::iSpeed);
    }
    else
    {
        Write<int>(zoomer + 0x58, ValueOfDefault);
    }
    return true;
}

bool nC(bool on = true)
{
    uint64_t NC = Read<uint64_t>(base_address + 0x53ae880);
    NC = Read<uint64_t>(NC + 0xF8);
    NC = Read<uint64_t>(NC + 0x8);
    if (g_Settings::bNoClip && !on)
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

bool nS()
{
    uintptr_t noSpread = Read<uintptr_t>(base_address + 0x53beff0);
    noSpread = Read<uintptr_t>(noSpread + 0xC8);
    noSpread = Read<uintptr_t>(noSpread + 0x0);
    noSpread = Read<uintptr_t>(noSpread + 0x90);
    noSpread = Read<uintptr_t>(noSpread + 0xC8);
    noSpread = Read<uintptr_t>(noSpread + 0x278);

    if (!noSpread) // This will prevent from writing when invalid
        return false; // This will prevent from writing when invalid

    Write<bool>(noSpread + 0x58, g_Settings::bSpread);
    return true;
}

bool nR() //not the best way
{
    uintptr_t noRecoil = Read<uintptr_t>(base_address + 0x53beff8);
    noRecoil = Read<uintptr_t>(noRecoil + 0x110);
    noRecoil = Read<uintptr_t>(noRecoil + 0x0);
    if (!g_Settings::bNoRecoil)
        return false;

    Write<bool>(noRecoil + 0xe2d, g_Settings::bNoRecoil);
    return true;
}

bool setGlow()
{
    float strength = -1.5f;
    uint64_t Glow = Read<uint64_t>(base_address + 0x60a2628);
    Glow = Read<uint64_t>(Glow + 0xB8);
    if (g_Settings::bChams)
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