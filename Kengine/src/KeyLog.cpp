#include "KeyLog.h"

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

inline VOID KeyLogger::pushFrame()
{
	KeyFrame* frame = new KeyFrame();

	SHORT o;
	for (WORD i = 0; i < sizeof(WORD); ++i) {
		o = GetAsyncKeyState(i);

		if (o)
			frame->addKey(i);
	}

	keyframes.emplace_back(frame);//something here
}

inline VOID KeyLogger::record()
{
	this->pushFrame();
}

inline VOID KeyLogger::stoprecord()
{
	return VOID();
}

inline KeyLogger::~KeyLogger()
{
	for (KeyLogger* obj : keyframes)
		delete obj;
	keyframes.clear();
}

inline VOID KeyFrame::addKey(WORD vKey)
{
	perframe.emplace_back(vKey);
}
