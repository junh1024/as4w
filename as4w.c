/*
http://msdn.microsoft.com/en-us/library/windows/desktop/dd318066%28v=vs.85%29.aspx
http://www.winprog.org/tutorial/
http://stackoverflow.com/questions/12931235/setwineventhook-does-not-catch-any-event
http://stackoverflow.com/questions/1565439/how-to-playsound-in-c-using-windows-api
*/

#pragma comment(lib, "winmm.lib")

#include <windows.h>
//#include <mmsystem.h>

const char g_szClassName[] = "myWindowClass";

DWORD prev_event;

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

// Global variable.
HWINEVENTHOOK g_hook;

// Prototypes
void CALLBACK HandleWinEvent(HWINEVENTHOOK, DWORD, HWND, LONG, LONG, DWORD, DWORD);
void PlayScrollingSound();

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


 //Callback function that handles events.
void CALLBACK HandleWinEvent(HWINEVENTHOOK hook, DWORD event, HWND hwnd,
	LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime)
{
	if (event == EVENT_OBJECT_DRAGSTART || event == EVENT_SYSTEM_SCROLLINGSTART || 
		event == EVENT_SYSTEM_MOVESIZESTART || event == EVENT_SYSTEM_DRAGDROPSTART)
	{
		PlayScrollingSound();
	}

	else if (event == EVENT_SYSTEM_SCROLLINGEND || event == EVENT_SYSTEM_MOVESIZEEND ||
			event == EVENT_SYSTEM_DRAGDROPEND)
	{
		PlaySound((LPCSTR)NULL, NULL, NULL);
	}

	else if (event == EVENT_OBJECT_FOCUS && event!= prev_event ) //play pop sound only if it was not the previous event. intended to stop mylti pops from apps with many windows. TODO: better solution: stop pops if many focus events together
	{
		PlaySound((LPCSTR) "C:\\Users\\Jun-Hong\\Music\\UI Sounds\\ApplicationShow.wav", NULL, SND_FILENAME | SND_ASYNC );
	}

	else
	{
		printf("nothing!");
	}

	prev_event = event;

	return;
}


void PlayScrollingSound()
{
	PlaySound((LPCSTR) "C:\\Users\\Jun-Hong\\Music\\UI Sounds\\DragSoundDragging.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
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
	wc.lpfnWndProc = WndProc;
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
