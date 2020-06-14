/*
	2D Game programming #1

	������ ����� ���α׷� �ڵ� �帧

	1. main()
	2. ���α׷� �ڵ� ���		
	3. ������ Ŭ���� ���		
	4. ������ ����			
	5. Run (Do something)	
	6. ������ �Ҹ�			
	7. ������ Ŭ���� ����
*/


/* ���� �⺻���� ������ ���� �ڵ� */

#include "SimpleWindow.h"

using namespace std;

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, INT)
{
	SimpleWindow win1(L"firstWindow", hInstance);
	
	if (FAILED(win1.Create()))
	{
		return -1;
	}
	
	return win1.Run();
}
