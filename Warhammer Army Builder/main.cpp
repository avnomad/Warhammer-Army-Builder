#include <iostream>
using std::cout;
using std::cin;
using std::endl;
using std::cerr;
using std::clog;
using std::left;
using std::right;

#include <fstream>
using std::ifstream;
using std::ofstream;
using std::fstream;
using std::ios;

#include <sstream>
using std::ostringstream;
using std::istringstream;
using std::stringstream;

#include <cstdlib>
using std::system;
using std::exit;

#include <string>
using std::string;
using std::getline;

#include <climits>

#include <windows.h>

#define length(A) (sizeof(A)/sizeof((A)[0]))


LRESULT CALLBACK armyBuilderProc(HWND window, UINT message, WPARAM argW, LPARAM argL);

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

	HWND mainWindow = CreateWindow(TEXT("armyBuilder"),TEXT("Army Configuration"),WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN|WS_VSCROLL|WS_HSCROLL,
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

void addSubtrackPoints(HWND label,HWND srcListbox,UINT selection,HWND dstListbox = NULL);

enum ChildIDs {CODEX_LABEL_ID,CODEX_LISTBOX_ID,ARMY_LABEL_ID,ARMY_LISTBOX_ID,ADD_BUTTON_ID,REMOVE_BUTTON_ID,
				LEFT_TOTAL_LABEL_ID,RIGHT_TOTAL_LABEL_ID};

LRESULT CALLBACK armyBuilderProc(HWND window, UINT message, WPARAM argW, LPARAM argL)
{
	static const UINT cxChar = LOWORD(GetDialogBaseUnits()), cyChar = HIWORD(GetDialogBaseUnits());
	static INT tab_stops[] = {120};
	HWND codex_label,codex_listbox,army_label,army_listbox,add_button,remove_button,left_total_label,right_total_label;
	ifstream in;
	string name;

	switch(message)
	{
	case WM_CREATE:
		SetClassLong(window,0,GetClassLong(window,0) + 1);	// increase the window count by 1

		// create child windows
		codex_label = CreateWindow(TEXT("static"),TEXT("Available Units:"),WS_CHILD|WS_VISIBLE|SS_LEFT,
							3*cxChar,2*cyChar,34*cxChar+GetSystemMetrics(SM_CXVSCROLL),cyChar,window,(HMENU)CODEX_LABEL_ID,
							(HINSTANCE)GetWindowLongPtr(window,GWLP_HINSTANCE),NULL);
		codex_listbox = CreateWindow(TEXT("listbox"),NULL,WS_CHILD|WS_VISIBLE|LBS_NOTIFY|WS_BORDER|WS_VSCROLL|LBS_USETABSTOPS,
							3*cxChar,3*cyChar,34*cxChar+GetSystemMetrics(SM_CXVSCROLL),20*cyChar,window,(HMENU)CODEX_LISTBOX_ID,
							(HINSTANCE)GetWindowLongPtr(window,GWLP_HINSTANCE),NULL);
		SendMessage(codex_listbox,LB_SETTABSTOPS,(WPARAM)length(tab_stops),(LPARAM)tab_stops);

		army_label = CreateWindow(TEXT("static"),TEXT("Selected Units:"),WS_CHILD|WS_VISIBLE|SS_LEFT,
							60*cxChar,2*cyChar,34*cxChar+GetSystemMetrics(SM_CXVSCROLL),cyChar,window,(HMENU)ARMY_LABEL_ID,
							(HINSTANCE)GetWindowLongPtr(window,GWLP_HINSTANCE),NULL);
		army_listbox = CreateWindow(TEXT("listbox"),NULL,WS_CHILD|WS_VISIBLE|LBS_NOTIFY|WS_BORDER|WS_VSCROLL|LBS_USETABSTOPS,
							60*cxChar,3*cyChar,34*cxChar+GetSystemMetrics(SM_CXVSCROLL),20*cyChar,window,(HMENU)ARMY_LISTBOX_ID,
							(HINSTANCE)GetWindowLongPtr(window,GWLP_HINSTANCE),NULL);
		SendMessage(army_listbox,LB_SETTABSTOPS,(WPARAM)length(tab_stops),(LPARAM)tab_stops);

		left_total_label = CreateWindow(TEXT("static"),TEXT("Total:"),WS_CHILD|WS_VISIBLE|SS_LEFT,
							60*cxChar,23*cyChar,8*cxChar,cyChar,window,(HMENU)LEFT_TOTAL_LABEL_ID,
							(HINSTANCE)GetWindowLongPtr(window,GWLP_HINSTANCE),NULL);
		
		right_total_label = CreateWindow(TEXT("static"),TEXT("0"),WS_CHILD|WS_VISIBLE|SS_RIGHT,
							68*cxChar,23*cyChar,26*cxChar+GetSystemMetrics(SM_CXVSCROLL),cyChar,window,(HMENU)RIGHT_TOTAL_LABEL_ID,
							(HINSTANCE)GetWindowLongPtr(window,GWLP_HINSTANCE),NULL);

		add_button = CreateWindow(TEXT("button"),TEXT("ADD"),WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
							47*cxChar,9*cyChar,5*cxChar,7*cyChar/4,window,(HMENU)ADD_BUTTON_ID,
							(HINSTANCE)GetWindowLongPtr(window,GWLP_HINSTANCE),NULL);

		remove_button = CreateWindow(TEXT("button"),TEXT("REMOVE"),WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
							46*cxChar,13*cyChar,8*cxChar,7*cyChar/4,window,(HMENU)REMOVE_BUTTON_ID,
							(HINSTANCE)GetWindowLongPtr(window,GWLP_HINSTANCE),NULL);

		// load unit entries
		//in.exceptions(std::ios_base::failbit);
		in.open(TEXT("units.txt"));
		while(in)
		{
			getline(in,name);
			SendMessage(codex_listbox,LB_ADDSTRING,0,(LPARAM)name.c_str());
		} // end while
		in.close();
		SendMessage(codex_listbox,LB_SETCURSEL,0,0);	// why doesn't work?
		return 0;
	case WM_COMMAND:
		switch(LOWORD(argW))
		{
		case CODEX_LISTBOX_ID:
			if(HIWORD(argW) == LBN_DBLCLK)
			{
				UINT selection = SendMessage((HWND)argL,LB_GETCURSEL,0,0);
				if(selection == LB_ERR) break;
				addSubtrackPoints(GetDlgItem(window,RIGHT_TOTAL_LABEL_ID),(HWND)argL,selection,GetDlgItem(window,ARMY_LISTBOX_ID));
			} // end if
			break;
		case ARMY_LISTBOX_ID:
			if(HIWORD(argW) == LBN_DBLCLK)
			{
				UINT selection = SendMessage((HWND)argL,LB_GETCURSEL,0,0);
				if(selection == LB_ERR) break;
				addSubtrackPoints(GetDlgItem(window,RIGHT_TOTAL_LABEL_ID),(HWND)argL,selection);
				SendMessage((HWND)argL,LB_DELETESTRING,selection,0);
			} // end if
			break;
		case ADD_BUTTON_ID:
			if(HIWORD(argW) == BN_CLICKED)
				SendMessage(window,WM_COMMAND,MAKEWPARAM(CODEX_LISTBOX_ID,LBN_DBLCLK),(LPARAM)GetDlgItem(window,CODEX_LISTBOX_ID));
			break;
		case REMOVE_BUTTON_ID:
			if(HIWORD(argW) == BN_CLICKED)
				SendMessage(window,WM_COMMAND,MAKEWPARAM(ARMY_LISTBOX_ID,LBN_DBLCLK),(LPARAM)GetDlgItem(window,ARMY_LISTBOX_ID));
			break;
		} // end switch
		return 0;
	case WM_DESTROY:
		if(SetClassLong(window,0,GetClassLong(window,0) - 1) <= 1)	// decrease the window count by 1
			PostQuitMessage(0);	// if this was the last window, quit.
		return 0;
	} // end switch

	return DefWindowProc(window,message,argW,argL);
} // end function armyBuilderProc


void addSubtrackPoints(HWND label,HWND srcListbox,UINT selection,HWND dstListbox)	// that's how code should NOT be written!
{
	// get string from listbox
	UINT stringLength = SendMessage(srcListbox,LB_GETTEXTLEN,selection,0);
	TCHAR *buffer = new TCHAR[stringLength+1];
	SendMessage(srcListbox,LB_GETTEXT,selection,(LPARAM)buffer);
	if(dstListbox)
		SendMessage(dstListbox,LB_ADDSTRING,0,(LPARAM)buffer);
	// parse string to get unit points
	int unit_points;
	istringstream(string(buffer)).ignore(UINT_MAX,'\t') >> unit_points;
	delete[] buffer;
	// get string from label
	stringLength = GetWindowTextLength(label);
	buffer = new TCHAR[stringLength+1];
	GetWindowText(label,buffer,stringLength+1);
	// parse string to get total points
	int total_points;
	istringstream(string(buffer)) >> total_points;
	delete[] buffer;
	if(dstListbox)
		total_points += unit_points;
	else
		total_points -= unit_points;
	SetWindowText(label,((ostringstream&)(ostringstream() << total_points)).str().c_str());
} // end function addSubtrackPoints
