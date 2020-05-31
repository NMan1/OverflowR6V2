#include "menu.h"
#include "Settings.h"
#include <Lmcons.h>
#include <time.h>
#include "Config.h"

// Settings
using namespace g_Settings;

// Pages
static int iPage = 0;
bool Close = false;

const char* SelectionType[] = { ("Closest FOV") };
const char* BoneType[] = { ("Closest Bone"), ("Head"), ("Neck"), ("Chest"), ("Stomach"), ("Pelvis"), ("Hand"), ("Feet") };
const char* ConfigList[] = { "Default", "Legit", "Rage" };
int iCurrentConfig;

char pConfigName[12]{  };
static auto flags2 = ImGuiInputTextFlags_CharsNoBlank | ImGuiInputTextFlags_EnterReturnsTrue;

Config* DefualtCfg;
Config* LegitCfg;
Config* RageCfg;

void ResetSettings()
{
	// Visual
	bESP = false;
	bOutline = false;
	bTeam = false;
	bChams = false;
	bCavEsp = false;

	// Weapon
	bAimbot = false;
	bRunShoot = false;
	bDamage = false;
	bNoRecoil = false;
	bSpread = false;
	bSilent = false;

	// Misc
	bUnlockAll = false;
	bNoClip = false;
	bWepFov = false;
	bPlayerFov = false;
	bSpeed = false;
	bNoFlash = false;

	/* VALUES */

	// Misc
	fWepFov = .8f;
	fPlayerFov = 1.f;
	iSpeed = 100;
	fSpread = 1.f;
	iBullet = 1;
	iSmoothing = 0;

	// Chams
	fChamsR = 255;
	fChamsG = 255;
	fChamsB = 255;
	fChamsOpp = 3;

	// Aimbot
	iAimFov = 150;
}

void Menu::RenderButtons()
{
	if (iPage == 0)
	{
		PushStyleColor(ImGuiCol_Button, ActiveButton);
		PushStyleColor(ImGuiCol_ButtonHovered, ActiveButton);
		PushStyleColor(ImGuiCol_ButtonActive, ActiveButton);
		if (Button(("Visuals"), ImVec2(85, 0)))
			iPage = 0;
		PopStyleColor();
		PopStyleColor();
		PopStyleColor();
	}
	else
	{
		if (Button(("Visuals"), ImVec2(85, 0)))
			iPage = 0;
	}
	ImGui::SameLine();
	if (iPage == 1)
	{
		PushStyleColor(ImGuiCol_Button, ActiveButton);
		PushStyleColor(ImGuiCol_ButtonHovered, ActiveButton);
		PushStyleColor(ImGuiCol_ButtonActive, ActiveButton);
		if (Button(("Weapon"), ImVec2(85, 0)))
			iPage = 1;
		PopStyleColor();
		PopStyleColor();
		PopStyleColor();
	}
	else
	{
		if (Button(("Weapon"), ImVec2(85, 0)))
			iPage = 1;
	}
	ImGui::SameLine();
	if (iPage == 2)
	{
		PushStyleColor(ImGuiCol_Button, ActiveButton);
		PushStyleColor(ImGuiCol_ButtonHovered, ActiveButton);
		PushStyleColor(ImGuiCol_ButtonActive, ActiveButton);
		if (Button(("Misc"), ImVec2(85, 0)))
			iPage = 2;
		PopStyleColor();
		PopStyleColor();
		PopStyleColor();
	}
	else
	{
		if (Button(("Misc"), ImVec2(85, 0)))
			iPage = 2;
	}
	ImGui::SameLine();
	if (iPage == 3)
	{
		PushStyleColor(ImGuiCol_Button, ActiveButton);
		PushStyleColor(ImGuiCol_ButtonHovered, ActiveButton);
		PushStyleColor(ImGuiCol_ButtonActive, ActiveButton);
		if (Button(("Settings"), ImVec2(85, 0)))
			iPage = 3;
		PopStyleColor();
		PopStyleColor();
		PopStyleColor();
	}
	else
	{
		if (Button(("Settings"), ImVec2(85, 0)))
			iPage = 3;
	}
}

