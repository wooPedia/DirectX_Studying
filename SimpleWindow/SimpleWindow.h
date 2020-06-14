/*
	*************************************
		2D Game programming #1
	*************************************

	윈도우 기반의 프로그램 코드 흐름

	1. main()
	2. 프로그램 핸들 얻기
	3. 윈도우 클래스 등록
	4. 윈도우 생성
	5. Run (Do something)
	6. 윈도우 소멸
	7. 윈도우 클래스 해제

	Create -> Run -> CleanUp
*/

#pragma once
#pragma comment(linker, "/subsystem:windows")	 /* 윈도우 창을 띄울 수 있게 해줍니다. */

#include <windows.h>
#include <memory>	   // std::unique_ptr

/* 윈도우 정보 */
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

	INT	 Create();	/* 윈도우 클래스를 생성 및 등록 & DX 초기화 */
	INT	 Run();		/* 게임 루프 실행 */

private:
	static LRESULT		   MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);  /* Wrapper(WndProc에서 호출) */
	static LRESULT WINAPI  WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);  /* 메세지 처리 루프 */
	static void			   CleanUp(); /* 데이터 소멸 */

	static std::unique_ptr<WindowInfo> mWinInfo;

	LPCWSTR		mName;
	HINSTANCE	mhInst;	/* 프로그램 핸들 */
	HWND		mhWnd;  /* 윈도우 핸들 */
};

