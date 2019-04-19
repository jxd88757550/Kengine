#include <Windows.h>
#include <WinUser.h>
#include <iostream>
#include <vector>
#include <array>
#include <stdio.h>

class KeyLogger;
extern KeyLogger Logger;

#pragma once

class KeyLogger {
private:
	std::vector<tagKBDLLHOOKSTRUCT> keyframes;
public:
	VOID record();
	std::vector<tagKBDLLHOOKSTRUCT>& getKeyFrames();

	#ifdef DEBUG
	VOID printFrames() const;
	#endif
};

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

VOID sendKey(WORD vkCode);