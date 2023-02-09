#include "../includes.h"

std::unique_ptr<CHook>m_pHook = std::make_unique<CHook>();

using fEndscene = HRESULT(__stdcall*)(IDirect3DDevice9*);
fEndscene pEndscene = NULL;
using fReset = long(__stdcall*)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
fReset pReset = NULL;

IDirect3D9* g_pD3D= nullptr;
IDirect3DDevice9* device = nullptr;
ID3D11DeviceContext* context = nullptr;

enum IDirect3DDevice9vTable //for dx9
{
    RESET = 16,
    ENDSCENE=42
};



void InitImGui(IDirect3DDevice9* pd3dDevice)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsClassic();

    auto& style = ImGui::GetStyle();

    style.FrameRounding = 3.f;
    style.ChildRounding = 3.f;
    style.ChildBorderSize = 1.f;
    style.ScrollbarSize = 0.6f;
    style.ScrollbarRounding = 3.f;
    style.GrabRounding = 3.f;
    style.WindowRounding = 0.f;

    style.Colors[ImGuiCol_FrameBg] = ImColor(200, 200, 200);
    style.Colors[ImGuiCol_FrameBgHovered] = ImColor(220, 220, 220);
    style.Colors[ImGuiCol_FrameBgActive] = ImColor(230, 230, 230);
    style.Colors[ImGuiCol_Separator] = ImColor(180, 180, 180);
    style.Colors[ImGuiCol_CheckMark] = ImColor(255, 172, 19);
    style.Colors[ImGuiCol_SliderGrab] = ImColor(255, 172, 19);
    style.Colors[ImGuiCol_SliderGrabActive] = ImColor(255, 172, 19);
    style.Colors[ImGuiCol_ScrollbarBg] = ImColor(120, 120, 120);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImColor(255, 172, 19);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImGui::GetStyleColorVec4(ImGuiCol_ScrollbarGrab);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImGui::GetStyleColorVec4(ImGuiCol_ScrollbarGrab);
    style.Colors[ImGuiCol_Header] = ImColor(160, 160, 160);
    style.Colors[ImGuiCol_HeaderHovered] = ImColor(200, 200, 200);
    style.Colors[ImGuiCol_Button] = ImColor(180, 180, 180);
    style.Colors[ImGuiCol_ButtonHovered] = ImColor(200, 200, 200);
    style.Colors[ImGuiCol_ButtonActive] = ImColor(230, 230, 230);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.78f, 0.78f, 0.78f, 1.f);
    style.Colors[ImGuiCol_WindowBg] = ImColor(220, 220, 220, 0.7 * 255);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.40f, 0.40f, 0.80f, 0.20f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.40f, 0.40f, 0.55f, 0.80f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.72f, 0.72f, 0.72f, 0.70f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.83f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.75f, 0.75f, 0.75f, 0.87f);
    style.Colors[ImGuiCol_Text] = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);
    style.Colors[ImGuiCol_ChildBg] = ImVec4(0.72f, 0.72f, 0.72f, 0.76f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.76f, 0.76f, 0.76f, 1.00f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.81f, 0.81f, 0.81f, 1.00f);
    style.Colors[ImGuiCol_Tab] = ImVec4(0.61f, 0.61f, 0.61f, 0.79f);
    style.Colors[ImGuiCol_TabHovered] = ImVec4(0.71f, 0.71f, 0.71f, 0.80f);
    style.Colors[ImGuiCol_TabActive] = ImVec4(0.77f, 0.77f, 0.77f, 0.84f);
    style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.73f, 0.73f, 0.73f, 0.82f);
    style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.58f, 0.58f, 0.58f, 0.84f);

    auto& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Verdana.ttf", 15.0f, NULL, io.Fonts->GetGlyphRangesCyrillic());
    m_pMenuFonts->LoadFonts();


    ImGui_ImplWin32_Init(globals::hGame);
    ImGui_ImplDX9_Init(pd3dDevice);

    ImGuiWindowFlags flags_color_edit = ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoInputs;
    ImGui::SetColorEditOptions(flags_color_edit);
}