void Bar()
{
	ImGui::Spacing(); ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
}

void SameLineDummy(int xSpacing)
{
	ImGui::SameLine(); Dummy(ImVec2(xSpacing, 0)); ImGui::SameLine();
}

void Menu::RenderMenu()
{
	static bool DoOnce = true;
	if (DoOnce)
	{
		DefualtCfg = new Config("Defualt.cfg");
		LegitCfg = new Config("Legit.cfg");
		RageCfg = new Config("Rage.cfg");

		SetConfig(DefualtCfg);

		DoOnce = false;
	}

	// Style
	auto& style = GetStyle();

	// Menu
	SetNextWindowPos(ImVec2(0, 0));
	SetNextWindowSize(ImVec2(396, 411));
	static const auto dwFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar;
	Begin(("Overflow"), 0, dwFlags);
	{
		// Multi Colored Rect
		ImVec2 p = ImGui::GetCursorScreenPos();
		ImColor c = ImColor(32, 114, 247);
		ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x, p.y + 22), ImVec2(p.x + ImGui::GetWindowWidth() - 16, p.y), ImColor(30, 30, 39));
		ImGui::GetWindowDrawList()->AddRectFilledMultiColor(ImVec2(p.x, p.y + 22), ImVec2(p.x + ImGui::GetWindowWidth() - 16, p.y + 24), ImColor(234, 57, 57, 255), ImColor(58, 31, 87, 255), ImColor(58, 31, 87, 255), ImColor(167, 24, 71, 255));
		ImGui::Spacing();

		// Tilte
		PushFont(Font18);
		ImGui::Text(("Overflow    -"));
		ImGui::SameLine();
		PushStyleColor(ImGuiCol_Text, TabTitleText);
		ImGui::Text(Status.c_str());
		PopStyleColor();
		ImGui::PopFont();

		// Main
		ImGui::SetCursorPosX(8);
		ImGui::SetCursorPosY(32);
		BeginChild(("Main"), ImVec2(380, 0), true);
		{
			ImGui::PushFont(Font18);
			RenderButtons();

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing(); ImGui::Spacing();

			if (iPage == 0)
			{
				ImGui::Spacing();

				Checkbox(("Caveria Esp        "), &bCavEsp);
				SameLineDummy(60);
				Checkbox(("Player Chams"), &bChams);
				ImGui::Spacing();
				Checkbox(("Outline Color      "), &bColor);
				SameLineDummy(60);
				Checkbox(("No Flash"), &bNoFlash);

				Bar();

				ImGui::Text(("Chams Red"));
				ImGui::SetCursorPosX(25);
				if (SliderFloat(("##  #  "), &g_Settings::fChamsR, 0, 255, ("%.0f")))
				{
					g_Cheat::bCanDoChams = true;
					g_Settings::bUpdateSettings = true;
				}
				ImGui::Spacing(); ImGui::Spacing();
				ImGui::Text(("Chams Green"));
				ImGui::SetCursorPosX(25);
				if (SliderFloat(("##  "), &g_Settings::fChamsG, 0, 255, ("%.0f")))
				{
					g_Cheat::bCanDoChams = true;
					g_Settings::bUpdateSettings = true;
				}
				ImGui::Spacing(); ImGui::Spacing();
				ImGui::Text(("Chams Blue"));
				ImGui::SetCursorPosX(25);
				if (SliderFloat(("####  "), &g_Settings::fChamsB, 0, 255, ("%.0f")))
				{
					g_Cheat::bCanDoChams = true;
					g_Settings::bUpdateSettings = true;
				}
				ImGui::Spacing(); ImGui::Spacing();
				ImGui::Text(("Chams Brightness"));
				ImGui::SetCursorPosX(25);
				if (SliderFloat(("########    "), &fOpp, 0, 5, ("%.0f")))
				{
					switch ((int)fOpp)
					{
					case 0:
						g_Settings::fChamsOpp = 0; break;
					case 1:
						g_Settings::fChamsOpp = -1; break;
					case 2:
						g_Settings::fChamsOpp = -2; break;
					case 3:
						g_Settings::fChamsOpp = -3; break;
					case 4:
						g_Settings::fChamsOpp = -4; break;
					case 5:
						g_Settings::fChamsOpp = -5; break;
					}

					g_Cheat::bCanDoChams = true;
					g_Settings::bUpdateSettings = true;
				}
			}
			else if (iPage == 1)
			{
				ImGui::Spacing();

				Checkbox(("Enable Aimbot"), &bAimbot);
				SameLineDummy(60);
				Checkbox(("Bullet multiplier"), &bDamage);
				ImGui::Spacing();
				Checkbox(("No Recoil       "), &bNoRecoil);
				SameLineDummy(60);
				Checkbox(("Spread Control"), &bSpread);

				Bar();

				bool bSil = bSilent;
				Checkbox(("Silent Aim      "), &bSilent);
				if (bSil != bSilent)
					g_Settings::bUpdateSettings = true;

				SameLineDummy(61);
				Checkbox(("Aim Smoothing"), &bSmoothing);
				SameLineDummy(61);
				Checkbox(("Rapid Fire"), &rapid_fire);

				Bar();

				ImGui::PushItemWidth(144);
				ImGui::Text(("Aimbot FOV                     Aimbot Smooth"));
				ImGui::SetCursorPosX(25);
				if (SliderInt(("### "), &iAimFov, 1, 500))
					g_Settings::bUpdateSettings = true;

				ImGui::SameLine();

				ImGui::PushItemWidth(144);
				ImGui::SetCursorPosX(195 + 16);
				if (SliderFloat(("#####  "), &iSmoothing, 0, 10, ("%.1f")))
					g_Settings::bUpdateSettings = true;

				ImGui::Spacing(); ImGui::Spacing();

				ImGui::PushItemWidth(144);
				ImGui::Text(("Bullet Ammount                Spread Amount"));
				ImGui::SetCursorPosX(25);
				if (SliderFloat(("######     "), &fBullet, 1, 20, ("%.0f")))
				{
					iBullet = (int)fBullet;
					g_Settings::bUpdateSettings = true;
				}

				ImGui::SameLine();

				ImGui::PushItemWidth(144);
				ImGui::SetCursorPosX(195 + 16);
				if (SliderFloat(("####   "), &g_Settings::fSpread, 0, 5, ("%.0f")))
				{
					g_Cheat::bCanDoNS = true;
					g_Settings::bUpdateSettings = true;
				}

				ImGui::Spacing(); ImGui::Spacing();

				ImGui::PushItemWidth(144);
				ImGui::Text(("Aimbot Selection                Aimbot Bone"));
				ImGui::SetCursorPosX(25);
				if (Combo(("##      "), &g_Settings::iAimSelection, SelectionType, IM_ARRAYSIZE(SelectionType)))
					g_Settings::bUpdateSettings = true;

				//Spacing(); Spacing(); 
				ImGui::SameLine();

				ImGui::PushItemWidth(144);
				ImGui::SetCursorPosX(195 + 16);
				if (Combo(("####      "), &g_Settings::iAimBone, BoneType, IM_ARRAYSIZE(BoneType)))
					g_Settings::bUpdateSettings = true;
			}
			else if (iPage == 2)
			{
				ImGui::Spacing();
				Checkbox(("Unlock All"), &bUnlockAll);
				SameLineDummy(10);
				Checkbox(("No-Clip"), &bNoClip);
				SameLineDummy(10);
				Checkbox(("Speed Changer"), &bSpeed);
				ImGui::Spacing();
				Checkbox(("WeaponFov Changer"), &bWepFov);
				SameLineDummy(40);
				Checkbox(("Player Fov "), &bPlayerFov);
				SameLineDummy(40);
				Checkbox(("No Animations "), &no_animations);

				Bar();

				ImGui::Text(("Speed"));
				ImGui::SetCursorPosX(25);
				if (SliderFloat(("##   # "), &fValue, 1, 10, ("%.0f")))
				{
					switch ((int)fValue)
					{
					case 1:
						g_Settings::iSpeed = 110; break;
					case 2:
						g_Settings::iSpeed = 120; break;
					case 3:
						g_Settings::iSpeed = 130; break;
					case 4:
						g_Settings::iSpeed = 140; break;
					case 5:
						g_Settings::iSpeed = 150; break;
					case 6:
						g_Settings::iSpeed = 160; break;
					case 7:
						g_Settings::iSpeed = 170; break;
					case 8:
						g_Settings::iSpeed = 180; break;
					case 9:
						g_Settings::iSpeed = 190; break;
					case 10:
						g_Settings::iSpeed = 200; break;

					}
					g_Cheat::bCanDoSpeed = true;
					g_Settings::bUpdateSettings = true;
				}
				ImGui::Spacing(); ImGui::Spacing();  ImGui::Spacing();
				ImGui::Text(("Weapon FOV"));
				ImGui::SetCursorPosX(25);
				if (SliderFloat(("##  "), &g_Settings::fWepFov, .1, 3, ("%.1f")))
				{
					g_Cheat::bCanDoWepFov = true;
					g_Settings::bUpdateSettings = true;
				}
				ImGui::Spacing(); ImGui::Spacing();  ImGui::Spacing();
				ImGui::Text(("Player FOV"));
				ImGui::SetCursorPosX(25);
				if (SliderFloat(("####  "), &g_Settings::fPlayerFov, .1, 3, ("%.1f")))
				{
					g_Cheat::bCanDoPlayerFov = true;
					g_Settings::bUpdateSettings = true;
				}
			}
			else if (iPage == 3)
			{
				ImGui::Spacing();

				PushFont(Font18);
				ImGui::Text(("Aimbot Key:                         Backup Key:"));
				ImGui::PushItemWidth(144);
				ImGui::SetCursorPosX(25);
				HotKey((" "), &iAimKey, Font16);

				SameLineDummy(25);
				HotKey(("## "), &iAimKey2, Font16);

				Spacing();

				ImGui::Text(("No-Clip Toggle Key:"));
				ImGui::SetCursorPosX(25);
				HotKey(("###### "), &iNoClip, Font16);

				Bar();

				ImGui::Text(("Configs                                   Toggle Silent:"));
				ImGui::PushItemWidth(144);
				ImGui::SetCursorPosX(25);
				if (Combo(("###    "), &iCurrentConfig, ConfigList, IM_ARRAYSIZE(ConfigList)))
					g_Settings::bUpdateSettings = true;

				SameLineDummy(25);
				HotKey(("  "), &iSilentKey, Font16);

				ImGui::Spacing();

				if (Button(("Load Config"), ImVec2(178, 0)))
				{
					switch (iCurrentConfig)
					{
					case 0: ReadConfig(DefualtCfg); break;
					case 1: ReadConfig(LegitCfg); break;
					case 2: ReadConfig(RageCfg); break;
					}
				}

				ImGui::SameLine();

				if (Button(("Save Config"), ImVec2(178, 0)))
				{
					switch (iCurrentConfig)
					{
					case 0: SetConfig(DefualtCfg); break;
					case 1: SetConfig(LegitCfg); break;
					case 2: SetConfig(RageCfg); break;
					}
				}

				Bar();

				ImGui::SetCursorPosY(280);
				ImGui::PushItemWidth(175);
				if (Button(("Reset Features"), ImVec2(364, 0)))
				{
					ResetSettings();
					bReset = true;
				}

				ImGui::PopItemWidth();
				ImGui::PushItemWidth(175);
				if (Button(("Refresh"), ImVec2(364, 0)))
					bUpdate = true;
				ImGui::PopItemWidth();
				if (Button(("End Cheat"), ImVec2(178, 0)))
					bEnd = true;
				ImGui::SameLine();
				if (Button(("End Menu"), ImVec2(178, 0)))
					bEndUM = true;
				PopFont();
			}
			ImGui::PopFont();
		}
		EndChild();
	}
	End();

	return;
}