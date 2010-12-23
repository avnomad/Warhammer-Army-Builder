#include <iostream>
using std::cout;
using std::cin;
using std::endl;
using std::cerr;
using std::clog;
using std::left;
using std::right;

#include <iomanip>
using std::setw;
using std::setprecision;

#include <fstream>
using std::ifstream;
using std::ofstream;
using std::fstream;
using std::ios;

#include <cstdlib>
using std::system;
using std::exit;

#include <sstream>
using std::ostringstream;
using std::istringstream;
using std::stringstream;

#include <string>
using std::string;
using std::getline;

#include <windows.h>

#define length(A) (sizeof(A)/sizeof((A)[0]))


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

enum ChildIDs {UNIT_LIST_ID};

HRESULT CALLBACK armyBuilderProc(HWND window, UINT message, WPARAM argW, LPARAM argL)
{
	static INT tab_stops[] = {120};
	HWND unitList;
	ifstream in;
	string name;
	unsigned int points;
	ostringstream entry;

	switch(message)
	{
	case WM_CREATE:
		SetClassLong(window,0,GetClassLong(window,0) + 1);	// increase the window count by 1

		unitList = CreateWindow(TEXT("listbox"),TEXT("TEST"),WS_CHILD|WS_VISIBLE|LBS_NOTIFY|LBS_SORT|WS_BORDER|WS_VSCROLL|LBS_USETABSTOPS,
						60,40,290,400,window,(HMENU)UNIT_LIST_ID,(HINSTANCE)GetWindowLong(window,GWL_HINSTANCE),NULL);

		in.exceptions(std::ios_base::failbit);
		in.open(TEXT("units.txt"));

		SendMessage(unitList,LB_SETTABSTOPS,(WPARAM)length(tab_stops),(LPARAM)tab_stops);
		while(in)
		{
			getline(in,name,'\t') >> points >> std::ws;	// skip newline
			entry << name << '\t' << points;
			SendMessage(unitList,LB_ADDSTRING,0,(LPARAM)entry.str().c_str());
			entry.str("");
		} // end while

		in.close();

		return 0;
	case WM_DESTROY:
		if(SetClassLong(window,0,GetClassLong(window,0) - 1) <= 1)	// decrease the window count by 1
			PostQuitMessage(0);	// if this was the last window, quit.
		return 0;
	} // end switch

	return DefWindowProc(window,message,argW,argL);
} // end function armyBuilderProc
