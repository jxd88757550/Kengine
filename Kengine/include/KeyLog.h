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
};

//assume each frame is 1ms
class KeyLogger {
private:
	std::vector<KeyLogger*> keyframes;

	VOID pushFrame();

	~KeyLogger();
public:
	VOID record();
	VOID stoprecord();
};

VOID sendKey(WORD vkCode);