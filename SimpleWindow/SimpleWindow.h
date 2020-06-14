/*
	*************************************
		2D Game programming #1
	*************************************

	������ ����� ���α׷� �ڵ� �帧

	1. main()
	2. ���α׷� �ڵ� ���
	3. ������ Ŭ���� ���
	4. ������ ����
	5. Run (Do something)
	6. ������ �Ҹ�
	7. ������ Ŭ���� ����

	Create -> Run -> CleanUp
*/

#pragma once
#pragma comment(linker, "/subsystem:windows")	 /* ������ â�� ��� �� �ְ� ���ݴϴ�. */

#include <windows.h>
#include <memory>	   // std::unique_ptr

/* ������ ���� */
struct WindowInfo
{
	DWORD WinStyle   = WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_VSCROLL;
	INT	  WinX		 = 20;
	INT	  WinY		 = 10;
	INT	  WinWidth   = 800;			
	INT	  WinHeight  = 600;			
};

class SimpleWindow final
{
public:
	SimpleWindow(LPCWSTR name, HINSTANCE hInst = GetModuleHandle(NULL));

	INT	 Create();	/* ������ Ŭ������ ���� �� ��� & DX �ʱ�ȭ */
	INT	 Run();		/* ���� ���� ���� */

private:
	static LRESULT		   MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);  /* Wrapper(WndProc���� ȣ��) */
	static LRESULT WINAPI  WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);  /* �޼��� ó�� ���� */
	static void			   CleanUp(); /* ������ �Ҹ� */

	static std::unique_ptr<WindowInfo> mWinInfo;

	LPCWSTR		mName;
	HINSTANCE	mhInst;	/* ���α׷� �ڵ� */
	HWND		mhWnd;  /* ������ �ڵ� */
};

