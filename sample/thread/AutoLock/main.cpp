#include <minwindef.h>
#include <size_t.h>
#include <winapi.h>
#include "typeinfo.h"
#include <AutoLock.h>
#include <systemcall_impl.h>
#include "excpt.h"

#define THROW_ATTRS(t)			((t).attributes)
#define TI_IsPure			0x00000008		// object thrown from a pure module
#define THROW_ISPURE(t)			(THROW_ATTRS(t) & TI_IsPure)

#define MAXIMUM_SUPPORTED_EXTENSION     512
#define SIZE_OF_80387_REGISTERS      80

typedef struct _FLOATING_SAVE_AREA {
	DWORD   ControlWord;
	DWORD   StatusWord;
	DWORD   TagWord;
	DWORD   ErrorOffset;
	DWORD   ErrorSelector;
	DWORD   DataOffset;
	DWORD   DataSelector;
	BYTE    RegisterArea[SIZE_OF_80387_REGISTERS];
	DWORD   Spare0;
} FLOATING_SAVE_AREA;

typedef struct _CONTEXT {

	//
	// The flags values within this flag control the contents of
	// a CONTEXT record.
	//
	// If the context record is used as an input parameter, then
	// for each portion of the context record controlled by a flag
	// whose value is set, it is assumed that that portion of the
	// context record contains valid context. If the context record
	// is being used to modify a threads context, then only that
	// portion of the threads context will be modified.
	//
	// If the context record is used as an IN OUT parameter to capture
	// the context of a thread, then only those portions of the thread's
	// context corresponding to set flags will be returned.
	//
	// The context record is never used as an OUT only parameter.
	//

	DWORD ContextFlags;

	//
	// This section is specified/returned if CONTEXT_DEBUG_REGISTERS is
	// set in ContextFlags.  Note that CONTEXT_DEBUG_REGISTERS is NOT
	// included in CONTEXT_FULL.
	//

	DWORD   Dr0;
	DWORD   Dr1;
	DWORD   Dr2;
	DWORD   Dr3;
	DWORD   Dr6;
	DWORD   Dr7;

	//
	// This section is specified/returned if the
	// ContextFlags word contians the flag CONTEXT_FLOATING_POINT.
	//

	FLOATING_SAVE_AREA FloatSave;

	//
	// This section is specified/returned if the
	// ContextFlags word contians the flag CONTEXT_SEGMENTS.
	//

	DWORD   SegGs;
	DWORD   SegFs;
	DWORD   SegEs;
	DWORD   SegDs;

	//
	// This section is specified/returned if the
	// ContextFlags word contians the flag CONTEXT_INTEGER.
	//

	DWORD   Edi;
	DWORD   Esi;
	DWORD   Ebx;
	DWORD   Edx;
	DWORD   Ecx;
	DWORD   Eax;

	//
	// This section is specified/returned if the
	// ContextFlags word contians the flag CONTEXT_CONTROL.
	//

	DWORD   Ebp;
	DWORD   Eip;
	DWORD   SegCs;              // MUST BE SANITIZED
	DWORD   EFlags;             // MUST BE SANITIZED
	DWORD   Esp;
	DWORD   SegSs;

	//
	// This section is specified/returned if the ContextFlags word
	// contains the flag CONTEXT_EXTENDED_REGISTERS.
	// The format and contexts are processor specific
	//

	BYTE    ExtendedRegisters[MAXIMUM_SUPPORTED_EXTENSION];

} CONTEXT;

typedef CONTEXT* PCONTEXT;


#define EXCEPTION_MAXIMUM_PARAMETERS 15 // maximum number of exception parameters

//
// Exception record definition.
//

typedef struct _EXCEPTION_RECORD {
	DWORD    ExceptionCode;
	DWORD ExceptionFlags;
	struct _EXCEPTION_RECORD* ExceptionRecord;
	PVOID ExceptionAddress;
	DWORD NumberParameters;
	ULONG_PTR ExceptionInformation[EXCEPTION_MAXIMUM_PARAMETERS];
} EXCEPTION_RECORD;

typedef EXCEPTION_RECORD* PEXCEPTION_RECORD;
typedef PVOID* PEXCEPTION_ROUTINE;

typedef struct _EXCEPTION_REGISTRATION_RECORD {
	struct _EXCEPTION_REGISTRATION_RECORD* Next;
	PEXCEPTION_ROUTINE Handler;
} EXCEPTION_REGISTRATION_RECORD;

typedef EXCEPTION_REGISTRATION_RECORD* PEXCEPTION_REGISTRATION_RECORD;




