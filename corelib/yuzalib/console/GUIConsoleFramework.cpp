#include "GUIConsoleFramework.h"
#include <memory.h>
#include <skyoswindow.h>
#include <systemcall_impl.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <yuzaapi.h>
#include "GUIConsole.h"

#define FONT_ENGLISHWIDTH   8
#define FONT_ENGLISHHEIGHT  16

// �ѱ� ��Ʈ�� �ʺ�� ����
#define FONT_HANGULWIDTH   16
#define FONT_HANGULHEIGHT  16

/**
 *  �簢���� �ʺ� ��ȯ
 */
inline int GetRectangleWidth(const RECT* pstArea)
{
	int iWidth;

	iWidth = pstArea->right - pstArea->left + 1;

	if (iWidth < 0)
	{
		return -iWidth;
	}

	return iWidth;
}

/**
 *  �簢���� ���̸� ��ȯ
 */
inline int GetRectangleHeight(const RECT* pstArea)
{
	int iHeight;

	iHeight = pstArea->bottom - pstArea->top + 1;

	if (iHeight < 0)
	{
		return -iHeight;
	}

	return iHeight;
}
GUIConsole* pConsole = 0;
QWORD g_qwWindowID = 0;
char* consoleName = "ConsoleShell";
bool GUIConsoleFramework::Run()
{
	CONSOLE_START_STRUCT consoleStruct;
	consoleStruct.argc = 1;
	consoleStruct.argv = (char**)calloc(1, sizeof(char*));
	consoleStruct.argv[0] = consoleName;
	consoleStruct.entry = StartConsoleShell;
	
	bool result = MainLoop(&consoleStruct);
	free(consoleStruct.argv);

	return result;
}

DWORD WINAPI GUIConsoleProc(LPVOID parameter)
{
	Syscall_RegisterWindowId(&g_qwWindowID);
	CONSOLE_START_STRUCT* console = (CONSOLE_START_STRUCT*)(parameter);
	return console->entry(console->argc, console->argv);
}

bool GUIConsoleFramework::Run(int argc, char** argv, MAIN_IMPL entry)
{
	bool isGrahpicMode = Syscall_IsGraphicMode();

	if (isGrahpicMode)
	{
		CONSOLE_START_STRUCT consoleStruct;
		consoleStruct.argc = argc;
		consoleStruct.argv = argv;
		consoleStruct.entry = entry;

		return MainLoop2(&consoleStruct);
	}
		
	return entry(argc, argv);
}

