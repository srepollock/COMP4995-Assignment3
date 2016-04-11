#include "Includes.h"
#include "Engine.h"

//Game g;
Engine e;

long CALLBACK WndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam) {
	D3DLOCKED_RECT rect;
	DWORD* pData;
	short delta;
	switch (uMessage) {
	case WM_CREATE:
	{
		return 0;
	}
	case WM_PAINT:
	{
		ValidateRect(hWnd, NULL);//basically saying - yeah we took care of any paint msg without any overhead
		return 0;
	}
	case WM_CHAR: {
		if (wParam == VK_ESCAPE)
		{
			SendMessage(hWnd, WM_DESTROY, NULL, NULL);
		}
		return 0;
	}
	case WM_MOUSEWHEEL:
		
		break;
	case WM_KEYDOWN:
		switch (wParam) {
		case 0x30:
			// 0
			// move camera
			// set move camera bool
			e.setSelected(-1);
			break;
		case 0x31:
			// 1
			e.setSelected(0);
			break;
		case 0x32:
			// 2
			e.setSelected(1);
			break;
		case 0x33:
			// 3
			e.setSelected(2);
			break;
			/*
			// moves current selection
				case 0x57:
					// w // move up
					if (e.getCameraMove()) {
						e.moveCamera(0.0f, 1.0f, 0.0f);
					}
					else if (e.getObj1Move()) {
						e.moveObject(1, 0.0f, 1.0f, 0.0f);
					}
					else if (e.getObj2Move()) {
						e.moveObject(2, 0.0f, 1.0f, 0.0f);
					}
					else if (e.getObj3Move()) {
						e.moveObject(3, 0.0f, 1.0f, 0.0f);
					}
					else if (e.getObj4Move()) {
						e.moveObject(4, 0.0f, 1.0f, 0.0f);
					}
					break;
				case 0x41:
					// a // move left
					if (e.getCameraMove()) {
						e.moveCamera(-1.0f, 0.0f, 0.0f);
					}
					else if (e.getObj1Move()) {
						e.moveObject(1, -1.0f, 0.0f, 0.0f);
					}
					else if (e.getObj2Move()) {
						e.moveObject(2, -1.0f, 0.0f, 0.0f);
					}
					else if (e.getObj3Move()) {
						e.moveObject(3, -1.0f, 0.0f, 0.0f);
					}
					else if (e.getObj4Move()) {
						e.moveObject(4, -1.0f, 0.0f, 0.0f);
					}
					break;
				case 0x53:
					// s // move down
					if (e.getCameraMove()) {
						e.moveCamera(0.0f, -1.0f, 0.0f);
					}
					else if (e.getObj1Move()) {
						e.moveObject(1, 0.0f, -1.0f, 0.0f);
					}
					else if (e.getObj2Move()) {
						e.moveObject(2, 0.0f, -1.0f, 0.0f);
					}
					else if (e.getObj3Move()) {
						e.moveObject(3, 0.0f, -1.0f, 0.0f);
					}
					else if (e.getObj4Move()) {
						e.moveObject(4, 0.0f, -1.0f, 0.0f);
					}
					break;
				case 0x44:
					// d // move right
					if (e.getCameraMove()) {
						e.moveCamera(1.0f, 0.0f, 0.0f);
					}
					else if (e.getObj1Move()) {
						e.moveObject(1, 1.0f, 0.0f, 0.0f);
					}
					else if (e.getObj2Move()) {
						e.moveObject(2, 1.0f, 0.0f, 0.0f);
					}
					else if (e.getObj3Move()) {
						e.moveObject(3, 1.0f, 0.0f, 0.0f);
					}
					else if (e.getObj4Move()) {
						e.moveObject(4, 1.0f, 0.0f, 0.0f);
					}
					break;
				case 0x58:
					// x // move farther
					if (e.getCameraMove()) {
						e.moveCamera(0.0f, 0.0f, 1.0f);
					}
					else if (e.getObj1Move()) {
						e.moveObject(1, 0.0f, 0.0f, 1.0f);
					}
					else if (e.getObj2Move()) {
						e.moveObject(2, 0.0f, 0.0f, 1.0f);
					}
					else if (e.getObj3Move()) {
						e.moveObject(3, 0.0f, 0.0f, 1.0f);
					}
					else if (e.getObj4Move()) {
						e.moveObject(4, 0.0f, 0.0f, 1.0f);
					}
					break;
				case 0x5A:
					// z // move closer
					if (e.getCameraMove()) {
						e.moveCamera(0.0f, 0.0f, -1.0f);
					}
					else if (e.getObj1Move()) {
						e.moveObject(1, 0.0f, 0.0f, -1.0f);
					}
					else if (e.getObj2Move()) {
						e.moveObject(2, 0.0f, 0.0f, -1.0f);
					}
					else if (e.getObj3Move()) {
						e.moveObject(3, 0.0f, 0.0f, -1.0f);
					}
					else if (e.getObj4Move()) {
						e.moveObject(4, 0.0f, 0.0f, -1.0f);
					}
					break;
				case 0x51:
					// q // rotate left
					if (e.getCameraMove()) {
						e.rotateCameraY(-0.3f);
					}
					else if (e.getObj1Move()) {
						e.rotateObjectY(1, -0.3f);
					}
					else if (e.getObj2Move()) {
						e.rotateObjectY(2, -0.3f);
					}
					else if (e.getObj3Move()) {
						e.rotateObjectY(3, -0.3f);
					}
					else if (e.getObj4Move()) {
						e.rotateObjectY(4, -0.3f);
					}
					break;
				case 0x45:
					// e // rotate right
					if (e.getCameraMove()) {
						e.rotateCameraY(0.3f);
					}
					else if (e.getObj1Move()) {
						e.rotateObjectY(1, 0.3f);
					}
					else if (e.getObj2Move()) {
						e.rotateObjectY(2, 0.3f);
					}
					else if (e.getObj3Move()) {
						e.rotateObjectY(3, 0.3f);
					}
					else if (e.getObj4Move()) {
						e.rotateObjectY(4, 0.3f);
					}
					break;
				case 0x50:
					// point lighting enabled
					// p
					e.SetLightingPoint();
					break;
				case 0x4F:
					// directional lighting enabled
					// o
					e.SetLightingDirectional();
					break;
				case 0x49:
					// spot lighting enabled
					// i
					e.SetLightingSpot();
					break;
				case 0x55:
					// ambient lighting enabled
					// u
					e.SetLightingAmbient();
					break;
				case 0x46:
					// f // rotate about z axis ccw
					if (e.getCameraMove()) {
						e.rotateCameraZ(0.3f);
					}
					else if (e.getObj1Move()) {
						e.rotateObjectZ(1, 0.3f);
					}
					else if (e.getObj2Move()) {
						e.rotateObjectZ(2, 0.3f);
					}
					else if (e.getObj3Move()) {
						e.rotateObjectZ(3, 0.3f);
					}
					else if (e.getObj4Move()) {
						e.rotateObjectZ(4, 0.3f);
					}
					break;
				case 0x47:
					// g // rotate about z axis cw
					if (e.getCameraMove()) {
						e.rotateCameraZ(-0.3f);
					}
					else if (e.getObj1Move()) {
						e.rotateObjectZ(1, -0.3f);
					}
					else if (e.getObj2Move()) {
						e.rotateObjectZ(2, -0.3f);
					}
					else if (e.getObj3Move()) {
						e.rotateObjectZ(3, -0.3f);
					}
					else if (e.getObj4Move()) {
						e.rotateObjectZ(4, -0.3f);
					}
					break;
				}*/
		}
		return 0;
	case WM_LBUTTONDOWN: 
		// This is for picking the object

		return 0;
	case WM_MOUSEMOVE:
		if (wParam == MK_LBUTTON) {
			// holding down the left button
		}
		return 0;
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	default:
	{
		return DefWindowProc(hWnd, uMessage, wParam, lParam);
	}
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pstrCmdLine, int iCmdShow) {
	HWND hWnd;
	MSG msg;
	WNDCLASSEX wc;

	static TCHAR strAppName[] = _T("First Windows App, Zen Style");

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_HAND);
	wc.hCursor = LoadCursor(NULL, IDC_CROSS);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = strAppName;

	RegisterClassEx(&wc);

	hWnd = CreateWindowEx(NULL,
		strAppName,
		strAppName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	//g_hWndMain = hWnd;//set our global window handle
	// FRAME HAS NOT BEEN SETUP!!!
	//g = Game(hWnd);
	e = Engine(hWnd);

	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);

	/*
	if (FAILED(g.GameInit())) {
		//initialize Game
		SetError(_T("Initialization Failed"));
		g.GameShutdown();
		return E_FAIL;
	}
	*/

	if (FAILED(e.EngineInit())) {
		//initialize Game
		SetError(_T("Initialization Failed"));
		e.EngineShutdown();
		return E_FAIL;
	}

	while (TRUE) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			//g.GameLoop();
			e.EngineLoop();
		}
	}
	//g.GameShutdown();// clean up the game
	e.EngineShutdown();// clean up the game
	return msg.wParam;
}
