#include "revision.h"
#include <yuza_support.h>
#include "test.h"

void TestCode();

#if SKY_EMULATOR
#include <SkyVirtualInput.h>
#include "../win32stub/SkyOSWin32Stub.h"

extern unsigned int g_tickCount;
bool StartWin32Timer()
{
	return StartWin32StubTimer(new SkyVirtualInput(), g_tickCount);
} 
#endif

#define YUZA_DEBUGGER 0

void PrintInfomation()
{
	kprintf("\nYUZA OS Main Entry Entered\n");
	kprintf("revision %d\n", REVISION_BUILD);
	kprintf("version %d.%d\n", REVISION_MAJOR, REVISION_MINOR);
	kprintf("build time %s %s\n\n", BUILD_DATE, BUILD_TIME);
}

void YuzaOSConsole(char* consoleName)
{
	SkyConsole::Clear();
	PrintInfomation();
	PrintCurrentTime();  
	  
#if YUZA_DEBUGGER
	Debugger::GetInstance()->DebugKernel();
#else 
	//TestCode();
	HANDLE handle = kCreateProcess(consoleName, nullptr, 16);  
	SKY_ASSERT(handle != 0, "YuzaOSConsole exec fail!!\n"); 
	WatchDogProc(0); 
#endif
	
	//not reached
} 

#include <audio/_pci.h>
#include <audio/audio.h>
void TestCode()
{

#if !defined(SKY_EMULATOR)	
	pci_scan();
	pci_installDevices();
	audio_test(0);
#endif

	avl_tests();
	atomic_tests();
	cbuf_tests(); 
	test_timers();

	//kCreateThread((THREAD_START_ENTRY)SystemIdle, "Systemidle", 0, 16);
	//Debugger::GetInstance()->DebugKernel();
}
