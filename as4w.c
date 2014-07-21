/*

Appearance sounds for windows, made using accesibility events.

Due to the absence of some events, you'll need to customize windows sounds with
applaunch, appquit, restore up/maximize, restore down/minimize, menuselect sounds.

References
http://msdn.microsoft.com/en-us/library/windows/desktop/dd318066%28v=vs.85%29.aspx
http://www.winprog.org/tutorial/
http://stackoverflow.com/questions/12931235/setwineventhook-does-not-catch-any-event
http://stackoverflow.com/questions/1565439/how-to-playsound-in-c-using-windows-api
http://stackoverflow.com/questions/298510/how-to-get-the-current-directory-in-a-c-program


*/



#pragma comment(lib, "winmm.lib")
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
//#include <mmsystem.h>

char currentfilepath[128];
char dragsoundpath[128];
//char menuitemoppath[128];
char menuitemhipath[128];
//char menuitemsepath[128];
char receivedroppath[128];
char appshowpath[128];
char apphidepath[128];

const char g_szClassName[] = "myWindowClass";



HWINEVENTHOOK g_hook; // Global variable.

DWORD prev_event;
HWND prev_hwnd;

// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, DWORD event, WPARAM wParam, LPARAM lParam)
{
	switch (event)
	{
		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hwnd, event, wParam, lParam);
	}
	return 0;
}



// Prototypes
void CALLBACK HandleWinEvent(HWINEVENTHOOK, DWORD, HWND, LONG, LONG, DWORD, DWORD);
void InitializePaths();

// Initializes COM and sets up the event hook.
void InitializeMSAA()
{
	CoInitialize(NULL);
	g_hook = SetWinEventHook(
		EVENT_MIN, EVENT_MAX,  // Range of events .
		NULL,                                          // Handle to DLL.
		HandleWinEvent,                                // The callback.
		0, 0,              // Process and thread IDs of interest (0 = all)
		WINEVENT_OUTOFCONTEXT); // Flags.
}

// Unhooks the event and shuts down COM.
void ShutdownMSAA()
{
	UnhookWinEvent(g_hook);
	CoUninitialize();
}


void InitializePaths(){

	_getcwd(dragsoundpath, 128);
	//_getcwd(menuitemoppath, 128);
	_getcwd(menuitemhipath, 128);
	_getcwd(receivedroppath, 128);
	_getcwd(appshowpath, 128);
	_getcwd(apphidepath, 128);

	strcat(dragsoundpath, "\\sounds\\DragSoundDragging.wav\0");
	//strcat(menuitemoppath, "\\sounds\\ApplicationLaunched.wav\0");
	strcat(menuitemhipath, "\\sounds\\MenuItemHilite.wav\0");
	strcat(receivedroppath, "\\sounds\\ReceiveDrop.wav\0");
	strcat(appshowpath, "\\sounds\\ApplicationShow.wav\0");
	strcat(apphidepath, "\\sounds\\ApplicationHide.wav\0");


	return;

}



 //Callback function that handles events.
