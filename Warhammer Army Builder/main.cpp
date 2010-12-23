#include <iostream>
using std::cout;
using std::cin;
using std::endl;
using std::cerr;
using std::clog;
using std::left;

#include <cstdlib>
using std::system;
using std::exit;

#include <sstream>
using std::ostringstream;
using std::istringstream;
using std::stringstream;

#include <windows.h>



HRESULT CALLBACK armyBuilderProc(HWND window, UINT message, WPARAM argW, LPARAM argL);

int main()
{
	WNDCLASS armyBuilder;

	armyBuilder.style = CS_VREDRAW | CS_HREDRAW;
	armyBuilder.lpfnWndProc = armyBuilderProc;
	armyBuilder.cbClsExtra = sizeof(LONG);	// The system initializes the bytes to zero. 
	armyBuilder.cbWndExtra = 0;
	armyBuilder.hInstance = GetModuleHandle(nullptr);
	armyBuilder.hIcon = LoadIcon(NULL,IDI_APPLICATION);
	armyBuilder.hCursor = LoadCursor(NULL,IDC_ARROW);
	armyBuilder.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	armyBuilder.lpszMenuName = nullptr;
	armyBuilder.lpszClassName = TEXT("armyBuilder");

	RegisterClass(&armyBuilder);

	HWND mainWindow = CreateWindow(TEXT("armyBuilder"),TEXT("Army Configuration"),WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN,
							CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,NULL,NULL,GetModuleHandle(nullptr),NULL);

	ShowWindow(mainWindow,SW_SHOWNORMAL);
	UpdateWindow(mainWindow);


	MSG message;
	while(GetMessage(&message,NULL,0,0))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	} // end while

	//system("pause");
	return message.wParam;
} // end function main


HRESULT CALLBACK armyBuilderProc(HWND window, UINT message, WPARAM argW, LPARAM argL)
{
	switch(message)
	{
	case WM_CREATE:
		SetClassLong(window,0,GetClassLong(window,0) + 1);	// increase the window count by 1
		return 0;
	case WM_DESTROY:
		if(SetClassLong(window,0,GetClassLong(window,0) - 1) <= 1)	// decrease the window count by 1
			PostQuitMessage(0);	// if this was the last window, quit.
		return 0;
	} // end switch

	return DefWindowProc(window,message,argW,argL);
} // end function armyBuilderProc