void BeginScene()
{
    if (globals::unload_dll)
        return;

    // 界面开始绘制
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    if (vars::bMenuOpen)
    {
        ImGui::GetIO().MouseDrawCursor = true;
        m_pMenu->Run();
    }
    else {
        ImGui::GetIO().MouseDrawCursor = false;
    }
    

    m_pHack->ScreenInfo();

    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4());
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
    ImGui::Begin("##BackBuffer", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoSavedSettings);
    ImGui::SetWindowPos(ImVec2(), ImGuiCond_Always);
    ImGui::SetWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y), ImGuiCond_Always);

    m_pHack->Run();
 

    ImGui::GetCurrentWindow()->DrawList->PushClipRectFullScreen();
    ImGui::End();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();

    ImGui::EndFrame();

    ImGui::Render();

    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

}

HRESULT __stdcall EndScene_Hooked(IDirect3DDevice9* pd3dDevice)
{
    static auto once = [pd3dDevice]()
    {
        InitImGui(pd3dDevice);
        std::cout << __FUNCTION__ << " > first called!" << std::endl;
        return true;
    }();

    
    

    BeginScene();

    return pEndscene(pd3dDevice);
}

HRESULT __stdcall Reset_Hooked(IDirect3DDevice9* pd3dDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
    static auto once = []()
    {
        std::cout << __FUNCTION__ << " > first called!" << std::endl;
        return true;
    }();

    ImGui_ImplDX9_InvalidateDeviceObjects();
    //HRESULT ret= pReset(pd3dDevice, pPresentationParameters);
    ImGui_ImplDX9_CreateDeviceObjects();

    return pReset(pd3dDevice, pPresentationParameters);
}


void CHook::SetupDX9Hook()
{

	g_pD3D =Direct3DCreate9(D3D_SDK_VERSION);

    D3DPRESENT_PARAMETERS g_d3dpp = {};
    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN; 
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    //g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
    
    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, globals::hGame, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &device) < 0)
    {
        std::cout << "failed to create device\n";
        return;
    }

    void** pVTabledevice = *reinterpret_cast<void***>(device);

    
    this->pEndSceneAddress = reinterpret_cast<LPVOID>(pVTabledevice[IDirect3DDevice9vTable::ENDSCENE]);
    this->pResetAddress = reinterpret_cast<LPVOID>(pVTabledevice[IDirect3DDevice9vTable::RESET]);

    if (MH_CreateHook(this->pEndSceneAddress, &EndScene_Hooked, (LPVOID*)&pEndscene) != MH_OK
        || MH_EnableHook(this->pEndSceneAddress) != MH_OK)
    {
        std::cout << "failed create hook EndScene\n";
        return;
    }

    if (MH_CreateHook(this->pResetAddress, &Reset_Hooked, (LPVOID*)&pReset) != MH_OK
        || MH_EnableHook(this->pResetAddress) != MH_OK)
    {
        std::cout << "failed create hook Reset\n";
        return;
    }
}



LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc_Hooked(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static auto once = []()
    {
        std::cout << __FUNCTION__ << " first called!" << std::endl;

        return true;
    }();
    //VK_HOME VK_INSERT
    //如果按下INS键，就打开或关闭外挂设置界面，如果之前是关闭的就打开，如果是打开的就关闭。
    if (uMsg == WM_KEYDOWN && wParam == VK_HOME)
    {
        vars::bMenuOpen = !vars::bMenuOpen;
        return FALSE;
    }

    //如果外挂设置界面是打开状态，则调用ImGui的消息处理
    if (vars::bMenuOpen && ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
    {
        return TRUE;
    }

    switch (uMsg)
    {
        //在失去键盘焦点之前立即发送到窗口
    case WM_KILLFOCUS:
        globals::killed_focus = true;
        break;
        //在窗口获得键盘焦点后发送到窗口
    case WM_SETFOCUS:
        globals::killed_focus = false;
        break;


    //case WM_KEYDOWN:
    //    switch (wParam)  //设置渲染图元类型
    //    {
    //    case VK_UP:  //光标键上
    //        globals::unload_dll = true;
    //        break;
    //    }
    default: break;
    }

    //调用原窗口处理消息的函数来处理其他消息，https://blog.csdn.net/wangpengk7788/article/details/55053053
    return CallWindowProc(m_pHook->pWndProc, hwnd, uMsg, wParam, lParam);
}

void CHook::SetupWndProcHook()
{
    this->pWndProc = (WNDPROC)SetWindowLongPtr(globals::hGame, GWLP_WNDPROC, (LONG_PTR)WndProc_Hooked);
}