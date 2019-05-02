#include "KeyLog.h"

int main()
{
	//Sleep(5000);
	//Logger.record();
	//Logger.printFrames();

	//Sleep(5000);
	
	//Sender.sendKeys();

	for (int i = 0; i < 150; ++i) {
		Sender.mouseEvent(MouseCoord(RELATIVE, 1, 0), MouseEvent::MOVE);
		Sleep(10000);
	}

	return 0;
}