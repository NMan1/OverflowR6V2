#pragma once
#include <dxgi.h>
#include <d3d11.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h" 
#include "imgui/imgui_internal.h"
#include "helpers/color.hpp"
#include "../helpers/vectors.h"
#include "../driver/driver.h"
#include "../helpers/settings.h"

using namespace ImGui;

namespace menu
{
	/* DirectX Pointers */
	extern ID3D11Device* g_pd3dDevice;
	extern ID3D11DeviceContext* g_pd3dDeviceContext;
	extern IDXGISwapChain* g_pSwapChain;
	extern ID3D11RenderTargetView* g_mainRenderTargetView;
	extern ID3D11BlendState* g_pBlendState;

	/* Menu Colors */
	extern ImVec4 TabSlide;
	extern ImVec4 TabTitleText;
	extern ImVec4 RedText;
	extern ImVec4 ActiveButton;
	extern ImVec4 GreenText;
	extern ImVec4 Underline;
	extern ImVec4 ActiveBut;
	extern ImVec4 RegButton;

	/* Fonts*/
	extern ImFont* Font12;
	extern ImFont* Font14;
	extern ImFont* Font16;
	extern ImFont* Font18;
	extern ImFont* Font24;
	extern ImFont* Icons;

	/* Handle*/
	extern WNDCLASSEX wc;
	extern HWND hwnd;
	void render_menu();

	void change_log();

	void legit_tab();

	void rage_tab();

	void visuals_tab();

	void misc_tab();

	void skins_tab();

	void settings_tab(); 

	void render_buttons();

	void render_menu();

    void init();

    void menu_shutdown();

    void begin_draw();

    void end_draw();

    void setup_menu();

};
