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
	for (WORD i = 0; i < 0xFF; ++i) {
		o = GetAsyncKeyState(i);

		if (o & 0x8000)
			frame->addKey(i);
	}

	keyframes.emplace_back(frame);
}

inline VOID KeyLogger::record()
{
#ifdef DEBUG
	std::cout << "Recording...\n";
#endif
	this->pushFrame();
}

VOID KeyLogger::recordFrames(size_t frames)
{
	for (size_t i = 0; i < frames; ++i) {
		this->record();
	}
}

inline VOID KeyLogger::stoprecord()
{
	return VOID();
}

#ifdef DEBUG
VOID KeyLogger::printFrames() const{
	for (size_t i = 0; i < keyframes.size(); ++i) {
		std::cout << "Frame: " << (i + 1) << '\n';
		keyframes[i]->printFrame();
		std::cout << '\n';
	}
}
#endif

KeyLogger::~KeyLogger()
{
	for (KeyFrame* obj : keyframes)
		delete obj;
	keyframes.clear();
}

inline VOID KeyFrame::addKey(WORD vKey)
{
	perframe.emplace_back(vKey);
}

#ifdef DEBUG
inline VOID KeyFrame::printFrame() const{
	for (size_t f = 0; f < perframe.size();++f) {
		std::cout << "Hex code: " << std::hex << perframe[f] << '\n';
	}
}
#endif