typedef struct EHEXCEPTION_RECORD
{
	DWORD ExceptionCode;
	DWORD ExceptionFlags;
	struct _EXCEPTION_RECODE* ExceptionRecord;
	void* ExceptionAddress;
	DWORD NumberParameters;
	struct EHParameters
	{
		DWORD magicNumber;
		void* pExceptionObject;
		_ThrowInfo* pThrowInfo;
	} params;
}EHEXCEPTION_RECORD;

#define EXCEPTION_NONCONTINUABLE 0x1    // Noncontinuable exception
#define EH_EXCEPTION_NUMBER 0xE06d7363    
#define EH_EXCEPTION_PARAMETERS 3
#define EH_MAGIC_NUMBER1 0x19930520
#define EH_PURE_MAGIC_NUMBER1   0x01994000

typedef struct _EXCEPTION_POINTERS {
	PEXCEPTION_RECORD ExceptionRecord;
	PCONTEXT ContextRecord;
} EXCEPTION_POINTERS, * PEXCEPTION_POINTERS;

__declspec(noreturn) extern "C" void _stdcall _CxxThrowException(void* pExceptionObject, _ThrowInfo * pThrowInfo)
{
	/*static const EHEXCEPTION_RECORD ExceptionTemplate =
	{
		EH_EXCEPTION_NUMBER,
		EXCEPTION_NONCONTINUABLE,
		NULL,
		NULL,
		EH_EXCEPTION_PARAMETERS,
	{
		EH_MAGIC_NUMBER1,
		NULL,
		NULL,
	}
	};

	EHEXCEPTION_RECORD ThisException = ExceptionTemplate;
	_ThrowInfo* pTI = (_ThrowInfo*)pThrowInfo;
	ThisException.params.pExceptionObject = pExceptionObject;
	ThisException.params.pThrowInfo = pTI;

	if (pTI != NULL)
	{
		if (THROW_ISPURE(*pTI))
		{
			ThisException.params.magicNumber = EH_PURE_MAGIC_NUMBER1;
		}
	}*/
	return;
	//EHTRACE_EXIT;

	//RaiseException(ThisException.ExceptionCode,
		//ThisException.ExceptionFlags,
		//ThisException.NumberParameters,
		//(PULONG_PTR)&ThisException.params);

	//Syscall_RaiseException(0, pExceptionObject);
}
#include "ehdata.h"
////////////////////////////////////////////////////////////////////////////////
//
// Forward declaration of local functions:
//

template <class T>
static void FindHandler(
	EHExceptionRecord*,
	EHRegistrationNode*,
	CONTEXT*,
	DispatcherContext*,
	typename T::FuncInfo*,
	BOOLEAN,
	int,
	EHRegistrationNode*
);

template <class T>
static void CatchIt(
	EHExceptionRecord*,
	EHRegistrationNode*,
	CONTEXT*,
	DispatcherContext*,
	typename T::FuncInfo*,
	typename T::HandlerType*,
	CatchableType*,
	typename T::TryBlockMapEntry*,
	int,
	EHRegistrationNode*,
	BOOLEAN,
	BOOLEAN
);

static void* CallCatchBlock(
	EHExceptionRecord*,
	EHRegistrationNode*,
	CONTEXT*,
	FuncInfo*,
	void*,
	int,
	unsigned long
);

template <class T>
static void FindHandlerForForeignException(
	EHExceptionRecord*,
	EHRegistrationNode*, CONTEXT*,
	DispatcherContext*,
	typename T::FuncInfo*,
	__ehstate_t,
	int,
	EHRegistrationNode*
);

static int ExFilterRethrow(
	EXCEPTION_POINTERS*
#if _EH_RELATIVE_FUNCINFO
	, EHExceptionRecord*,
	int*
#endif
);

#if _EH_RELATIVE_FUNCINFO
static int ExFilterRethrowFH4(
	EXCEPTION_POINTERS*,
	EHExceptionRecord*,
	__ehstate_t,
	int*
);
#endif

static BOOLEAN IsInExceptionSpec(
	EHExceptionRecord* pExcept,         // Information for this (logical)
										//   exception
	ESTypeList* pFuncInfo               // Static information for subject frame
);

static void CallUnexpected(ESTypeList* pESTypeList);

static BOOLEAN Is_bad_exception_allowed(ESTypeList* pExceptionSpec);


