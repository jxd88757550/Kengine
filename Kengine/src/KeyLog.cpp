#include "KeyLog.h"

HHOOK keyboardHook;
HHOOK mouseHook;
KeyLogger Logger;
KeySender Sender;
DWORD initTime;

DWORD firstTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

VOID KeySender::keyDown(WORD vkCode) {
	ip.ki.wVk = vkCode;
	ip.ki.dwFlags = 0;

	SendInput(1, &ip, sizeof(INPUT));
}

VOID KeySender::keyUp(WORD vkCode) {
	ip.ki.wVk = vkCode;
	ip.ki.dwFlags = KEYEVENTF_KEYUP;

	SendInput(1, &ip, sizeof(INPUT));
}

VOID KeySender::sendKeys()
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
	//creating hooks cause lag

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
		ip.mi.dwExtraInfo = mouse->dwExtraInfo;
		ip.mi.dwFlags = MOUSEEVENTF_VIRTUALDESK | MOUSEEVENTF_ABSOLUTE;
		//these flags will be set later in the switch statement
		
		switch (wParam) {
			case WM_LBUTTONDOWN:
				printf_s("LEFT CLICK DOWN\n");
				ip.mi.dwFlags |= MOUSEEVENTF_LEFTDOWN;
				break;
			case WM_LBUTTONUP:
				printf_s("LEFT CLICK UP\n");
				ip.mi.dwFlags |= MOUSEEVENTF_LEFTUP;
				break;
			case WM_MOUSEMOVE:
				//printf_s("MOUSE MOVED\n");
				ip.mi.dwFlags |= MOUSEEVENTF_MOVE;
				break;
			case WM_MBUTTONDOWN:
				//printf_s("MOUSE MOVED\n");
				ip.mi.dwFlags |= MOUSEEVENTF_MIDDLEDOWN;
				break;
			case WM_MBUTTONUP:
				//printf_s("MOUSE MOVED\n");
				ip.mi.dwFlags |= MOUSEEVENTF_MIDDLEUP;
				break;
			case WM_MOUSEWHEEL:
				printf_s("VERTICAL WHEEL CHANGE\n");
				ip.mi.dwFlags |= MOUSEEVENTF_WHEEL;
				break;
			case WM_MOUSEHWHEEL:
				printf_s("HORIZONTAL WHEEL CHANGE\n");
				ip.mi.dwFlags |= MOUSEEVENTF_HWHEEL;
				break;
			case WM_RBUTTONDOWN:
				printf_s("RIGHT CLICK DOWN\n");
				ip.mi.dwFlags |= MOUSEEVENTF_RIGHTDOWN;
				break;
			case WM_RBUTTONUP:
				printf_s("RIGHT CLICK UP\n");
				ip.mi.dwFlags |= MOUSEEVENTF_RIGHTUP;
				break;
			case WM_XBUTTONDOWN:
				printf_s("XBUTTON DOWN\n");
				ip.mi.dwFlags |= MOUSEEVENTF_XDOWN;
				break;
			case WM_XBUTTONUP:
				printf_s("XBUTTON UP\n");
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
		
		if (vec.size() > 1 && vec[vec.size() - 2].ki.wVk == VK_LCONTROL && key->vkCode == 'Q' || key->vkCode == VK_ESCAPE) {
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

KeySender::KeySender()
{
	ZeroMemory(&ip, sizeof(ip));
	ip.type = INPUT_KEYBOARD;

	ip.ki.wScan = 0;
	ip.ki.dwFlags = 0;
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;
}