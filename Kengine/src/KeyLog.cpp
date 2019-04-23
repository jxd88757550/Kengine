#include "KeyLog.h"

HHOOK keyboardHook;
HHOOK mouseHook;
KeyLogger Logger;

VOID sendKey(WORD vkCode) {
	INPUT ip;

	// Set up a generic keyboard event.
	ip.type = INPUT_KEYBOARD;

	ip.ki.wVk = vkCode;
	ip.ki.wScan = 0; // hardware scan code for key
	ip.ki.dwFlags = 0;
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;

	SendInput(1, &ip, sizeof(INPUT));

	ip.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &ip, sizeof(INPUT));
}

VOID KeyLogger::record()
{
	std::cout << "Creating Hook" << '\n';
	keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, NULL);
	mouseHook = SetWindowsHookEx(WH_MOUSE_LL, KeyboardProc, NULL, NULL);
	std::cout << "Recording started" << '\n';
	MSG msg{ 0 };

	while (GetMessage(&msg, NULL, 0, 0) != 0);
	std::cout << "Unhooked...\n";
}

std::vector<tagKBDLLHOOKSTRUCT>& KeyLogger::getKeyFrames()
{
	return keyframes;
}

VOID KeyLogger::sendKeys()
{
	for (tagKBDLLHOOKSTRUCT i : keyframes) {
		INPUT ip;

		// Set up a generic keyboard event.
		ip.type = INPUT_KEYBOARD;

		ip.ki.wVk = i.vkCode;
		ip.ki.wScan = 0; // hardware scan code for key
		ip.ki.dwFlags = 0;
		ip.ki.time = 0;
		ip.ki.dwExtraInfo = 0;

		SendInput(1, &ip, sizeof(INPUT));

		ip.ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, &ip, sizeof(INPUT));
	}
}

#ifdef DEBUG
VOID KeyLogger::printFrames() const{
	for (size_t i = 0; i < keyframes.size(); ++i) {
		std::cout<<static_cast<char>(keyframes[i].vkCode);
	}
	std::cout << '\n';
}
#endif

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	PKBDLLHOOKSTRUCT key = (PKBDLLHOOKSTRUCT)lParam;
	//a key was pressed

	if (wParam == WM_LBUTTONDOWN && nCode == HC_ACTION) {
		std::cout << "Left click\n";
	}
	//mouse events

	if (wParam == WM_SYSKEYDOWN || wParam == WM_KEYDOWN && nCode == HC_ACTION)
	{
		std::vector<tagKBDLLHOOKSTRUCT>& vec = Logger.getKeyFrames();
		vec.push_back(*key);

		#ifdef DEBUG
				std::cout << key->vkCode << '\n';
		#endif

		if (vec.size() > 1 && vec[vec.size() - 2].vkCode == VK_LCONTROL && key->vkCode == 'Q') {
			std::cout << "Recording finished" << "\n\n";
			PostQuitMessage(0);
			vec.pop_back();
			vec.pop_back();
			return UnhookWindowsHookEx(keyboardHook);
		}
	}
	else if (wParam == WM_KEYUP && nCode == HC_ACTION) {
		
	}

	return CallNextHookEx(NULL, nCode, wParam, lParam);
}