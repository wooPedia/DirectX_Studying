/*
	*************************************
		2D Game programming #2
	*************************************

	SimpleWindow�� ����̽� ������ �߰��Ͽ� �������� �� �ִ� ȯ���� �����մϴ�.
*/

#pragma once
#pragma comment(linker, "/subsystem:windows")
#pragma comment(lib, "d3d9.lib") /* Direct3D�� ����ϱ� ���� ���̺귯�� ���� */

#include <windows.h>
#include <d3d9.h>
#include <memory>	   // std::unique_ptr

/* ������ ���� */
struct WindowInfo
{
	DWORD WinStyle	  = WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_VSCROLL;
	INT	  WinX		  = 20;
	INT	  WinY		  = 10;
	INT	  WinWidth    = 800;
	INT	  WinHeight   = 600;
	BOOL  bShowCursor = TRUE;
};

/* Direct3D ���� */
struct D3DInfo
{
	LPDIRECT3D9		  D3D		   = nullptr;   /* Direct3D ��ü */
	LPDIRECT3DDEVICE9 D3D_Device   = nullptr;	/* �׷��� ī�� ��ġ�� �߻�ȭ�� ����̽� ��ü */
	BOOL			  bWindowMode  = TRUE;		/* ������ ����� TRUE, Full Screen�̶�� FALSE */

};


class SimpleDevice final
{
public:
	SimpleDevice(LPCWSTR name, HINSTANCE hInst = GetModuleHandle(NULL));

	/* ������ & ����̽� ���� */
	INT	 Create();
	INT	 Run();


	/* ���� ���� ���� */
	INT    FrameMove();

private:
	static LRESULT		   MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT WINAPI  WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static void			   CleanUp();
	
	static INT			   Init();
	static void			   Destory();
	static INT			   Render();


	static std::unique_ptr<WindowInfo> mWinInfo;
	static std::unique_ptr<D3DInfo>	   mD3DInfo;

	LPCWSTR		mName;
	HINSTANCE	mhInst; /* ���α׷� �ν��Ͻ� �ڵ� */
	HWND		mhWnd;	/* ������ �ڵ� */
};


