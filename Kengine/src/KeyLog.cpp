#include "KeyLog.h"

HHOOK keyboardHook;
HHOOK mouseHook;
KeyLogger Logger;
EventSender Sender;
DWORD initTime;

DWORD firstTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

VOID EventSender::keyEvent(WORD vkCode, bool isUp) {
	INPUT ki;
	ZeroMemory(&ki, sizeof(ki));

	ki.type = INPUT_KEYBOARD;
	ki.ki.wVk = vkCode;

	ki.ki.dwFlags = KEYEVENTF_EXTENDEDKEY;

	if(isUp)
	ki.ki.dwFlags |= KEYEVENTF_KEYUP;

	SendInput(1, &ki, sizeof(INPUT));
}

VOID EventSender::mouseEvent(const MouseCoord& coords, enum MouseEvent e)
{
	INPUT mi;
	ZeroMemory(&mi, sizeof(mi));

	mi.type = INPUT_MOUSE;
	mi.mi.dwFlags |= MOUSEEVENTF_VIRTUALDESK | MOUSEEVENTF_ABSOLUTE;

	if (coords.type == MOUSE_ABSOLUTE)
	{
		mi.mi.dx = coords.x * (0xFFFF / GetSystemMetrics(SM_CXSCREEN));
		mi.mi.dy = coords.y * (0xFFFF / GetSystemMetrics(SM_CYSCREEN));
	}else if (coords.type == RELATIVE)
	{
		POINT p;
		GetCursorPos(&p);

		mi.mi.dx = (p.x + coords.x) * (0xFFFF / GetSystemMetrics(SM_CXSCREEN));
		mi.mi.dy = (p.y + coords.y) * (0xFFFF / GetSystemMetrics(SM_CYSCREEN));
	}
	else if (coords.type == MOUSE_OFFSET)
	{
		//LPRECT rect;
		//ZeroMemory(&rect, sizeof(rect));
		//
		//BOOL windowrect = GetWindowRect(GetForegroundWindow(), rect);
		//
		//mi.mi.dx = (rect->top + coords.x) * (0xFFFF / GetSystemMetrics(SM_CXSCREEN));
		//mi.mi.dy = (rect->left + coords. (0xFFFF / GetSystemMetrics(SM_CYSCREEN));y) *
	}

	if (coords.type != MOUSE_IGNORE) {
		mi.mi.dwFlags |= MOUSEEVENTF_MOVE;
	}

	switch (e) {
		case MOVE: mi.mi.dwFlags |= MOUSEEVENTF_MOVE;break;
		case HWHEEL_RIGHT:mi.mi.dwFlags |= MOUSEEVENTF_HWHEEL;
			mi.mi.mouseData = -WHEEL_DELTA;
			break;
		case HWHEEL_LEFT:mi.mi.dwFlags |= MOUSEEVENTF_HWHEEL;
			mi.mi.mouseData = WHEEL_DELTA;
			break;
		case WHEEL_DOWN:mi.mi.dwFlags |= MOUSEEVENTF_WHEEL;
			mi.mi.mouseData = -WHEEL_DELTA;
			break;
		case WHEEL_UP:mi.mi.dwFlags |= MOUSEEVENTF_WHEEL;
			mi.mi.mouseData = WHEEL_DELTA;
			break;
		case LEFT_CLICK: mi.mi.dwFlags |= MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP; break;
		case LEFT_DOWN:mi.mi.dwFlags |= MOUSEEVENTF_LEFTDOWN; break;
		case LEFT_UP:mi.mi.dwFlags |= MOUSEEVENTF_LEFTUP; break;
		case RIGHT_CLICK: mi.mi.dwFlags |= MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP; break;
		case RIGHT_DOWN:mi.mi.dwFlags |= MOUSEEVENTF_RIGHTDOWN; break;
		case RIGHT_UP:mi.mi.dwFlags |= MOUSEEVENTF_RIGHTUP; break;
		case MIDDLE_CLICK:mi.mi.dwFlags |= MOUSEEVENTF_MIDDLEUP | MOUSEEVENTF_MIDDLEDOWN; break;
		case MIDDLE_DOWN:mi.mi.dwFlags |= MOUSEEVENTF_MIDDLEDOWN; break;
		case MIDDLE_UP:mi.mi.dwFlags |= MOUSEEVENTF_MIDDLEUP; break;
		case X1_DOWN:mi.mi.dwFlags |= MOUSEEVENTF_XDOWN;
			mi.mi.mouseData |= XBUTTON1;
			break;
		case X1_UP:mi.mi.dwFlags |= MOUSEEVENTF_XUP;
			mi.mi.mouseData |= XBUTTON1;
			break;
		case X2_DOWN:mi.mi.dwFlags |= MOUSEEVENTF_XDOWN;
			mi.mi.mouseData |= XBUTTON2;
			break;
		case X2_UP:mi.mi.dwFlags |= MOUSEEVENTF_XUP;
			mi.mi.mouseData |= XBUTTON2;
			break;
	}
	//x1 is backward
	//x2 is forward

	SendInput(1, &mi, sizeof(INPUT));
}

