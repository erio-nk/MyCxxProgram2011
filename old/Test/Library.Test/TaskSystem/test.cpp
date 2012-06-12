#include <stdio.h>
#include <windows.h>

#include "Game/TaskSystem.h"


TaskManager taskMgr;

//
class MyMessage : public TaskMessage
{
public:
	int x;
	MyMessage(int x) { this->x = x; }
};

//
class MyTask1 : public Task
{
public:
	void Update(float fDelta)
	{
		MyMessage t(64);
		pOwner->PostTaskMessage(&t, 2);
		printf(__FUNCTION__ " is update.\n");
	}
};


class MyTask2 : public Task
{
public:
	~MyTask2() {
		printf("MyTask2 deleted.\n");
	}

	void Update(float fDelta)
	{
		printf(__FUNCTION__ " is update.\n");
	}

	void OnMessage(TaskMessage* pMsg)
	{
		MyMessage* pMyMsg = (MyMessage*)pMsg;
		printf("received. %d\n", pMyMsg->x);
		taskMgr.DeleteTask(GetId());
	}
};

//
int main()
{
	taskMgr.Initialize();

	taskMgr.CreateTask<MyTask1>();
	taskMgr.CreateTask<MyTask2>();

	while (true) {
		taskMgr.Update(0.0f);
		Sleep(500);
	}

	taskMgr.Finalize();

	return 0;
}