bool GUIConsoleFramework::MainLoop(CONSOLE_START_STRUCT* args)
{
	QWORD qwWindowID = WINDOW_INVALIDID;
	int iWindowWidth, iWindowHeight;
	EVENT stReceivedEvent;
	KEYEVENT* pstKeyEvent;
	WINDOWEVENT* windowEvent;
	RECT stScreenArea;
	KEYDATA stKeyData;


	Syscall_GetScreenArea(&stScreenArea);

	// �������� ũ�� ����, ȭ�� ���ۿ� ���� ������ �ִ� �ʺ�� ���̸� ����ؼ� ���
	iWindowWidth = CONSOLE_WIDTH * FONT_ENGLISHWIDTH + 4;
	iWindowHeight = CONSOLE_HEIGHT * FONT_ENGLISHHEIGHT + WINDOW_TITLEBAR_HEIGHT + 2;

	// ������ ���� �Լ� ȣ��, ȭ�� ����� ����
	RECT rect;
	rect.left = (stScreenArea.right - iWindowWidth) / 2;
	rect.top = (stScreenArea.bottom - iWindowHeight) / 2;
	rect.right = rect.left + iWindowWidth;
	rect.bottom = rect.top + iWindowHeight;

	bool result = Syscall_CreateWindow(&rect, args->argv[0], WINDOW_FLAGS_DEFAULT | WINDOW_FLAGS_RESIZABLE, &qwWindowID);
	// �����츦 �������� �������� ����
	if (qwWindowID == WINDOW_INVALIDID)
	{
		return 0;
	}

	g_qwWindowID = qwWindowID;

	pConsole = new GUIConsole();
	pConsole->Initialize(0, 0);
	pConsole->SetWindowId(qwWindowID);

	Syscall_CreateThread(GUIConsoleProc, args->argv[0], args, 16);
	CONSOLE_START_STRUCT* console = (CONSOLE_START_STRUCT*)args;
	// ���� ȭ�� ���۸� �ʱ�ȭ
	CHARACTER* vstPreviousScreenBuffer = new CHARACTER[CONSOLE_WIDTH * CONSOLE_HEIGHT];
	memset(vstPreviousScreenBuffer, (char)0xFF, CONSOLE_WIDTH * CONSOLE_HEIGHT * sizeof(CHARACTER));

	//--------------------------------------------------------------------------
	// GUI �½�ũ�� �̺�Ʈ ó�� ����
	//--------------------------------------------------------------------------
	while (1)
	{
		// ȭ�� ������ ����� ������ �����쿡 ������Ʈ
		pConsole->ProcessConsoleBuffer(qwWindowID, vstPreviousScreenBuffer);

		// �̺�Ʈ ť���� �̺�Ʈ�� ����
		if (Syscall_ReceiveEventFromWindowQueue(&qwWindowID, &stReceivedEvent) == FALSE)
		{
			//20180628
			Syscall_Sleep(1);
			continue;
		}

		// ���ŵ� �̺�Ʈ�� Ÿ�Կ� ���� ������ ó��
		switch (stReceivedEvent.qwType)
		{
			// Ű �̺�Ʈ ó��
		case EVENT_KEY_DOWN:
		case EVENT_KEY_UP:
			// ���⿡ Ű���� �̺�Ʈ ó�� �ڵ� �ֱ�
			pstKeyEvent = &(stReceivedEvent.stKeyEvent);
			stKeyData.bASCIICode = pstKeyEvent->bASCIICode;
			stKeyData.bFlags = pstKeyEvent->bFlags;
			stKeyData.bScanCode = pstKeyEvent->bScanCode;

			
			// Ű�� �׷��� ���� Ű ť�� �����Ͽ� �� �½�ũ�� �Է����� ����
			pConsole->PutKeyToGUIKeyQueue(&stKeyData);		

			break;
		case EVENT_CONSOLE_PRINT:
		{
			windowEvent = &(stReceivedEvent.stWindowEvent);
			int iNextPrintOffset = pConsole->ConsolePrintString((char*)windowEvent->stringEvent);
			if (strlen((char*)windowEvent->stringEvent) == 0)
				break;

			pConsole->SetCursor(iNextPrintOffset % CONSOLE_WIDTH, iNextPrintOffset / CONSOLE_WIDTH);

			break;
		}
		case EVENT_CONSOLE_COMMAND_END:
		{
			pConsole->PrintPrompt();

			break;
		}
		// ������ �̺�Ʈ ó��
		case EVENT_WINDOW_CLOSE:
			// ������ �� �½�ũ�� ����ǵ��� ���� �÷��׸� �����ϰ� ����� ������ ���
		   // kSetConsoleShellExitFlag( TRUE );
			//while( kIsTaskExist( qwConsoleShellTaskID ) == TRUE )
		{
			//20180628
			Syscall_Sleep(1);
		}

		// �����츦 �����ϰ� ������ ID�� �ʱ�ȭ
		Syscall_DeleteWindow(&qwWindowID);
		qwWindowID = WINDOW_INVALIDID;
		return 0;

		break;

		// �� �� ����
		default:
			// ���⿡ �� �� ���� �̺�Ʈ ó�� �ڵ� �ֱ�
			break;
		}
	}

	return true;
}

