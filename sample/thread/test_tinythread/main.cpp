#include <windef.h>
#include <tinythread.h>
#include <list>

using namespace tthread;
using namespace std;

int gCount = 10;
mutex gMutex;
condition_variable gCond;

void NotifierThreadProc(void* aArg)
{
	lock_guard<mutex> lock(gMutex);
	--gCount;
	gCond.notify_all();
}

void ConditionWaitThreadProc(void* aArg)
{
	printf(" Wating...");
	lock_guard<mutex> lock(gMutex);
	while (gCount > 0)
	{
		printf(".");
		gCond.wait(gMutex);
	}
	printf("end\n");
}

int main(int argc, char* argv[])
{
	printf("Condition variable (11 + 1 threads)");
	thread t1(ConditionWaitThreadProc, 0);

	list<thread*> threadList;
	for (int i = 0; i < 10; ++i)
		threadList.push_back(new thread(NotifierThreadProc, 0));

	t1.join();

	list<thread*>::iterator it;
	for (it = threadList.begin(); it != threadList.end(); ++it)
	{
		thread* t = *it;
		t->join();
		delete t;
	}

	return 0;
}