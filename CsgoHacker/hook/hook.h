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
//����ָ����,�൱�ڴ�����һ��ָ��CHook��Ŀ�ָ�롣
extern std::unique_ptr<CHook>m_pHook;

extern IDirect3D9* g_pD3D;
extern IDirect3DDevice9* device;
