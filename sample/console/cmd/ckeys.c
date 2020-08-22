#include "cmd.h"

extern TCHAR Fmt24[], KeysStr[];
extern struct envdata CmdEnv ;

/* global variables */
int KeysFlag = 0;	   /* KeysFlag indicates on / off */

int
EditLine( DataPtr, Buffer, MaxLength, ReturnLength )
CRTHANDLE DataPtr;
TCHAR *Buffer;	   /* the buffer to be filled */
		   /* has to be KBDBUFLEN+1 big */
int MaxLength;	   /* the Maximum length of the buffer, */
		   /* including <CR> */
int *ReturnLength; /* the number of characters in the buffer, not */
		   /* including the <CR> */
{
    BOOL flag;

    flag = ReadBufFromInput(
		CRTTONT(DataPtr),
		(TCHAR*)Buffer, MaxLength, ReturnLength);

    if (flag == 0 || *ReturnLength == 0) {
        return(GetLastError());
    }
    else {
        return(0);
    }
}

/**************** START OF SPECIFICATIONS ***********************/
/*								*/
/* SUBROUTINE NAME: eKeys					*/
/*								*/
/* DESCRIPTIVE NAME: Keys internal command			*/
/*								*/
/* FUNCTION: If no argument supplied then display the state	*/
/*	     of on / off.  If argument is on / off then 	*/
/*	     change the state.	If argument is list then	*/
/*	     display the stack. 				*/
/*								*/
/* NOTES: New for OS/2 1.2.					*/
/*								*/
/* ENTRY POINT: eKeys						*/
/*    LINKAGE:							*/
/*								*/
/* INPUT:							*/
/*	n							*/
/*								*/
/* EXIT-NORMAL: 						*/
/*	returns SUCCESS 					*/
/*								*/
/* EXIT-ERROR:							*/
/*	none.							*/
/*								*/
/* EFFECTS:							*/
/*								*/
/* INTERNAL REFERENCES: 					*/
/*    ROUTINES: 						*/
/*	PutStdErr						*/
/*	PutStdOut						*/
/*	TokStr							*/
/*	strcmpi 						*/
/*	strncpy 						*/
/*								*/
/* EXTERNAL REFERENCES: 					*/
/*    ROUTINES: 						*/
/**************** END OF SPECIFICATIONS *************************/

int
eKeys( n )
struct cmdnode *n; /* the command node for the Keys command */
{
    TCHAR *argptr;		 /* pointer to tozenized argument */

    /* get the value of the argument pointer */
    argptr = TokStr( n->argptr, 0, TS_NOFLAGS );

    /* take action based on the argument */

    if ((*argptr == 0) && (KeysFlag)) {
        PutStdOut( MSG_KEYS_ON, NOARGS );
        }

    else if ((*argptr == 0) && (!KeysFlag)) {
        PutStdOut( MSG_KEYS_OFF, NOARGS );
        }

    else if (!(argptr[ mystrlen( argptr ) + 1 ] == 0)) {
        /* too many parameters */
        PutStdErr( MSG_BAD_SYNTAX, NOARGS );
        }

    else if (_tcsicmp( argptr, TEXT("ON") ) == 0) {
        /* set keys on */
        KeysFlag = TRUE;
        SetEnvVar(KeysStr, TEXT("ON"), &CmdEnv);
        }
    else if (_tcsicmp( argptr, TEXT("OFF") ) == 0) {
        /* set keys off */
        KeysFlag = FALSE;
        SetEnvVar(KeysStr, TEXT("OFF"), &CmdEnv);
        }

    else if (_tcsicmp( argptr, TEXT("LIST") ) == 0) {
        }

    else {
        /* invalid parameter */
        PutStdErr( MSG_BAD_PARM1, NOARGS );
        }

    return SUCCESS;

} /* eKeys */
