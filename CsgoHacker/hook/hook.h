#pragma once
class CHook
{
public:
	PVOID pEndSceneAddress;
	PVOID pResetAddress;
	PVOID pSetCursorPosAddress;
	WNDPROC pWndProc;
	void SetupDX9Hook();
	void SetupWndProcHook();
};
//智能指针类,相当于创建了一个指向CHook类的空指针。
extern std::unique_ptr<CHook>m_pHook;

extern IDirect3D9* g_pD3D;
extern IDirect3DDevice9* device;