VOID EventSender::sendKeys()
{
	std::cout << "Playing keys...\n";
	std::vector<INPUT>& vec = Logger.getKeyFrames();

	for (size_t i = 0; i < vec.size(); ++i) {
		DWORD time = 0;

		if (vec[i].type == INPUT_MOUSE)
		{
			Sleep(vec[i].mi.time);
			time = vec[i].mi.time;
			vec[i].mi.time = 0;
			SendInput(1, &vec[i], sizeof(INPUT));
			vec[i].mi.time = time;
		}
		else if (vec[i].type == INPUT_KEYBOARD)
		{
			Sleep(vec[i].ki.time);
			time = vec[i].ki.time;
			vec[i].ki.time = 0;
			SendInput(1, &vec[i], sizeof(INPUT));
			vec[i].ki.time = time;
		}
	}
}
//function only works when outer applications aren't running at administer level rights
//more read at https://docs.microsoft.com/en-us/windows/desktop/api/winuser/nf-winuser-sendinput

VOID KeyLogger::record()
{
	std::cout << "Creating Hook" << '\n';
	keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, NULL);
	mouseHook = SetWindowsHookEx(WH_MOUSE_LL, mouseProc, NULL, NULL);
	//creating hooks can cause lag

	std::cout << "Recording started" << '\n';
	MSG msg{ 0 };

	while (GetMessage(&msg, NULL, 0, 0) != 0);
	std::cout << "Unhooked...\n\n";
}

std::vector<INPUT>& KeyLogger::getKeyFrames()
{
	return keyframes;
}

#ifdef DEBUG
VOID KeyLogger::printFrames() const{
	std::cout << "Printing info...\n";
	for (size_t i = 0; i < keyframes.size(); ++i) {
		if (keyframes[i].type == INPUT_MOUSE) {
			std::cout << "Delay: " << keyframes[i].mi.time << '\n';

			DWORD flags = keyframes[i].mi.dwFlags;
			flags &= ~(MOUSEEVENTF_VIRTUALDESK | MOUSEEVENTF_ABSOLUTE);

			switch (flags)
			{
			case MOUSEEVENTF_LEFTDOWN:
				printf_s("LEFT CLICK DOWN\n");
				break;
			case MOUSEEVENTF_LEFTUP:
				printf_s("LEFT CLICK UP\n");
				break;
			case MOUSEEVENTF_MOVE:
				printf_s("MOUSE MOVED\n");
				break;
			case MOUSEEVENTF_MIDDLEDOWN:
				printf_s("MIDDLE BUTTON DOWN\n");
				break;
			case MOUSEEVENTF_MIDDLEUP:
				printf_s("MIDDLE BUTTON UP\n");
				break;
			case MOUSEEVENTF_WHEEL:
				printf_s("VERTICAL WHEEL CHANGE\n");
				break;
			case MOUSEEVENTF_HWHEEL:
				printf_s("HORIZONTAL WHEEL CHANGE\n");
				//horizontal wheel not working atm
				break;
			case MOUSEEVENTF_RIGHTDOWN:
				printf_s("RIGHT CLICK DOWN\n");
				break;
			case MOUSEEVENTF_RIGHTUP:
				printf_s("RIGHT CLICK UP\n");
				break;
			case MOUSEEVENTF_XDOWN:
				printf_s("XBUTTON DOWN\n");
				break;
			case MOUSEEVENTF_XUP:
				printf_s("XBUTTON UP\n");
				break;
			}
			std::cout << "Y: " << keyframes[i].mi.dy / (0xFFFF / GetSystemMetrics(SM_CXSCREEN)) << '\n';
			std::cout << "X: " << keyframes[i].mi.dx / (0xFFFF / GetSystemMetrics(SM_CYSCREEN)) << '\n';
		}
		else if (keyframes[i].type == INPUT_KEYBOARD) {
			std::cout << "Delay: " << keyframes[i].ki.time << '\n';
			std::cout << Logger.codes.at(keyframes[i].ki.wVk) << " " << (keyframes[i].ki.dwFlags== KEYEVENTF_KEYUP ? "KEYUP" : "KEYDOWN") << "\n\n";
		}
	}
}
#endif

