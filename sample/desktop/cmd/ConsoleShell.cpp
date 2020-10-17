#include "ConsoleShell.h"
#include "Keyboard.h"
#include "string.h"
#include "memory.h"
#include <stdio.h>
//#include "window.h"
#include "GUIConsole.h"
#include <include/systemcall_impl.h>
#include <ConsoleManager.h>

extern SHELLCOMMANDENTRY gs_vstCommandTable;


extern "C" void printf(const char* str, ...);
DWORD WINAPI StartConsoleShell(LPVOID parameter)
{
	GUIConsole* pConsole = (GUIConsole*)parameter;
    char vcCommandBuffer[ CONSOLESHELL_MAXCOMMANDBUFFERCOUNT ];
    int iCommandBufferIndex = 0;
    BYTE bKey;
    int iCursorX, iCursorY;
    CONSOLEMANAGER* pstConsoleManager;	
    
    // �ܼ��� �����ϴ� �ڷᱸ���� ��ȯ
    pstConsoleManager = pConsole->GetConsoleManager();
    
    // ������Ʈ ���
   
    pConsole->PrintPrompt();
	//pConsole->Printf( CONSOLESHELL_PROMPTMESSAGE );
    
    // �ܼ� �� ���� �÷��װ� TRUE�� �� ������ �ݺ�
    while( pstConsoleManager->bExit == FALSE )
    {
		QWORD topWindowId = 0;
		Syscall_GetTopWindowID(&topWindowId);
		if (pConsole->GetWindowId() != topWindowId)
		{
			Syscall_Sleep(1);
			continue;
		}

        bKey = pConsole->GetCh();

        // �ܼ� �� ���� �÷��װ� ������ ��� ������ ����
        if( pstConsoleManager->bExit == TRUE )
        {
            break;
        }
        
        if( bKey == KEY_BACKSPACE )
        {
            if( iCommandBufferIndex > 0 )
            {
                // ���� Ŀ�� ��ġ�� �� �� ���� ������ �̵��� ���� ������ ����ϰ� 
                // Ŀ�ǵ� ���ۿ��� ������ ���� ����
				pConsole->GetCursor( &iCursorX, &iCursorY );
				pConsole->PrintStringXY( iCursorX - 1, iCursorY, " " );
				pConsole->SetCursor( iCursorX - 1, iCursorY );
                iCommandBufferIndex--;
            }
        }
        else if( bKey == KEY_ENTER || bKey == 0x0d)
        {
			pConsole->Printf( "\n" );
            
            if( iCommandBufferIndex > 0 )
            {
                // Ŀ�ǵ� ���ۿ� �ִ� ����� ����
                vcCommandBuffer[ iCommandBufferIndex ] = '\0';
				pConsole->ExecuteCommand( vcCommandBuffer );
            }
            else
            {
                pConsole->PrintPrompt();
            }
            
  
			//pConsole->Printf( "%s", CONSOLESHELL_PROMPTMESSAGE );
            memset( vcCommandBuffer, '\0', CONSOLESHELL_MAXCOMMANDBUFFERCOUNT );
            iCommandBufferIndex = 0;
        }
        // ����Ʈ Ű, CAPS Lock, NUM Lock, Scroll Lock�� ����
        else if( ( bKey == KEY_LSHIFT ) || ( bKey == KEY_RSHIFT ) ||
                 ( bKey == KEY_CAPSLOCK ) || ( bKey == KEY_NUMLOCK ) ||
                 ( bKey == KEY_SCROLLLOCK ) )
        {
            ;
        }
        else if( bKey < 128 )
        {
            // TAB�� �������� ��ȯ
            if( bKey == KEY_TAB )
            {
                bKey = ' ';
            }
            
            // ���۰� �������� ���� ����
            if( iCommandBufferIndex < CONSOLESHELL_MAXCOMMANDBUFFERCOUNT )
            {
                vcCommandBuffer[ iCommandBufferIndex++ ] = (char)bKey;
				pConsole->Printf( "%c", (char)bKey );
            }
        }

		Syscall_Sleep(1);
    }

	return 0;
}

