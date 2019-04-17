#include <Windows.h>
#include <WinUser.h>
#include <iostream>
#include <vector>
#include <array>

#pragma once

class KeyFrame {
private:
	std::vector<WORD> perframe;
public:
	VOID addKey(WORD vKey);

	#ifdef DEBUG
		VOID printFrame() const;
	#endif
};

//assume each frame is 1ms
class KeyLogger {
private:
	std::vector<KeyFrame*> keyframes;

	VOID pushFrame();
public:
	VOID record();
	VOID recordFrames(size_t frames);
	VOID stoprecord();

	#ifdef DEBUG
	VOID printFrames() const;
	#endif

	~KeyLogger();
};

VOID sendKey(WORD vkCode);