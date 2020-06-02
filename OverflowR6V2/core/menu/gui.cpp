#include <string>
#include <thread>
#include <dwmapi.h>
#include <iostream>
#include <ShlObj.h>
#include "menu.h"
#include "../helpers/settings.h"
#include "imgui/imgui_internal.h"

#pragma comment(lib, "dwmapi.lib")
#define STB_IMAGE_IMPLEMENTATION

using namespace ImGui;
using namespace settings;

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
ImVec4 clear_color;
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
std::string status = {};

namespace menu
{
	/* DirectX Pointers */
	ID3D11Device* g_pd3dDevice = NULL;
	ID3D11DeviceContext* g_pd3dDeviceContext = NULL;
	IDXGISwapChain* g_pSwapChain = NULL;
	ID3D11RenderTargetView* g_mainRenderTargetView = NULL;
	ID3D11BlendState* g_pBlendState = NULL;

	/* Menu Colors */
	ImVec4 TabSlider = { 0.929f, 0.290f, 0.290f, 1.00f };
	ImVec4 TabTitleText = { 0.929f, 0.290f, 0.290f, 1.00f };
	ImVec4 RedText = { 1.f, 0.0f, 0.0f, .95f };
	ImVec4 ActiveButton = { 0.917, 0.223, 0.223, 1.f };
	ImVec4 GreenText = { 0.f, 1.0f, 0.0f, .95f };
	ImVec4 Underline = { 0.f, 0.749f, 1.f, 1.00f };
	ImVec4 ActiveBut = { 0.2f, 0.2f, 0.231f, 1.00f };
	ImVec4 RegButton = { 0.137f, 0.137f, 0.160f, 1.00f };

	/* Fonts*/
	ImFont* Font12 = NULL;
	ImFont* Font14 = NULL;
	ImFont* Font16 = NULL;
	ImFont* Font18 = NULL;
	ImFont* Font24 = NULL;
	ImFont* Icons = NULL;

	/* Handle*/
	WNDCLASSEX wc = { NULL };
	HWND hwnd = NULL;
	bool bDisplayAdmin = false;

	LPCTSTR Logo = "D:\\Everything\\Overflow\\Loader.png";
	ID3D11ShaderResourceView* LogoTexture = NULL;
}

void menu::init()
{
	/* Colors */
	StyleColorsBlueMain();
	SetColorEditOptions(ImGuiColorEditFlags_HEX);

	/* Setup Style */
	ImGuiStyle& style = GetStyle();
	status = " Rainbow Six Seige";

	style.Alpha = 1.f;
	style.WindowPadding = ImVec2(8, 8);
	style.WindowMinSize = ImVec2(32, 32);
	style.WindowRounding = 0.0f; //4.0 for slight curve
	style.WindowTitleAlign = ImVec2(0.5f, 1.5f);
	style.ChildRounding = 0.0f;
	style.FramePadding = ImVec2(4, 3);
	style.FrameRounding = 0.0f; //2.0
	style.ItemSpacing = ImVec2(8, 4);
	style.ItemInnerSpacing = ImVec2(4, 4);
	style.TouchExtraPadding = ImVec2(0, 0);
	style.IndentSpacing = 21.0f;
	style.ColumnsMinSpacing = 3.0f;
	style.ScrollbarSize = 6.0f;
	style.ScrollbarRounding = 16.0f; //16.0
	style.GrabMinSize = 0.1f;
	style.GrabRounding = 16.0f; //16.0
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.DisplayWindowPadding = ImVec2(22, 22);
	style.DisplaySafeAreaPadding = ImVec2(4, 4);
	style.AntiAliasedLines = true;
	style.CurveTessellationTol = 1.25f;

	ImGuiIO& io = GetIO();
	io.IniFilename = nullptr;
	io.LogFilename = nullptr;

	TCHAR szPath[MAX_PATH];
	SHGetFolderPath(NULL, CSIDL_FONTS, NULL, NULL, szPath);

	std::string Path = szPath;
	std::string pTahoma{ Path + "\\Tahoma.ttf" };
	//std::string pRuda{ Path + "\\Ruda-Bold.ttf" };

	Font16 = io.Fonts->AddFontFromFileTTF(pTahoma.c_str(), 16.0f);
	Font12 = io.Fonts->AddFontFromFileTTF(pTahoma.c_str(), 12.0f);
	Font14 = io.Fonts->AddFontFromFileTTF(pTahoma.c_str(), 14.0f);
	Font18 = io.Fonts->AddFontFromFileTTF(pTahoma.c_str(), 18.0f);
	Font24 = io.Fonts->AddFontFromFileTTF(pTahoma.c_str(), 24.0f);

	int my_image_width = 370;
	int my_image_height = 90;

	return;
}

