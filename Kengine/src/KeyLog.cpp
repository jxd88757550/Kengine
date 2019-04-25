#include "KeyLog.h"

HHOOK keyboardHook;
HHOOK mouseHook;
KeyLogger Logger;
KeySender Sender;
DWORD initTime;
DWORD firstTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

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
	std::vector<VKKEYINFO>& vec = Logger.getKeyFrames();

	for (int i = 0; i < vec.size(); ++i) 
	{
		Sleep(vec[i].time);
		if (vec[i].isDown)
			this->keyDown(vec[i].vkCode);
		else
			this->keyUp(vec[i].vkCode);
	}
}

KeyLogger::KeyLogger() : keyframes(), indices(std::array<bool,255>())
{
	//indices.fill(0)
}

VOID KeyLogger::record()
{
	std::cout << "Creating Hook" << '\n';
	keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, NULL);
	mouseHook = SetWindowsHookEx(WH_MOUSE_LL, KeyboardProc, NULL, NULL);
	std::cout << "Recording started" << '\n';
	MSG msg{ 0 };

	std::cout << sizeof(bool) << std::endl;

	initTime = GetMessageTime();
	auto start = std::chrono::system_clock::now();
	std::cout <<"Init: " << std::chrono::system_clock::to_time_t(start) << std::endl;

	while (GetMessage(&msg, NULL, 0, 0) != 0);
	std::cout << "Unhooked...\n";
}

std::vector<VKKEYINFO>& KeyLogger::getKeyFrames()
{
	return keyframes;
}

std::array<bool, 255>& KeyLogger::getIndices()
{
	return indices;
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

	if (wParam == WM_SYSKEYUP || wParam == WM_SYSKEYDOWN || wParam == WM_KEYUP || wParam == WM_KEYDOWN && nCode == HC_ACTION)
	{
		std::vector<VKKEYINFO>& vec = Logger.getKeyFrames();

		if (vec.size() == 0)
		{
			firstTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) - firstTime;
		}

		VKKEYINFO info;
		info.vkCode = key->vkCode;
		info.isDown = !(key->flags >> 7);
		
		info.time = key->time - initTime;
		std::cout <<"Time: "<< info.time << std::endl;
		initTime = key->time;

		std::cout << info.vkCode << std::endl;
		std::array<bool, 255>& indices = Logger.getIndices();

		std::cout << indices.at((WORD)key->vkCode) << std::endl;

		if (indices.at((WORD)key->vkCode) != info.isDown) {
			vec.push_back(info);
			indices.at((WORD)key->vkCode) = info.isDown;
		}
		
		if (vec.size() > 1 && vec[vec.size() - 2].vkCode == VK_LCONTROL && key->vkCode == 'Q' || key->vkCode == VK_ESCAPE) {
			std::cout << "Recording finished" << "\n\n";
			PostQuitMessage(0);
			vec.pop_back();
			vec.pop_back();
			vec[0].time = firstTime;
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