#include "KeyLog.h"

HHOOK keyboardHook;
KeyLogger Logger;

inline VOID sendKey(WORD vkCode) {
	// This structure will be used to create the keyboard
	// input event.
	INPUT ip;

	// Set up a generic keyboard event.
	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0; // hardware scan code for key
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;

	ip.ki.wVk = vkCode;
	ip.ki.dwFlags = 0;
	SendInput(1, &ip, sizeof(INPUT));

	ip.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &ip, sizeof(INPUT));
}

VOID KeyLogger::record()
{
	keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, NULL);
	std::cout << "Recording started" << '\n';
	MSG msg{ 0 };

	while (GetMessage(&msg, NULL, 0, 0) != 0);
	std::cout << "Unhooked...\n";
}

std::vector<tagKBDLLHOOKSTRUCT>& KeyLogger::getKeyFrames()
{
	return keyframes;
}

#ifdef DEBUG
VOID KeyLogger::printFrames() const{
	for (size_t i = 0; i < keyframes.size()-2; ++i) {
		std::cout<<static_cast<char>(keyframes[i].vkCode);
	}
	std::cout << '\n';
}
#endif

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	PKBDLLHOOKSTRUCT key = (PKBDLLHOOKSTRUCT)lParam;
	//a key was pressed
	if (wParam == WM_KEYDOWN && nCode == HC_ACTION)
	{
		std::vector<tagKBDLLHOOKSTRUCT>& vec = Logger.getKeyFrames();
		vec.push_back(*key);

		std::cout << key->vkCode << '\n';

		if (vec.size() > 1 && vec[vec.size() - 2].vkCode == VK_LCONTROL && vec[vec.size() - 1].vkCode == 'Q') {
			std::cout << "Recording finished" << "\n\n";
			PostQuitMessage(0);
			return UnhookWindowsHookEx(keyboardHook);
		}
	}
	return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}