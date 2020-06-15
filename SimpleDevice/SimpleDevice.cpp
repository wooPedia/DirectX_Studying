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
			PostQuitMessage(0); /* �޼���ť�� WM_QUIT �޼��� ���� */
			return 0;
		}
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

/* �޼����� ó���մϴ�. */
LRESULT WINAPI SimpleDevice::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return MsgProc(hWnd, msg, wParam, lParam);
}

/* ������ Ŭ������ ���� �� ��� & DX �ʱ�ȭ */
INT SimpleDevice::Create()
{
	/* static ��� ���� �ʱ�ȭ */
	std::unique_ptr<WindowInfo> mWinInfo = std::make_unique<WindowInfo>();

	/* ���⼭���� ������ Ŭ������ ����ϰ� �����մϴ�. */
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
		, GetDesktopWindow()	/* �θ� �Ǵ� ������ �������� �ڵ� ���� */
		, NULL
		, mhInst
		, NULL
	);

	ShowWindow(mhWnd, SW_SHOW);
	UpdateWindow(mhWnd);
	ShowCursor(TRUE);


	/* ���⼭���� DX�� �ʱ�ȭ�մϴ�. */

	/* SDK ������ ���� ��� ���� ���θ� Ȯ���ϰ� Direct3D ��ü�� �����մϴ�. */
	mD3DInfo->D3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (!mD3DInfo->D3D)
	{
		return -1;
	}

	/* ȭ�� ����, �ȼ�, ���� ���۸� ��� ���� ��� set�� ���� D3DPRESENT_PARAMETERS ����ü ��ü�� �����մϴ�. */
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.Windowed				 = mD3DInfo->bWindowMode;   /* â��� or ��üȭ�� */
	d3dpp.SwapEffect			 = D3DSWAPEFFECT_DISCARD;	/* ���� ü�� ����� ���� */
	d3dpp.EnableAutoDepthStencil = TRUE;					
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;				


	/* �׷��� ī�带 �߻�ȭ�� ����̽� ��ü�� �����մϴ�. */
	auto createResult = mD3DInfo->D3D->CreateDevice
	(
		  D3DADAPTER_DEFAULT		       /* ����Ʈ ����͸� ������� */
		, D3DDEVTYPE_HAL
		, mhWnd
		, D3DCREATE_MIXED_VERTEXPROCESSING /* Vertex ó�� ���� */
		, &d3dpp
		, &mD3DInfo->D3D_Device
	);

	/* ����̽� ��ü ���� ���� �� */
	if (FAILED(createResult))
	{
		createResult = mD3DInfo->D3D->CreateDevice
		(
			  D3DADAPTER_DEFAULT
			, D3DDEVTYPE_HAL
			, mhWnd
			, D3DCREATE_SOFTWARE_VERTEXPROCESSING	/* Vertex ó�� ���� */
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

	/* �޼��� ������ �����մϴ�. */
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			/* ���� ���� */
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
		 ���� ������ + DX ����
		***********************
	*/

	Destory();

	/* DirectX�� ���õ� ��� ��ü���� COM ��ü�� ��ӹޱ� ������ Release()�� ���ؼ��� �����մϴ�. */
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
		���� ������ ����
		****************
	*/

	return 0;
}

void SimpleDevice::Destory()
{
	/*
		****************
		���� ������ ����
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
		��� ������ ����� ���� ������ ���� ����̽��� �����մϴ�.

		1. ȭ���� Clear() �Լ��� �ʱ�ȭ
		2. ����� �ĸ� ����(Back Face Buffer)�� �׸��� ���� ����̽��� BeginScene() �Լ��� ȣ��
		3. ����� �׸�
		4. EndScene() �Լ� ȣ���� ���� �ϼ��� �˸�
		5. Present() �Լ��� ���� ���� ����(Front Face Buffer)�� �ĸ� ���۸� ��ü
		6. 1~5 �ܰ踦 �ݺ�
	*/

	mD3DInfo->D3D_Device->Clear
	(
		  0
		, NULL
		, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER   /* ������ۿ� ���̹��۸� ä�� */
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
		  �̰��� ������ �۾��� �����մϴ�.
		*********************************
	*/

	mD3DInfo->D3D_Device->EndScene();

	/* ���� ����� �ȼ� �����Ͱ� ����� �ĸ� ���۸� ���� ����� ��� ���� ���ۿ� ��ü�մϴ�. */
	mD3DInfo->D3D_Device->Present(0, 0, 0, 0);  

	return 0;
}