void menu::menu_shutdown()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	CleanupDeviceD3D();
	::DestroyWindow(hwnd);
	::UnregisterClass(wc.lpszClassName, wc.hInstance);
	return;
}

void menu::begin_draw()
{
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
}

void menu::end_draw()
{
	// Rendering
	float clearColor[4] = { 0.0f,0.0f,0.0f,0.0f };
	ImGui::Render();
	g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
	g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clearColor);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	g_pSwapChain->Present(1, 0); // Present with vsync
}

void menu::setup_menu()
{
	// Create application window
	wc = { sizeof(WNDCLASSEX), NULL, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, ("Overflow"), NULL };
	::RegisterClassEx(&wc);
	hwnd = ::CreateWindow(wc.lpszClassName, _T(("Overflow  -  Rainbow 6 Seige")), 0, 1100, 300, 412, 450, NULL, NULL, wc.hInstance, NULL);

	// Initialize Direct3D
	if (!CreateDeviceD3D(hwnd))
	{
		CleanupDeviceD3D();
		::UnregisterClass(wc.lpszClassName, wc.hInstance);
		return;
	}

	SetLayeredWindowAttributes(hwnd, 0, 1.0f, LWA_ALPHA);
	SetLayeredWindowAttributes(hwnd, 0, RGB(0, 0, 0), LWA_COLORKEY);

	// Show the window
	::ShowWindow(hwnd, SW_SHOW);
	::UpdateWindow(hwnd);

	//MARGINS margins = { -1 };
	//DwmExtendFrameIntoClientArea(hwnd, &margins);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Platform/Renderer bindings
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

	// Curosr
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	// Colors
	init();

	// Our state
	bool show_demo_window = true;
	bool show_another_window = false;
	clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	return;
}

bool CreateDeviceD3D(HWND hWnd)
{
	// Setup swap chain
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	UINT createDeviceFlags = 0;
	//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
	if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &menu::g_pSwapChain, &menu::g_pd3dDevice, &featureLevel, &menu::g_pd3dDeviceContext) != S_OK)
		return false;

	CreateRenderTarget();
	return true;
}

void CleanupDeviceD3D()
{
	CleanupRenderTarget();
	if (menu::g_pSwapChain) { menu::g_pSwapChain->Release(); menu::g_pSwapChain = NULL; }
	if (menu::g_pd3dDeviceContext) { menu::g_pd3dDeviceContext->Release(); menu::g_pd3dDeviceContext = NULL; }
	if (menu::g_pd3dDevice) { menu::g_pd3dDevice->Release(); menu::g_pd3dDevice = NULL; }
	return;
}

void CreateRenderTarget()
{
	ID3D11Texture2D* pBackBuffer;
	menu::g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	menu::g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &menu::g_mainRenderTargetView);
	pBackBuffer->Release();
	return;
}

void CleanupRenderTarget()
{
	if (menu::g_mainRenderTargetView) { menu::g_mainRenderTargetView->Release(); menu::g_mainRenderTargetView = NULL; }
	return;
}

// Win32 message handler
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	RECT rect;
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
		if (menu::g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
		{
			CleanupRenderTarget();
			menu::g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
			CreateRenderTarget();
		}
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}