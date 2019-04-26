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

	//SendInput(vec.size(), vec.data(), sizeof(INPUT));
	//sendinput not sleeping properly...?
	for (size_t i = 0; i < vec.size(); ++i) {
		Sleep(vec[i].ki.time);
		SendInput(1, &vec[i], sizeof(INPUT));
	}
}
 
VOID KeyLogger::record()
{
	std::cout << "Creating Hook" << '\n';
	keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, NULL);
	//mouseHook = SetWindowsHookEx(WH_MOUSE_LL, mouseProc, NULL, NULL);
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
	std::cout << "Printing keys...\n";
	for (size_t i = 0; i < keyframes.size(); ++i) {
		if (keyframes[i].type == INPUT_MOUSE) {
			std::cout << "Delay: " << keyframes[i].mi.time << '\n';
			std::cout << "Dx: " << keyframes[i].mi.dx << '\n';
			std::cout << "Dy: " << keyframes[i].mi.dy << '\n';
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

}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	PKBDLLHOOKSTRUCT key = (PKBDLLHOOKSTRUCT)lParam;
	//a key was pressed

	if (wParam == WM_LBUTTONDOWN && nCode == HC_ACTION) {
		std::vector<INPUT>& vec = Logger.getKeyFrames();

		INPUT ip;
		ip.type = INPUT_MOUSE;

	}
	//mouse events

	if (wParam == WM_SYSKEYUP || wParam == WM_SYSKEYDOWN || wParam == WM_KEYUP || wParam == WM_KEYDOWN && nCode == HC_ACTION)
	{
		std::vector<INPUT>& vec = Logger.getKeyFrames();

		if (vec.size() == 0)
		{
			firstTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - firstTime;
		}
		INPUT ip;
		ip.type = INPUT_KEYBOARD;
		ip.ki.time = key->time - initTime;
		initTime = key->time;
		ip.ki.dwExtraInfo = 0;
		ip.ki.wScan = 0;
		ip.ki.wVk = key->vkCode;
#ifdef DEBUG
		std::cout << Logger.codes.at(ip.ki.wVk) << std::endl;
#endif
		if (key->flags & 0x1)
			ip.ki.dwFlags = KEYEVENTF_EXTENDEDKEY;
		else if(key->flags >> 7)
			ip.ki.dwFlags = KEYEVENTF_KEYUP;
		else
			ip.ki.dwFlags = 0x0000;

		vec.push_back(ip);
		
		if (vec.size() > 1 && vec[vec.size() - 2].ki.wVk == VK_LCONTROL && key->vkCode == 'Q' || key->vkCode == VK_ESCAPE) {
			std::cout << "Recording finished" << "\n\n";
			PostQuitMessage(0);
			vec.pop_back();
			vec.pop_back();
			vec[0].ki.time = firstTime;
			return UnhookWindowsHookEx(keyboardHook);
		}
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

KeySender::KeySender()
{
	ip.type = INPUT_KEYBOARD;

	ip.ki.wScan = 0;
	ip.ki.dwFlags = 0;
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;
}