extern "C"  __declspec(naked)  EXCEPTION_DISPOSITION __cdecl __CxxFrameHandler3(

	//EHEXCEPTION_RECORD * pExcept,        // Information for this exception
	//EHRegistrationNode * pRN,            // Dynamic information for this frame
	//void* pContext,       // Context info (we don't care what's in it)
	//DispatcherContext * pDC             // More dynamic info for this frame (ignored on Intel)
	PEXCEPTION_RECORD pExcept,
	EXCEPTION_REGISTRATION_RECORD * pRN,
	PCONTEXT pContext,
	PVOID pDC)
{
	FuncInfo* pFuncInfo;
	EXCEPTION_DISPOSITION result;

	__asm {
		//
		// Standard function prolog
		//
		push    ebp
		mov     ebp, esp
		sub     esp, __LOCAL_SIZE
		push    ebx
		push    esi
		push    edi
		cld             // A bit of paranoia -- Our code-gen assumes this

		//
		// Save the extra parameter
		//
		mov     pFuncInfo, eax
	}

	//EHTRACE_FMT1("pRN = 0x%p", pRN);
	//result = __InternalCxxFrameHandler<RENAME_EH_EXTERN(__FrameHandler3)>( pExcept, pRN, (PCONTEXT)pContext, pDC, pFuncInfo, 0, nullptr, FALSE );


	//EHTRACE_HANDLER_EXIT(result);

	__asm {
		pop     edi
		pop     esi
		pop     ebx
		mov     eax, result
		mov     esp, ebp
		pop     ebp
		ret     0
	}
}

int _divider = 0;
int _dividend = 100;
void TestDivideByZero()
{
	int result = _dividend / _divider;

	//예외처리를 통해 아래 코드가 실행된다고 해도
	//result 결과는 정상적인 값이 아니다
	//위의 한줄은 어셈블리 명령어단에서 보면 여러 줄이며 
	//중간정도에서 오동작 부분을 수정했다 해서 정상적인 결과를 기대하는 것은 무리다.

	if (_divider != 0)
		result = _dividend / _divider;

}

/*void TestFunction(int value)
{
	try
	{
		throw value;
	}
	catch (int e)
	{
		printf("bbbbb %d\n", e);
	}
}*/

using namespace AutoLock;


#include <iostream>
using namespace std;

class A
{
public:
	virtual void f() { cout << "A::f()" << endl; }
};

class B : public A
{
public:
	void f() { cout << "B::f()" << endl; }
};

int main(int argc, char* argv[])
{
	DWORD dwHandler = (DWORD)__CxxFrameHandler3;

	A a;
	B b;
	a.f();        // A::f()
	b.f();        // B::f()

	A* pA = &a;
	B* pB = &b;
	pA->f();      // A::f()
	pB->f();      // B::f()

	pA = &b;
	// pB = &a;      // not allowed
	//pB = dynamic_cast<B*>(&a); // allowed but it returns NULL

	/*__asm
	{
		push	dwHandler
		push	fs : [0]
		mov		fs : [0] , esp
	}*/

	//CLockableMutex cs("test_mutex");
	int ab = 5;
	try
	{
		throw 100;
	}
	catch (int e)
	{
		printf("aaaaa %d\n", e);

	}

	/*__asm
	{
		mov		eax, [esp]
		mov		fs : [0] , eax
		add		esp, 8
	}*/

	printf("autolock main end\n");
	return 0;
}


class type_info {
public:
	virtual ~type_info();
	const char* name() const { return __name; }
	bool operator==(const type_info& __arg) const {
		return __name == __arg.__name;
	}

	bool operator!=(const type_info& __arg) const {
		return !operator==(__arg);
	}
protected:
	const char* __name;
};



type_info::~type_info()
{
}

DWORD g_dwValue;

EXCEPTION_DISPOSITION __cdecl YHD_ExceptHandler
(
	PEXCEPTION_RECORD pER, PVOID pFrame, PCONTEXT pCtx, PVOID pDC
)
{
	PEXCEPTION_REGISTRATION_RECORD pEF = PEXCEPTION_REGISTRATION_RECORD(pFrame);

	printf("==> YHD_ExceptHandler: Exception caught!!!\n");
	printf("    Frame=0x%08X, Prev=0x%08X, Handler=0x%08X\n", pEF, pEF->Next, pEF->Handler);
	printf("    Exception=0x%08X, Address=0x%08X\n", pER->ExceptionCode, pER->ExceptionAddress);

	pCtx->Eax = (DWORD)&g_dwValue;

	return ExceptionContinueExecution;
}
/*
int main(int argc, char* argv[])
{
	DWORD dwHandler = (DWORD)YHD_ExceptHandler;

	__asm
	{
		push	dwHandler
		push	fs : [0]
		mov		fs : [0] , esp
	}

	printf("Before Exception -> Value=%d\n", g_dwValue);
	__asm
	{
		mov		eax, 0
		mov[eax], 36
	}
	printf("After  Exception -> Value=%d\n", g_dwValue);

	__asm
	{
		mov		eax, [esp]
		mov		fs : [0] , eax
		add		esp, 8
	}

	return 0;
}*/