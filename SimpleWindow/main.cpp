/*
	2D Game programming #1

	윈도우 기반의 프로그램 코드 흐름

	1. main()
	2. 프로그램 핸들 얻기		
	3. 윈도우 클래스 등록		
	4. 윈도우 생성			
	5. Run (Do something)	
	6. 윈도우 소멸			
	7. 윈도우 클래스 해제
*/


/* 가장 기본적인 윈도우 생성 코드 */

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