bool GUIConsoleFramework::MainLoop2(CONSOLE_START_STRUCT* args)
{
	QWORD qwWindowID = WINDOW_INVALIDID;
	int iWindowWidth, iWindowHeight;
	EVENT stReceivedEvent;
	KEYEVENT* pstKeyEvent;
	WINDOWEVENT* windowEvent;
	RECT stScreenArea;
	KEYDATA stKeyData;

	Syscall_GetScreenArea(&stScreenArea);

	// �������� ũ�� ����, ȭ�� ���ۿ� ���� ������ �ִ� �ʺ�� ���̸� ����ؼ� ���
	iWindowWidth = CONSOLE_WIDTH * FONT_ENGLISHWIDTH + 4;
	iWindowHeight = CONSOLE_HEIGHT * FONT_ENGLISHHEIGHT + WINDOW_TITLEBAR_HEIGHT + 2;

	// ������ ���� �Լ� ȣ��, ȭ�� ����� ����
	RECT rect;
	rect.left = (stScreenArea.right - iWindowWidth) / 2;
	rect.top = (stScreenArea.bottom - iWindowHeight) / 2;
	rect.right = rect.left + iWindowWidth;
	rect.bottom = rect.top + iWindowHeight;

	bool result = Syscall_CreateWindow(&rect, args->argv[0], WINDOW_FLAGS_DEFAULT | WINDOW_FLAGS_RESIZABLE, &qwWindowID);
	// �����츦 �������� �������� ����
	if (qwWindowID == WINDOW_INVALIDID)
	{
		return 0;
	}

	g_qwWindowID = qwWindowID;

	pConsole = new GUIConsole();
	pConsole->Initialize(0, 0);
	pConsole->SetWindowId(qwWindowID);

	Syscall_CreateThread(GUIConsoleProc, args->argv[0], args, 16);

	CONSOLE_START_STRUCT* console = (CONSOLE_START_STRUCT*)args;
	// ���� ȭ�� ���۸� �ʱ�ȭ
	CHARACTER* vstPreviousScreenBuffer = new CHARACTER[CONSOLE_WIDTH * CONSOLE_HEIGHT];
	memset(vstPreviousScreenBuffer, (char)0xFF, CONSOLE_WIDTH * CONSOLE_HEIGHT * sizeof(CHARACTER));

	//--------------------------------------------------------------------------
	// GUI �½�ũ�� �̺�Ʈ ó�� ����
	//--------------------------------------------------------------------------
	while (1)
	{
		// ȭ�� ������ ����� ������ �����쿡 ������Ʈ
		pConsole->ProcessConsoleBuffer(qwWindowID, vstPreviousScreenBuffer);

		// �̺�Ʈ ť���� �̺�Ʈ�� ����
		if (Syscall_ReceiveEventFromWindowQueue(&qwWindowID, &stReceivedEvent) == FALSE)
		{
			//20180628
			Syscall_Sleep(1);
			continue;
		}

		// ���ŵ� �̺�Ʈ�� Ÿ�Կ� ���� ������ ó��
		switch (stReceivedEvent.qwType)
		{
			// Ű �̺�Ʈ ó��
		case EVENT_KEY_DOWN:
		//case EVENT_KEY_UP:

			if (pConsole->ProcessKey(stReceivedEvent.stKeyEvent.bASCIICode))
			{
				stReceivedEvent.qwType = EVENT_CONSOLE_KEY;
				stReceivedEvent.stKeyEvent.qwWindowID = qwWindowID;
				Syscall_SendEventToWindowManager(&stReceivedEvent);
			}
			
			
			
			break;
		case EVENT_CONSOLE_PRINT:
		{
			windowEvent = &(stReceivedEvent.stWindowEvent);
			int iNextPrintOffset = pConsole->ConsolePrintString((char*)windowEvent->stringEvent);
			if (strlen((char*)windowEvent->stringEvent) == 0)
				break;

			pConsole->SetCursor(iNextPrintOffset % CONSOLE_WIDTH, iNextPrintOffset / CONSOLE_WIDTH);

			break;
		}
		case EVENT_CONSOLE_COMMAND_END:
		{
			pConsole->PrintPrompt();

			break;
		}
		// ������ �̺�Ʈ ó��
		case EVENT_WINDOW_CLOSE:
			// ������ �� �½�ũ�� ����ǵ��� ���� �÷��׸� �����ϰ� ����� ������ ���
		   // kSetConsoleShellExitFlag( TRUE );
			//while( kIsTaskExist( qwConsoleShellTaskID ) == TRUE )
		{
			//20180628
			Syscall_Sleep(1);
		}

		// �����츦 �����ϰ� ������ ID�� �ʱ�ȭ
		Syscall_DeleteWindow(&qwWindowID);
		qwWindowID = WINDOW_INVALIDID;
		return 0;

		break;

		// �� �� ����
		default:
			// ���⿡ �� �� ���� �̺�Ʈ ó�� �ڵ� �ֱ�
			break;
		}
	}

	return true;
}