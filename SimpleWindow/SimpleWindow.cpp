#include "SimpleWindow.h"

/* static 멤버 변수 초기화 */
std::unique_ptr<WindowInfo> SimpleWindow::mWinInfo = std::make_unique<WindowInfo>();

/* 생성자에서 프로그램 핸들을 얻습니다. */
SimpleWindow::SimpleWindow(LPCWSTR name, HINSTANCE hInst)
	: mName(name)
	, mhInst(hInst)
	, mhWnd()
{
}

LRESULT SimpleWindow::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_KEYDOWN:
		{
			switch (wParam)
			{
				case VK_ESCAPE:
				{
					SendMessage(hWnd, WM_DESTROY, 0, 0);
					break;
				}
			}

			return 0;
		}

		case WM_DESTROY:
		{
			CleanUp();
			PostQuitMessage(0); /* 메세지큐에 WM_QUIT 메세지 전달 */
			return 0;
		}
	}

	return DefWindowProc(hWnd, msg, wParam, lParam); /* 프로그램이 처리하지 않은 잡다한 메세지를 처리 */
}

LRESULT WINAPI SimpleWindow::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return MsgProc(hWnd, msg, wParam, lParam);
}

INT SimpleWindow::Create()
{
	/* 윈도우 클래스를 등록합니다. */
	WNDCLASS wc =
	{
		  CS_CLASSDC
		, WndProc		/* 메세지 처리 함수인 WndProc을 등록해주어야 함 */
		, 0L
		, 0L
		, mhInst		/* WinMain으로부터 받은 프로그램 핸들(인스턴스) */
		, NULL
		, LoadCursor(NULL, IDC_ARROW)
		, (HBRUSH)GetStockObject(LTGRAY_BRUSH)
		, NULL
		, mName		    /* 윈도우 클래스 이름 */
	};

	RegisterClass(&wc);

	/* 윈도우 영역을 정해줍니다. 지금은 크게 신경X */
	RECT rc;

	SetRect(&rc, 0, 0, 500, 300);
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW | WS_VISIBLE, FALSE);

	/* 윈도우를 생성합니다. */
	mhWnd = CreateWindow
	(
		  mName					/* WNDCLASS 타입 객체의 윈도우 클래스 이름과 동일 */
		, mName					/* 윈도우 타이틀 바에 띄울 이름 */
		, mWinInfo->WinStyle	
		, 20					/*   x	  */
		, 10					/*   y	  */
		, mWinInfo->WinWidth	/* width  */
		, mWinInfo->WinHeight	/* height */
		, GetDesktopWindow()	/* 부모 또는 소유주 윈도우의 핸들 지정 */
		, NULL
		, mhInst
		, NULL
	);

	ShowWindow(mhWnd, SW_SHOW);
	UpdateWindow(mhWnd);
	ShowCursor(TRUE);

	/*
		***********************
		여기에 DX를 초기화합니다.
		***********************
	*/


	return 0;
}

INT SimpleWindow::Run()
{
	// 메세지 루프로 진입합니다.
	MSG msg;
	memset(&msg, 0, sizeof(msg));

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			/*
				**************
				게임 루프 작성
				**************
			*/
		}
	}

	UnregisterClass(mName, mhInst);

	return 0;
}

void SimpleWindow::CleanUp()
{
	/*
		***********************
		 게임 데이터 + DX 해제
		***********************
	*/
}