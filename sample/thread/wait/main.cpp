#include <windef.h>
#include <memory.h>
#include <stdio.h>
#include <string.h>
#include <winapi.h>
#include <systemcall_impl.h>

LONG run_now = 1;
CHAR message[] = "Hello I'm a Thread";

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

DWORD WINAPI ChildThreadProc(LPVOID arg)
{
    int count = 0;
    printf("Child Thread is running. Argument sent was : %s\n", arg);
    printf("Entering child loop...\n");
   
    for (count = 0; count < 10; count++)
    {
        if(InterlockedCompareExchange(&run_now, 1, 2) == 2)
            printf("Child : 0x%x, %d\n", GetCurrentThreadId(), run_now);
        else
            Sleep(1000);
    }

    Sleep(2000);

    return 0;
}

int main(int argc, char* argv[])
{
    HANDLE thread;
    DWORD threadId;
   
    int count = 0;

    printf("Process ID is 0x%x\n", GetCurrentProcessId());
    printf("Main Thread ID is 0x%\n", GetCurrentThreadId());

    thread = CreateThread(NULL, 0, ChildThreadProc, (PVOID)message, 0, &threadId);

    if (thread == NULL)
    {
        printf("CreateThread() - Thread creation failed, error 0x%x\n", GetLastError());
        Syscall_exit(EXIT_FAILURE);
    }
    else
        printf("CreateThread() is OK, thread ID is 0x%x\n", threadId);

    printf("Entering main() loop...\n");

    for (count = 0; count< 10; count++)
    {
        if (InterlockedCompareExchange(&run_now, 2, 1) == 1)
            printf("Parent : 0x%x, %d\n", GetCurrentThreadId(), run_now);
        else
            Sleep(500);
    }
    printf("Waiting for thread 0x%x to finish...\n", threadId);

    if (WaitForSingleObject(thread, INFINITE) != WAIT_OBJECT_0)
    {
        printf("Thread join failed!Error 0x%x\n", GetLastError());
        Syscall_exit(EXIT_FAILURE);
    }
    else
        printf("WaitForSingleObject() is OK, an object was signalled...\n");

    if (CloseHandle(thread) != 0)
        printf("thread handle was closed successfully!\n");
    else
        printf("Failed to close a_thread handle, error 0x%x\n", GetLastError());

    return 0;
}