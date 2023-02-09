// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "includes.h"


namespace console
{
	FILE* output_stream = nullptr;

	void attach(const char* name)
	{
		if (AllocConsole())
		{
			freopen_s(&output_stream, "conout$", "w", stdout);
		}
		SetConsoleTitle(name);
	}

	void detach()
	{
		if (output_stream)
		{
			fclose(output_stream);
		}
		FreeConsole();
	}
}

#define RAISE_ERROR(check_var, error_message, success_message) \
if (!check_var) \
{ \
	MessageBoxA(NULL, error_message, "csgo hack", MB_OK | MB_ICONERROR); \
	FreeLibraryAndExitThread(globals::hmModule, 1); \
} \
else \
{ \
	std::cout << success_message << "0x" << std::hex << check_var << std::endl; \
} \

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	auto process_id_that_interests_us_very_much = GetCurrentProcessId();

	HWND* cur_hwnd = (HWND*)lParam;

	if ((!GetWindow(hwnd, GW_OWNER)) && IsWindow(hwnd))
	{
		DWORD process_id = NULL;
		GetWindowThreadProcessId(hwnd, &process_id);

		char* text_window = new char[255];

		GetWindowText(hwnd, text_window, 255);

		if (process_id_that_interests_us_very_much == process_id && strstr(text_window, "Counter-Strike") && !strstr(text_window, ".exe"))
		{
			std::cout << "Window: " << text_window << std::endl;
			*cur_hwnd = hwnd;
			return 0;
		}
	}

	return 1;
}

void SetupHackThread(void)
{
	//开启一个控制台用来输出一些信息。
	console::attach("csgo console debug");

	//获取Battlefield窗口的句柄
	EnumWindows(&EnumWindowsProc, (LPARAM)&globals::hGame);

	RAISE_ERROR(globals::hGame, "Error find window", "window handle: ");

	vars::load_default_settings();

	if (!m_pHack->GetHandle())
	{
		std::cout << "Error get handle\n";
	}
	//minhook的初始化
	if (MH_Initialize() != MH_OK)
	{
		MessageBoxA(NULL, "Error initialize minhook", "csgo hack", MB_OK | MB_ICONERROR);
	}
	//DirectX9 Hook
	m_pHook->SetupDX9Hook();

	RAISE_ERROR(m_pHook->pEndSceneAddress, "Error hook DX9", "EndScene");
	RAISE_ERROR(m_pHook->pResetAddress, "Error hook DX9", "Reset: ");

	//调用SetWindowLongPtr函数修改了游戏窗口的WndProc，也就是窗口的消息处理函数，具体的消息处理函数将在对应函数位置进行分析。
	m_pHook->SetupWndProcHook();
	RAISE_ERROR(m_pHook->pWndProc, "Error hook wndproc", "wndproc: ")

	while (true)
	{
		if (globals::unload_dll) break;
		Sleep(228);
	}

	vars::bMenuOpen = false;
	Sleep(30);

	vars::global::enable= false;
	Sleep(100);

	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	Sleep(100);

	MH_DisableHook(m_pHook->pEndSceneAddress);
	MH_RemoveHook(m_pHook->pEndSceneAddress);
	Sleep(100);

	MH_DisableHook(m_pHook->pResetAddress);
	MH_RemoveHook(m_pHook->pResetAddress);

	MH_Uninitialize();
	Sleep(100);

	SetWindowLongPtr(globals::hGame, GWLP_WNDPROC, (LONG_PTR)m_pHook->pWndProc);

	Sleep(100);

	//free library
	std::cout << "free library...\n\n";
	FreeLibraryAndExitThread(globals::hmModule, 0);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		globals::hmModule = hModule;
        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)SetupHackThread, NULL, NULL, NULL);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

