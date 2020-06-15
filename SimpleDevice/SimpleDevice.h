/*
	*************************************
		2D Game programming #2
	*************************************

	SimpleWindow에 디바이스 생성을 추가하여 렌더링할 수 있는 환경을 구성합니다.
*/

#pragma once
#pragma comment(linker, "/subsystem:windows")
#pragma comment(lib, "d3d9.lib") /* Direct3D를 사용하기 위한 라이브러리 연결 */

#include <windows.h>
#include <d3d9.h>
#include <memory>	   // std::unique_ptr

/* 윈도우 정보 */
struct WindowInfo
{
	DWORD WinStyle	  = WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_VSCROLL;
	INT	  WinX		  = 20;
	INT	  WinY		  = 10;
	INT	  WinWidth    = 800;
	INT	  WinHeight   = 600;
	BOOL  bShowCursor = TRUE;
};

/* Direct3D 정보 */
struct D3DInfo
{
	LPDIRECT3D9		  D3D		   = nullptr;   /* Direct3D 객체 */
	LPDIRECT3DDEVICE9 D3D_Device   = nullptr;	/* 그래픽 카드 장치를 추상화한 디바이스 객체 */
	BOOL			  bWindowMode  = TRUE;		/* 윈도우 모드라면 TRUE, Full Screen이라면 FALSE */

};


class SimpleDevice final
{
public:
	SimpleDevice(LPCWSTR name, HINSTANCE hInst = GetModuleHandle(NULL));

	/* 윈도우 & 디바이스 관련 */
	INT	 Create();
	INT	 Run();


	/* 게임 실행 관련 */
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
	HINSTANCE	mhInst; /* 프로그램 인스턴스 핸들 */
	HWND		mhWnd;	/* 윈도우 핸들 */
};


