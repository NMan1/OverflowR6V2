#include <Lmcons.h>
#include <time.h>
#include "menu.h"
#include "../helpers/settings.h"
#include "imgui/imgui_internal.h"

// Settings
using namespace settings;

// Pages
static int iPage = 0;
bool Close = false;

const char* SelectionType[] = { ("Closest FOV") };
const char* BoneType[] = { ("Closest Bone"), ("Head"), ("Neck"), ("Chest"), ("Stomach"), ("Pelvis"), ("Hand"), ("Feet") };


void menu::render_buttons()
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

void bar()
{
	ImGui::Spacing(); ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
}

void same_line_dummy(int xSpacing)
{
	ImGui::SameLine(); Dummy(ImVec2(xSpacing, 0)); ImGui::SameLine();
}

void menu::render_menu()
{
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
		ImGui::Text(("Overflow    -    "));
		ImGui::SameLine();
		PushStyleColor(ImGuiCol_Text, TabTitleText);
		ImGui::Text("Rainbow Six");
		PopStyleColor();
		ImGui::PopFont();

		// Main
		ImGui::SetCursorPosX(8);
		ImGui::SetCursorPosY(32);
		BeginChild(("Main"), ImVec2(380, 0), true);
		{
			ImGui::PushFont(Font18);
			render_buttons();

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing(); ImGui::Spacing();

			if (iPage == 0)
			{
				ImGui::Spacing();

				Checkbox(("Caveria Esp        "), &cav_esp);
				same_line_dummy(60);
				Checkbox(("Player Chams"), &chams);
				ImGui::Spacing();
				same_line_dummy(60);
				Checkbox(("Rainbow"), &rainbow_chams);

				bar();

				ImGui::Text(("Chams Red"));
				ImGui::SetCursorPosX(25);
				SliderFloat(("##  #  "), &settings::chams_clr.r, 0, 255, ("%.0f"));
				ImGui::Spacing(); ImGui::Spacing();
				ImGui::Text(("Chams Green"));
				ImGui::SetCursorPosX(25);
				SliderFloat(("##  "), &settings::chams_clr.g, 0, 255, ("%.0f"));
				ImGui::Spacing(); ImGui::Spacing();
				ImGui::Text(("Chams Blue"));
				ImGui::SetCursorPosX(25);
				SliderFloat(("####  "), &settings::chams_clr.b, 0, 255, ("%.0f"));
				ImGui::Spacing(); ImGui::Spacing();
				ImGui::Text(("Chams Brightness"));
				ImGui::SetCursorPosX(25);
				SliderFloat(("########    "), &chams_oppacity, -2, 5, ("%.0f"));
			}
			else if (iPage == 1)
			{
				ImGui::Spacing();

				Checkbox(("Enable Aimbot"), &aimbot);
				same_line_dummy(60);
				Checkbox(("Bullet multiplier"), &damage);
				ImGui::Spacing();
				Checkbox(("No Recoil       "), &no_recoil);
				same_line_dummy(60);
				Checkbox(("Spread Control"), &no_spread);

				bar();

				Checkbox(("Silent Aim      "), &silent);

				same_line_dummy(61);
				Checkbox(("Aim Smoothing"), &smoothing);

				bar();

				ImGui::PushItemWidth(144);
				ImGui::Text(("Aimbot FOV                     Aimbot Smooth"));
				ImGui::SetCursorPosX(25);
				SliderInt(("### "), &aim_fov, 1, 500);

				ImGui::SameLine();

				ImGui::PushItemWidth(144);
				ImGui::SetCursorPosX(195 + 16);
				SliderFloat(("#####  "), &smoothing_value, 0, 10, ("%.1f"));

				ImGui::Spacing(); ImGui::Spacing();

				ImGui::PushItemWidth(144);
				ImGui::Text(("Bullet Ammount                Spread Amount"));
				ImGui::SetCursorPosX(25);
				if (SliderFloat(("######     "), &bullet_value_f, 1, 20, ("%.0f")))
					bullet_value = (int)bullet_value_f;
				
				ImGui::SameLine();

				ImGui::PushItemWidth(144);
				ImGui::SetCursorPosX(195 + 16);
				SliderFloat(("####   "), &settings::spread_value, 0, 5, ("%.0f"));

				ImGui::Spacing(); ImGui::Spacing();

				ImGui::PushItemWidth(144);
				ImGui::Text(("Aimbot Selection                Aimbot Bone"));
				ImGui::SetCursorPosX(25);
				Combo(("##      "), &settings::aim_selection, SelectionType, IM_ARRAYSIZE(SelectionType));

				//Spacing(); Spacing(); 
				ImGui::SameLine();

				ImGui::PushItemWidth(144);
				ImGui::SetCursorPosX(195 + 16);
				Combo(("####      "), &settings::aim_bone, BoneType, IM_ARRAYSIZE(BoneType));
			}
			else if (iPage == 2)
			{
				ImGui::Spacing();
				Checkbox(("Unlock All"), &unlock_all);
				same_line_dummy(10);
				Checkbox(("No-Clip"), &no_clip);
				same_line_dummy(10);
				Checkbox(("Speed Changer"), &speed);
				ImGui::Spacing();
				Checkbox(("Weapon Fov"), &wep_fov);
				same_line_dummy(95);
				Checkbox(("Player Fov "), &player_fov);
				Checkbox(("No Animations "), &no_animations);
				same_line_dummy(77);
				Checkbox(("Rapid Fire "), &rapid_fire);

				bar();

				ImGui::Text(("Speed"));
				ImGui::SetCursorPosX(25);
				SliderInt(("##   # "), &speed_value, 100, 250, ("%.0f"));
				ImGui::Spacing(); ImGui::Spacing();  ImGui::Spacing();
				ImGui::Text(("Weapon FOV"));
				ImGui::SetCursorPosX(25);
				SliderFloat(("##  "), &settings::wep_fov_value, .1, 10, ("%.1f"));
				ImGui::Spacing(); ImGui::Spacing();  ImGui::Spacing();
				ImGui::Text(("Player FOV"));
				ImGui::SetCursorPosX(25);
				SliderFloat(("####  "), &settings::player_fov_value, .1, 3, ("%.1f"));
			}
			else if (iPage == 3)
			{
				ImGui::Spacing();

				PushFont(Font18);
				ImGui::Text(("Aimbot Key:                         Backup Key:"));
				ImGui::PushItemWidth(144);
				ImGui::SetCursorPosX(25);
				HotKey((" "), &aim_key, Font16);

				same_line_dummy(25);
				HotKey(("## "), &aim_key_2, Font16);

				Spacing();

				ImGui::Text(("No-Clip Toggle Key:"));
				ImGui::SetCursorPosX(25);
				HotKey(("###### "), &no_clip_key, Font16);

				bar();

				ImGui::Text(("Configs                                   Toggle Silent:"));
				ImGui::PushItemWidth(144);
				ImGui::SetCursorPosX(25);
				/*Combo(("###    "), &iCurrentConfig, ConfigList, IM_ARRAYSIZE(ConfigList));

				same_line_dummy(25);
				HotKey(("  "), &silent_key, Font16);

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
				}*/

				bar();

				ImGui::SetCursorPosY(280);
				ImGui::PushItemWidth(175);
				if (Button(("Reset Features"), ImVec2(364, 0)))
				{

				}

				ImGui::PopItemWidth();
				if (Button(("End Cheat"), ImVec2(178, 0)))
					end = true;
				PopFont();
			}
			ImGui::PopFont();
		}
		EndChild();
	}
	End();

	return;
}