LRESULT CALLBACK mouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == HC_ACTION) {
		MSLLHOOKSTRUCT * mouse = (MSLLHOOKSTRUCT *)lParam;
		//mouse event

		std::vector<INPUT>& vec = Logger.getKeyFrames();

		if (vec.size() == 0)
		{
			firstTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - firstTime;
			std::cout<<"FirstTime: " << firstTime << std::endl;
		}
		INPUT ip;
		ZeroMemory(&ip, sizeof(ip));

		ip.type = INPUT_MOUSE;

		ip.mi.dx = mouse->pt.x * (0xFFFF / GetSystemMetrics(SM_CXSCREEN));
		ip.mi.dy = mouse->pt.y * (0xFFFF / GetSystemMetrics(SM_CYSCREEN));
		
		ip.mi.time = mouse->time - initTime;
		initTime = mouse->time;

		ip.mi.mouseData = (mouse->mouseData >> 16);
		ip.mi.dwFlags = MOUSEEVENTF_VIRTUALDESK | MOUSEEVENTF_ABSOLUTE;
		//these flags will be set later in the switch statement

		switch (wParam) {
			case WM_LBUTTONDOWN:
				ip.mi.dwFlags |= MOUSEEVENTF_LEFTDOWN;
				break;
			case WM_LBUTTONUP:
				ip.mi.dwFlags |= MOUSEEVENTF_LEFTUP;
				break;
			case WM_MOUSEMOVE:
				ip.mi.dwFlags |= MOUSEEVENTF_MOVE;
				break;
			case WM_MBUTTONDOWN:
				ip.mi.dwFlags |= MOUSEEVENTF_MIDDLEDOWN;
				break;
			case WM_MBUTTONUP:
				ip.mi.dwFlags |= MOUSEEVENTF_MIDDLEUP;
				break;
			case WM_MOUSEWHEEL:
				ip.mi.dwFlags |= MOUSEEVENTF_WHEEL;

				if (ip.mi.mouseData != WHEEL_DELTA)
					ip.mi.mouseData |= 0xFFFF0000;
				//makes sure that scrolling down works properly
				break;
			case WM_MOUSEHWHEEL:
				//callback function not calling for horizontal wheel for some reason
				ip.mi.dwFlags |= MOUSEEVENTF_HWHEEL;
				break;
			case WM_RBUTTONDOWN:
				ip.mi.dwFlags |= MOUSEEVENTF_RIGHTDOWN;
				break;
			case WM_RBUTTONUP:
				ip.mi.dwFlags |= MOUSEEVENTF_RIGHTUP;
				break;
			case WM_XBUTTONDOWN:
				ip.mi.dwFlags |= MOUSEEVENTF_XDOWN;
				break;
			case WM_XBUTTONUP:
				ip.mi.dwFlags |= MOUSEEVENTF_XUP;
				break;
		}
		vec.push_back(ip);
	}
	//mouse events
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (wParam == WM_SYSKEYUP || wParam == WM_SYSKEYDOWN || wParam == WM_KEYUP || wParam == WM_KEYDOWN && nCode == HC_ACTION)
	{
		PKBDLLHOOKSTRUCT key = (PKBDLLHOOKSTRUCT)lParam;
		//a key was pressed

		std::vector<INPUT>& vec = Logger.getKeyFrames();

		if (vec.size() == 0)
		{
			firstTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - firstTime;
		}
		INPUT ip;
		ZeroMemory(&ip, sizeof(ip));
		ip.type = INPUT_KEYBOARD;
		ip.ki.time = key->time - initTime;
		initTime = key->time;
		ip.ki.dwExtraInfo = 0;
		ip.ki.wScan = 0;
		ip.ki.wVk = key->vkCode;
#ifdef DEBUG
		std::cout << Logger.codes.at(ip.ki.wVk) << std::endl;
#endif
		ip.ki.dwFlags = 0x0000;

		if (key->flags & 0x1)
			ip.ki.dwFlags |= KEYEVENTF_EXTENDEDKEY;
		if(key->flags >> 7)
			ip.ki.dwFlags |= KEYEVENTF_KEYUP;
		
		vec.push_back(ip);

		if (vec.size() > 1 && (vec[vec.size() - 2].ki.wVk == VK_LCONTROL && key->vkCode == 'Q')) 
		{
			std::cout << "Recording finished" << "\n\n";
			PostQuitMessage(0);
			vec.pop_back();
			vec.pop_back();
			if (vec[0].type == INPUT_MOUSE)
				vec[0].mi.time = firstTime;
			else if (vec[0].type == INPUT_KEYBOARD)
				vec[0].ki.time = firstTime;

			return UnhookWindowsHookEx(mouseHook) | UnhookWindowsHookEx(keyboardHook);
		}
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

MouseCoord::MouseCoord(TYPE type, LONG x, LONG y) : type(type), x(x), y(y)
{

}