/**
 *  �Ķ���� �ڷᱸ���� �ʱ�ȭ
 */
void kInitializeParameter( PARAMETERLIST* pstList, const char* pcParameter )
{
    pstList->pcBuffer = pcParameter;
    pstList->iLength = strlen( pcParameter );
    pstList->iCurrentPosition = 0;
}

/**
 *  �������� ���е� �Ķ������ ����� ���̸� ��ȯ
 */
int kGetNextParameter( PARAMETERLIST* pstList, char* pcParameter )
{
    int i;
    int iLength;

    // �� �̻� �Ķ���Ͱ� ������ ����
    if( pstList->iLength <= pstList->iCurrentPosition )
    {
        return 0;
    }
    
    // ������ ���̸�ŭ �̵��ϸ鼭 ������ �˻�
    for( i = pstList->iCurrentPosition ; i < pstList->iLength ; i++ )
    {
        if( pstList->pcBuffer[ i ] == ' ' )
        {
            break;
        }
    }
    
    // �Ķ���͸� �����ϰ� ���̸� ��ȯ
    memcpy( pcParameter, pstList->pcBuffer + pstList->iCurrentPosition, i );
    iLength = i - pstList->iCurrentPosition;
    pcParameter[ iLength ] = '\0';

    // �Ķ������ ��ġ ������Ʈ
    pstList->iCurrentPosition += iLength + 1;
    return iLength;
}
    
//==============================================================================
//  Ŀ�ǵ带 ó���ϴ� �ڵ�
//==============================================================================
/**
 *  �� ������ ���
 */
void kHelp(GUIConsole* pConsole, const char* pcCommandBuffer )
{
    int i;
    int iCount;
    int iCursorX, iCursorY;
    int iLength, iMaxCommandLength = 0;
    
    
	pConsole->Printf( "=========================================================\n" );
	pConsole->Printf( "                    SKYOS32 Shell Help                    \n" );
	pConsole->Printf( "=========================================================\n" );
    
	iCount = pConsole->GetCommandCount();
	SHELLCOMMANDENTRY* vstCommandTable = pConsole->GetCommandArray();

    // ���� �� Ŀ�ǵ��� ���̸� ���
    for( i = 0 ; i < iCount ; i++ )
    {
        iLength = strlen(vstCommandTable[ i ].pcCommand );
        if( iLength > iMaxCommandLength )
        {
            iMaxCommandLength = iLength;
        }
    }
    
    // ���� ���
    for( i = 0 ; i < iCount ; i++ )
    {
		pConsole->Printf( "%s", vstCommandTable[ i ].pcCommand );
		pConsole->GetCursor( &iCursorX, &iCursorY );
      //  kSetCursor( iMaxCommandLength, iCursorY );
		pConsole->Printf( "  - %s\n", vstCommandTable[ i ].pcHelp );

        // ����� ���� ��� ������ ������
        if( ( i != 0 ) && ( ( i % 20 ) == 0 ) )
        {
			pConsole->Printf( "Press any key to continue... ('q' is exit) : " );
            if(pConsole->GetCh() == 'q' )
            {
				pConsole->Printf( "\n" );
                break;
            }        
			pConsole->Printf( "\n" );
        }
    }
}

/**
 *  ȭ���� ���� 
 */
void kCls(GUIConsole* pConsole, const char* pcParameterBuffer )
{
    // �� ������ ����� ������ ����ϹǷ� ȭ���� ���� ��, ���� 1�� Ŀ�� �̵�
	pConsole->ClearScreen();
	pConsole->SetCursor( 0, 0 );
}

/**
 *  �� �޸� ũ�⸦ ���
 */
void kShowTotalRAMSize(GUIConsole* pConsole, const char* pcParameterBuffer )
{
	//pConsole->Printf( "Total RAM Size = %d MB\n", kGetTotalRAMSize() );
}

/**
 *  PC�� �����(Reboot)
 */
void kShutdown(GUIConsole* pConsole, const char* pcParamegerBuffer )
{
	pConsole->Printf( "System Shutdown Start...\n" );
}