#include <stdio.h>
#include <SkyInputHandler.h>
#include <systemcall_impl.h>
#include <UnicodeFont.h>

int main(int argc, char** argv)
{
	QWORD qwWindowID;
	int iMouseX, iMouseY;
	int iWindowWidth, iWindowHeight;
	EVENT stReceivedEvent;
	MOUSEEVENT* pstMouseEvent;
	KEYEVENT* pstKeyEvent;
	WINDOWEVENT* pstWindowEvent;

	//--------------------------------------------------------------------------
	// �����츦 ����
	//--------------------------------------------------------------------------
	// ���콺�� ���� ��ġ�� ��ȯ
	Syscall_GetCursorPosition(&iMouseX, &iMouseY);

	// �������� ũ��� ���� ����
	iWindowWidth = 500;
	iWindowHeight = 200;

	// ������ ���� �Լ� ȣ��, ���콺�� �ִ� ��ġ�� �������� ����
	RECT rect;
	rect.left = iMouseX - 10;
	rect.top = iMouseY - WINDOW_TITLEBAR_HEIGHT / 2;
	rect.right = rect.left + iWindowWidth;
	rect.bottom = rect.top + iWindowHeight;
	Syscall_CreateWindow(&rect, "HanCmd", WINDOW_FLAGS_DEFAULT | WINDOW_FLAGS_RESIZABLE, &qwWindowID);

	// �����츦 �������� �������� ����
	if (qwWindowID == WINDOW_INVALIDID)
	{
		return 0;
	}

	UnicodeFont* pFont = new UnicodeFont();
	pFont->Initialize();

	//--------------------------------------------------------------------------
	// GUI �½�ũ�� �̺�Ʈ ó�� ����
	//--------------------------------------------------------------------------
	while (1)
	{
		// �̺�Ʈ ť���� �̺�Ʈ�� ����
		if (Syscall_ReceiveEventFromWindowQueue(&qwWindowID, &stReceivedEvent) == FALSE)
		{

			//20180628
			Syscall_Sleep(0);
			continue;
		}

		// ���ŵ� �̺�Ʈ�� Ÿ�Կ� ���� ������ ó��
		switch (stReceivedEvent.qwType)
		{
			// ���콺 �̺�Ʈ ó��
		case EVENT_MOUSE_MOVE:
		case EVENT_MOUSE_LBUTTONDOWN:
		case EVENT_MOUSE_LBUTTONUP:
		case EVENT_MOUSE_RBUTTONDOWN:
		case EVENT_MOUSE_RBUTTONUP:
		case EVENT_MOUSE_MBUTTONDOWN:
		case EVENT_MOUSE_MBUTTONUP:
			// ���⿡ ���콺 �̺�Ʈ ó�� �ڵ� �ֱ�
			pstMouseEvent = &(stReceivedEvent.stMouseEvent);
			break;

			// Ű �̺�Ʈ ó��
		case EVENT_KEY_DOWN:
		case EVENT_KEY_UP:
			// ���⿡ Ű���� �̺�Ʈ ó�� �ڵ� �ֱ�
			pstKeyEvent = &(stReceivedEvent.stKeyEvent);
			break;

			// ������ �̺�Ʈ ó��
		case EVENT_WINDOW_SELECT:
		case EVENT_WINDOW_DESELECT:
		case EVENT_WINDOW_MOVE:
		case EVENT_WINDOW_RESIZE:
		case EVENT_WINDOW_CLOSE:
			// ���⿡ ������ �̺�Ʈ ó�� �ڵ� �ֱ�
			pstWindowEvent = &(stReceivedEvent.stWindowEvent);

			//------------------------------------------------------------------
			// ������ �ݱ� �̺�Ʈ�̸� �����츦 �����ϰ� ������ �������� �½�ũ�� ����
			//------------------------------------------------------------------
			if (stReceivedEvent.qwType == EVENT_WINDOW_CLOSE)
			{
				// ������ ����
				Syscall_DeleteWindow(&qwWindowID);
				return 1;
			}
			break;

			// �� �� ����
		default:
			// ���⿡ �� �� ���� �̺�Ʈ ó�� �ڵ� �ֱ�
			break;
		}
	}
	
	return 0;
}
