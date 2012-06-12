#include <stdio.h>
#include <windows.h>

#include "Utility/Timer.h"
#include "Game/FpsTimer.h"

int main()
{
	Timer timer;

	//timer.Start();
	//while (true) {
	//	printf("time=%g\n", timer.GetTime());
	//	Sleep(10);
	//}

	//timer.Start();
	//Sleep(1200);
	//timer.Stop();
	//printf("time=%g\n", timer.GetTime());
	//Sleep(2000);
	//timer.Start();
	//Sleep(500);
	//timer.Stop();
	//printf("time=%g\n", timer.GetTime());

	double d;
	FpsTimer fps;
	fps.SetFrameRate(60);
	timer.Start();
	d = timer.GetTime();
	while (true) {
		printf("fps=%d, time=%f\n", fps.GetFps(), timer.GetTime() - d);
		d = timer.GetTime();
		fps.Wait();
	}

	return 0;
}