void CALLBACK HandleWinEvent(HWINEVENTHOOK hook, DWORD event, HWND hwnd,
	LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime)
{

	//Windows doesn't distingush between a window move and a resize (but MacOS does)
	if (event == EVENT_OBJECT_DRAGSTART || event == EVENT_SYSTEM_SCROLLINGSTART || 
		event == EVENT_SYSTEM_MOVESIZESTART || event == EVENT_SYSTEM_DRAGDROPSTART)
	{
		PlaySound((LPCSTR) dragsoundpath , NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	}

	else if (event == EVENT_SYSTEM_SCROLLINGEND || event == EVENT_SYSTEM_MOVESIZEEND ||
			event == EVENT_SYSTEM_DRAGDROPEND)
	{
		PlaySound((LPCSTR)NULL, NULL, NULL);
	}

	else if ( 
				(event == EVENT_OBJECT_FOCUS  && prev_event != EVENT_OBJECT_SELECTION
				&& event != prev_event && prev_event != EVENT_SYSTEM_MENUSTART
				)
				//|| 
				//(
				//event == EVENT_OBJECT_SELECTION &&prev_event != EVENT_OBJECT_FOCUS
				//&& event != prev_event && prev_event != EVENT_SYSTEM_MENUSTART
				//)
		
		
		) //play pop sound only if it was not the previous event. intended to stop mylti pops from apps with many windows. TODO: better solution: stop pops if many focus events together
	{
		//PlaySound((LPCSTR) "C:\\Users\\Jun-Hong\\Music\\UI Sounds\\ApplicationShow.wav", NULL, SND_FILENAME | SND_ASYNC);
		PlaySound((LPCSTR) menuitemhipath , NULL, SND_FILENAME | SND_ASYNC);
	}



	//else if (event == EVENT_SYSTEM_FOREGROUND && EVENT_SYSTEM_MINIMIZEEND != prev_event  && event != prev_event)
	//{
	//	PlaySound((LPCSTR) "C:\\Users\\Jun-Hong\\Music\\UI Sounds\\ApplicationShow.wav", NULL, SND_FILENAME | SND_ASYNC);
	//}

	else if (event == EVENT_OBJECT_INVOKED  )
	{
		PlaySound((LPCSTR) receivedroppath, NULL, SND_FILENAME | SND_ASYNC);
	}

	//extra?

	//else if (event == EVENT_SYSTEM_DIALOGSTART)
	//{
	//	PlaySound((LPCSTR) "C:\\Users\\Jun-Hong\\Music\\UI Sounds\\ReceiveDrop.wav", NULL, SND_FILENAME | SND_ASYNC);
	//}

	//else if (event == EVENT_SYSTEM_DIALOGEND)
	//{
	//	PlaySound((LPCSTR) "C:\\Users\\Jun-Hong\\Music\\UI Sounds\\ReceiveDrop.wav", NULL, SND_FILENAME | SND_ASYNC);
	//}

	//else if (event == EVENT_SYSTEM_MENUPOPUPSTART)
	//{
	//	PlaySound((LPCSTR) "C:\\Users\\Jun-Hong\\Music\\UI Sounds\\ReceiveDrop.wav", NULL, SND_FILENAME | SND_ASYNC);
	//}

	//else if (event == EVENT_SYSTEM_MENUPOPUPEND)
	//{
	//	PlaySound((LPCSTR) "C:\\Users\\Jun-Hong\\Music\\UI Sounds\\ReceiveDrop.wav", NULL, SND_FILENAME | SND_ASYNC);
	//}

	/*else if (event == EVENT_SYSTEM_MENUSTART)
	{
		PlaySound((LPCSTR) menuitemoppath, NULL, SND_FILENAME | SND_ASYNC);
	}*/

	//else if (event == EVENT_SYSTEM_MENUEND)
	//{
	//	PlaySound((LPCSTR) menuitemsepath, NULL, SND_FILENAME | SND_ASYNC);
	//}


	else if (event == EVENT_SYSTEM_SWITCHSTART)
	{
		PlaySound((LPCSTR) appshowpath, NULL, SND_FILENAME | SND_ASYNC);
	}

	else if (event == EVENT_SYSTEM_SWITCHEND)
	{
		PlaySound((LPCSTR) apphidepath , NULL, SND_FILENAME | SND_ASYNC);
	}

	

	prev_hwnd = hwnd;
	prev_event = event;

	return;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;

	

	//Step 1: Registering the Window Class
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = (WNDPROC)WndProc;//C++ compat
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = g_szClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Window Registration Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	InitializeMSAA();
	InitializePaths();


	// Step 2: Creating the Window
	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		g_szClassName,
		"Appearance Sounds 4 Windows",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 240, 120,
		NULL, NULL, hInstance, NULL);

	if (hwnd == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	

	// Step 3: The Message Loop
	while (GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	ShutdownMSAA();

	return Msg.wParam;
}
