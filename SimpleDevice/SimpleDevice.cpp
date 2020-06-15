#include "SimpleDevice.h"

std::unique_ptr<D3DInfo> SimpleDevice::mD3DInfo = std::make_unique<D3DInfo>();

SimpleDevice::SimpleDevice(LPCWSTR name, HINSTANCE hInst)
	: mName(name)
	, mhInst(hInst)
	, mhWnd()
{
}

/* Wrapper */
LRESULT SimpleDevice::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
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

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

/* 메세지를 처리합니다. */
LRESULT WINAPI SimpleDevice::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return MsgProc(hWnd, msg, wParam, lParam);
}

/* 윈도우 클래스를 생성 및 등록 & DX 초기화 */
INT SimpleDevice::Create()
{
	/* static 멤버 변수 초기화 */
	std::unique_ptr<WindowInfo> mWinInfo = std::make_unique<WindowInfo>();

	/* 여기서부터 윈도우 클래스를 등록하고 생성합니다. */
	WNDCLASS wc =
	{
		  CS_CLASSDC
		, WndProc
		, 0L
		, 0L
		, mhInst
		, NULL
		, LoadCursor(NULL, IDC_ARROW)
		, (HBRUSH)GetStockObject(LTGRAY_BRUSH)
		, NULL
		, mName
	};

	RegisterClass(&wc);

	RECT rc;

	SetRect(&rc, 0, 0, 500, 300);
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW | WS_VISIBLE, FALSE);

	mhWnd = CreateWindow
	(
		mName
		, mName
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


	/* 여기서부터 DX를 초기화합니다. */

	/* SDK 버전에 따른 사용 가능 여부를 확인하고 Direct3D 객체를 생성합니다. */
	mD3DInfo->D3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (!mD3DInfo->D3D)
	{
		return -1;
	}

	/* 화면 제어, 픽셀, 더블 버퍼링 등과 같은 기능 set을 위해 D3DPRESENT_PARAMETERS 구조체 객체를 생성합니다. */
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.Windowed				 = mD3DInfo->bWindowMode;   /* 창모드 or 전체화면 */
	d3dpp.SwapEffect			 = D3DSWAPEFFECT_DISCARD;	/* 스왑 체인 방법을 설정 */
	d3dpp.EnableAutoDepthStencil = TRUE;					
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;				


	/* 그래픽 카드를 추상화한 디바이스 객체를 생성합니다. */
	auto createResult = mD3DInfo->D3D->CreateDevice
	(
		  D3DADAPTER_DEFAULT		       /* 디폴트 모니터를 대상으로 */
		, D3DDEVTYPE_HAL
		, mhWnd
		, D3DCREATE_MIXED_VERTEXPROCESSING /* Vertex 처리 관련 */
		, &d3dpp
		, &mD3DInfo->D3D_Device
	);

	/* 디바이스 객체 생성 실패 시 */
	if (FAILED(createResult))
	{
		createResult = mD3DInfo->D3D->CreateDevice
		(
			  D3DADAPTER_DEFAULT
			, D3DDEVTYPE_HAL
			, mhWnd
			, D3DCREATE_SOFTWARE_VERTEXPROCESSING	/* Vertex 처리 관련 */
			, &d3dpp
			, &mD3DInfo->D3D_Device
		);

		if (FAILED(createResult))
		{
			mD3DInfo->D3D->Release();
			return -1;
		}
	}

	ShowWindow(mhWnd, SW_SHOW);
	UpdateWindow(mhWnd);
	ShowCursor(mWinInfo->bShowCursor);

	if (FAILED(Init()))
	{
		return -1;
	}

	return 0;
}

INT SimpleDevice::Run()
{
	MSG msg;
	memset(&msg, 0, sizeof(msg));

	/* 메세지 루프로 진입합니다. */
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			/* 게임 루프 */
			if (FAILED(FrameMove()))
			{
				SendMessage(mhWnd, WM_QUIT, 0, 0);
			}

			if (FAILED(Render()))
			{
				SendMessage(mhWnd, WM_QUIT, 0, 0);
			}
		}
	}

	UnregisterClass(mName, mhInst);

	return 0;
}

void SimpleDevice::CleanUp()
{
	/*
		***********************
		 게임 데이터 + DX 해제
		***********************
	*/

	Destory();

	/* DirectX에 관련된 모든 객체들은 COM 객체를 상속받기 때문에 Release()를 통해서만 해제합니다. */
	if (mD3DInfo->D3D_Device)
	{
		mD3DInfo->D3D_Device->Release();
		mD3DInfo->D3D_Device = nullptr;
	}

	if (mD3DInfo->D3D)
	{
		mD3DInfo->D3D->Release();
		mD3DInfo->D3D = nullptr;
	}
}

INT SimpleDevice::Init()
{
	/*
		****************
		게임 데이터 생성
		****************
	*/

	return 0;
}

void SimpleDevice::Destory()
{
	/*
		****************
		게임 데이터 해제
		****************
	*/
}

INT SimpleDevice::FrameMove()
{
	return 0;
}

INT SimpleDevice::Render()
{
	/*
		모든 게임의 장면은 다음 순서를 통해 디바이스에 연출합니다.

		1. 화면을 Clear() 함수로 초기화
		2. 장면을 후면 버퍼(Back Face Buffer)에 그리기 위해 디바이스의 BeginScene() 함수를 호출
		3. 장면을 그림
		4. EndScene() 함수 호출을 통해 완성을 알림
		5. Present() 함수를 통해 전면 버퍼(Front Face Buffer)와 후면 버퍼를 교체
		6. 1~5 단계를 반복
	*/

	mD3DInfo->D3D_Device->Clear
	(
		  0
		, NULL
		, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER   /* 색상버퍼와 깊이버퍼를 채움 */
		, D3DCOLOR_XRGB(0, 120, 160)
		, 1.0f
		, 0
	);

	if (FAILED(mD3DInfo->D3D_Device->BeginScene()))
	{
		return -1;
	}

	/* 
		*********************************
		  이곳에 렌더링 작업을 구현합니다.
		*********************************
	*/

	mD3DInfo->D3D_Device->EndScene();

	/* 다음 장면의 픽셀 데이터가 저장된 후면 버퍼를 이전 장면이 담긴 전면 버퍼와 교체합니다. */
	mD3DInfo->D3D_Device->Present(0, 0, 0, 0);  

	return 0;
}