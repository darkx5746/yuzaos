//#include <windows.h>
//#include <typeinfo>
#include "Excpt.h"
#include <memory.h>

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

#pragma warning (disable: 4731) // ebp register overwritten
#pragma warning (disable: 4733) // fs:[0] accessed
#pragma warning (disable: 4200) // zero-sized arrays
#pragma warning (disable: 4094)	// untagged class

#ifdef _DEBUG
#	define ASSERT(x) do { if (!(x)) __debugbreak(); } while (false)
#else
#	define ASSERT(x) __noop
#endif

template <bool ok> struct AssertTest;
template <> struct AssertTest<true> {};
#define STATIC_ASSERT(x) class { class AssertTstAtLine##__LINE__ :public AssertTest<(bool) (x)> {}; }


inline void Verify(bool bVal)
{
	if (!bVal) __debugbreak();
}

//#define CHKREG_ENABLE

#ifdef CHKREG_ENABLE

#	define CHKREG_PROLOG_NO_EBX \
	{ \
		_asm push ebp \
		_asm push esi \
		_asm push edi \
	}

#	define CHKREG_PROLOG \
	{ \
		_asm push ebx \
	} \
	CHKREG_PROLOG_NO_EBX


#	define CHKREG_EPILOG_REG(reg) \
		_asm cmp reg, [esp] \
		_asm je Ok##reg \
		_asm int 3 \
		_asm Ok##reg: \
		_asm add esp, 4

#	define CHKREG_EPILOG_NO_EBX \
	{ \
		CHKREG_EPILOG_REG(edi) \
		CHKREG_EPILOG_REG(esi) \
		CHKREG_EPILOG_REG(ebp) \
	}

#	define CHKREG_EPILOG \
	CHKREG_EPILOG_NO_EBX \
	{ \
		CHKREG_EPILOG_REG(ebx) \
	}


#else // CHKREG_ENABLE

#	define CHKREG_PROLOG_NO_EBX
#	define CHKREG_PROLOG
#	define CHKREG_EPILOG_NO_EBX
#	define CHKREG_EPILOG

#endif // CHKREG_ENABLE

////////////////////////////////////////
// Exception registration record (not only C++)
struct ExcReg
{ 
	ExcReg*	m_pPrev;
	PVOID	m_pfnHandler;

	inline static ExcReg* get_Top();
	inline void put_Top(); // can call even for NULL obj

	void Install() {
		m_pPrev = get_Top();
		put_Top();
	}

	void Dismiss() {
		Verify(this == get_Top()); // corruption test
		m_pPrev->put_Top();
	}

	bool IsValid() const { return this != (ExcReg*) (-1); }
};

ExcReg* ExcReg::get_Top()
{
	ExcReg* pExcReg;
	_asm
	{
		mov eax, fs:[0]
		mov pExcReg, eax
	}
	return pExcReg;
}

void ExcReg::put_Top() // can call even for NULL obj
{
	ExcReg* pExcReg = this;
	_asm
	{
		mov eax, pExcReg;
		mov fs:[0], eax
	}
}

static const DWORD EXC_FLAG_UNWIND = 6;



EXCEPTION_DISPOSITION Exc::MonitorRaw::HandlerStat(EXCEPTION_RECORD* pExc, PVOID pPtr, CONTEXT* pCpuCtx)
{
	ASSERT(pPtr);
	ExcReg* pExcReg = (ExcReg*) pPtr;
	Monitor* pThis = (Monitor*) (PBYTE(pPtr) - offsetof(Monitor, m_pOpaque));

	if (EXC_FLAG_UNWIND & pExc->ExceptionFlags)
	{
		pExcReg->m_pfnHandler = NULL; // dismissed
		pThis->AbnormalExit();
	} else
		if (pThis->Handle(pExc, pCpuCtx))
		{
			ASSERT(!(EXCEPTION_NONCONTINUABLE & pExc->ExceptionFlags));
			return ExceptionContinueExecution;
		}

	return ExceptionContinueSearch;

}

Exc::MonitorRaw::MonitorRaw()
{
	STATIC_ASSERT(sizeof(m_pOpaque) == sizeof(ExcReg));
	((ExcReg&) m_pOpaque).m_pfnHandler = HandlerStat;
	((ExcReg&) m_pOpaque).Install();

}

void Exc::MonitorRaw::NormalExit()
{
	STATIC_ASSERT(sizeof(m_pOpaque) == sizeof(ExcReg));
	ASSERT(((ExcReg&) m_pOpaque).m_pfnHandler);
	((ExcReg&) m_pOpaque).Dismiss();
}


Exc::Monitor::~Monitor()
{
	STATIC_ASSERT(sizeof(m_pOpaque) == sizeof(ExcReg));
	if (((ExcReg&) m_pOpaque).m_pfnHandler)
		((ExcReg&) m_pOpaque).Dismiss();
}

////////////////////////////////////////
// Exception registration record built by C++ compiler
struct ExcRegCpp
	:public ExcReg
{ 
	union {
		long	m_NextCleanup;	// if used for cleanup
		DWORD	m_dwTryID;		// if used for search
	};
	ULONG	m_nEbpPlaceholder;
};

////////////////////////////////////////
// Thrown type description (_s__ThrowInfo)
struct ThrownType
{
	DWORD	u1;
	PVOID	m_pfnDtor;
	DWORD	u2;

	struct Sub
	{
		DWORD		u1;
		type_info*	ti; 
		DWORD		u2[3];
		DWORD		m_nSize;
		PVOID		m_pfnCopyCtor;
	};

	struct Table {
		long m_nCount;
		Sub* m_pArr[];
	} *m_pTable;
};

////////////////////////////////////////
// Frame info, generated by C++ compiler for __CxxFrameHandler3
struct FrameInfo
{
	ULONG			m_sig;

	template <typename T> struct Arr_T {
		long	m_nCount;
		T*		m_pArr;
	};

	// Cleanup data
	struct Cleanup {
		long	m_nPrevIdx;
		PVOID	m_pfnCleanup; // __stdcall, takes no params
	};
	Arr_T<Cleanup> m_Cleanup;

	// Try blocks
	struct Try {
		DWORD	m_IdStart;
		DWORD	m_IdEnd;
		DWORD	m_u1;

		// Catch blocks
		struct Catch {
			DWORD		m_ValOrRef;
			type_info*	m_ti;
			int			m_Offset;	// from EBP where exception should be copied.
			DWORD		m_Catchblock_addr;
		};
		Arr_T<Catch> m_Catch;
	};
	Arr_T<Try> m_Try;

	EXCEPTION_DISPOSITION __thiscall Handler(EXCEPTION_RECORD* pExc, ExcRegCpp* pExcRegCpp, CONTEXT* pCpuCtx);
};


// Worker struct
struct WorkerForLocalUnwind
{
	FrameInfo*	m_pFrame;
	ExcRegCpp*	m_pExcRegCpp;
	ULONG		m_nEbp;

	void UnwindLocal(long nStopAtId);
	void UnwindLocalWithGuard();
};

struct WrapExcRecord
{
	EXCEPTION_RECORD*	m_pExc;
	CONTEXT*			m_pCpuCtx;

	EXCEPTION_DISPOSITION InvokeHandler(const ExcReg&) const;
	void RaiseExcRaw(ExcReg* pStart) const throw(...);
	void Unhandled() const;
};



// Last exception information (per-thread)
// This structure is used to support nested exceptions and "rethrow"
struct ExcMon
	:public ExcReg
	,public WrapExcRecord
{
	
	//__declspec(thread) static ExcMon* s_pInst;
	//__declspec(thread) static bool s_bExcAllowed;

	static ExcMon* s_pInst;
	static bool s_bExcAllowed;

	static EXCEPTION_DISPOSITION Handler(EXCEPTION_RECORD* pExc, ExcMon* pThis, CONTEXT* pCpuCtx) {
		ASSERT(pExc && pThis);
		if (EXC_FLAG_UNWIND & pExc->ExceptionFlags)
			pThis->RemoveLastExc();
		else
			Verify(s_bExcAllowed); // Check for EH corruption

		return ExceptionContinueSearch;
	}

	ExcMon(EXCEPTION_RECORD* pExc, CONTEXT* pCpuCtx)
	{
		ExcReg::m_pfnHandler = Handler;
		ExcReg::Install();

		m_pExc		= pExc;
		m_pCpuCtx	= pCpuCtx;
		s_pInst		= this;
	}

	void AppendExc(EXCEPTION_RECORD* pExc)
	{
		ASSERT(pExc && (pExc != m_pExc));
		pExc->ExceptionRecord = m_pExc;
		m_pExc = pExc;
	}

	void RemoveLastExc()
	{
		ASSERT(this == s_pInst);
		s_pInst = NULL;
	}

	void NormalExit()
	{
		RemoveLastExc();
		ExcReg::Dismiss();
	}
};

//__declspec(thread) ExcMon* ExcMon::s_pInst = NULL;
//__declspec(thread) bool ExcMon::s_bExcAllowed = false;

ExcMon* ExcMon::s_pInst = NULL;
bool ExcMon::s_bExcAllowed = false;



struct WorkerForAll
	:public WorkerForLocalUnwind
{
	ThrownType*	m_pThrownType;
	PVOID		m_pThrownObj;

	void Catch();

	__forceinline DWORD CatchInternal(const FrameInfo::Try&, const FrameInfo::Try::Catch&, EXCEPTION_RECORD*); // called only once
	void UnwindUntil(const FrameInfo::Try&);
	__forceinline void AssignCatchObj(const FrameInfo::Try::Catch&, const ThrownType::Sub&); // called only once
	__forceinline DWORD CallCatchRaw(const FrameInfo::Try::Catch&); // called only once
	__forceinline DWORD CallCatchDestroyObj(const FrameInfo::Try::Catch&); // small
	__forceinline void DestroyThrownObjSafe(); // called once
	__forceinline __declspec(noreturn) void PassCtlAfterCatch(DWORD dwAddr); // called once
};


void WorkerForLocalUnwind::UnwindLocal(long nStopAtId)
{
	const FrameInfo::Cleanup* pCleanupArr = m_pFrame->m_Cleanup.m_pArr;
	ULONG nEbp = m_nEbp;

	while (true)
	{
		long nNextID = m_pExcRegCpp->m_NextCleanup;
		if (nNextID <= nStopAtId)
			break;

		Verify(nNextID < m_pFrame->m_Cleanup.m_nCount); // stack corruption test

		const FrameInfo::Cleanup& entry = pCleanupArr[nNextID];
		PVOID pfnCleanup = entry.m_pfnCleanup;
		m_pExcRegCpp->m_NextCleanup = entry.m_nPrevIdx;

		if (pfnCleanup)
		{
			ExcMon::s_bExcAllowed = true;

			_asm
			{
				mov eax, pfnCleanup
				mov ebx, ebp	// save current ebp

				CHKREG_PROLOG

				push esi
				push edi

				mov ebp, nEbp	// the ebp of the unwinding function
				call eax
				mov ebp, ebx	// restore ebp

				pop edi
				pop esi

				CHKREG_EPILOG
			}

			ExcMon::s_bExcAllowed = false;
		}
	}
}

void WorkerForLocalUnwind::UnwindLocalWithGuard()
{
	struct Monitor_Guard :public Exc::MonitorRaw {
		WorkerForLocalUnwind* m_pWrk;
		virtual void AbnormalExit() {
			m_pWrk->UnwindLocalWithGuard(); // recursively
		}
	};

	Monitor_Guard mon;
	mon.m_pWrk = this;

	UnwindLocal(-1);

	// dismiss
	mon.NormalExit();
}

EXCEPTION_DISPOSITION WrapExcRecord::InvokeHandler(const ExcReg& excReg) const
{
	EXCEPTION_RECORD* pExc = m_pExc;
	CONTEXT* pCpuCtx = m_pCpuCtx;
	const ExcReg* pExcReg = &excReg;
	PVOID pfnHandler = excReg.m_pfnHandler;
	ASSERT(pfnHandler);

	EXCEPTION_DISPOSITION retVal;

	ExcMon::s_bExcAllowed = true;

	_asm
	{
		// There's some ambiguity here: Some handlers are actually __cdecl funcs, whereas
		// others are __stdcall. So we have no guarantee about the state of the stack.
		// Hence - we save it manually in ebx register (it remains valid across function call).

		CHKREG_PROLOG_NO_EBX

		mov ebx, esp	// save esp

		push 0
		push pCpuCtx
		push pExcReg
		push pExc
		call pfnHandler

		mov esp, ebx	// restore esp
		mov retVal, eax

		CHKREG_EPILOG_NO_EBX
	}

	ExcMon::s_bExcAllowed = false;

	return retVal;
}

void WrapExcRecord::RaiseExcRaw(ExcReg* pExcReg) const throw(...)
{
	const EXCEPTION_RECORD* pExc = m_pExc;

	for ( ; ; pExcReg = pExcReg->m_pPrev)
	{
		if (!pExcReg->IsValid())
			Unhandled();

		if (ExceptionContinueExecution == InvokeHandler(*pExcReg))
		{
			Verify(!(EXCEPTION_NONCONTINUABLE & m_pExc->ExceptionFlags)); // stupid EH handler?
			break;
		}
	}
}

void WrapExcRecord::Unhandled() const
{
	/*EXCEPTION_POINTERS excPt;
	excPt.ExceptionRecord = m_pExc;
	excPt.ContextRecord = m_pCpuCtx;
	UnhandledExceptionFilter(&excPt);*/
}

void WorkerForAll::UnwindUntil(const FrameInfo::Try& tryEntry)
{
	// Global unwind
	EXCEPTION_RECORD exc;
	exc.ExceptionCode		= 0;
	exc.ExceptionFlags		= EXC_FLAG_UNWIND;
	exc.ExceptionRecord		= NULL; // ???
	exc.ExceptionAddress	= 0;
	exc.NumberParameters	= 0;

	static CONTEXT ctx = { 0 };

	WrapExcRecord wrk;
	wrk.m_pExc = &exc;
	wrk.m_pCpuCtx = &ctx;

	ExcReg* pParent = NULL;
	for (ExcReg* pExcReg = ExcReg::get_Top(); m_pExcRegCpp != pExcReg; )
	{
		Verify(pExcReg > pParent); // stack corruption test

		ASSERT(pExcReg->IsValid());
		ExcReg* pPrev = pExcReg->m_pPrev;

		if (pExcReg->m_pfnHandler == ExcMon::Handler)
			pParent = pExcReg;
		else
		{
			// dismiss it before calling the handler. This is the convention.
			if (pParent)
				pParent->m_pPrev = pPrev;
			else
				pPrev->put_Top();

			wrk.InvokeHandler(*pExcReg);
		}

		pExcReg = pPrev;
	}

	// Local unwind
	UnwindLocal(tryEntry.m_IdStart - 1);
}

void WorkerForAll::AssignCatchObj(const FrameInfo::Try::Catch& catchEntry, const ThrownType::Sub& typeSub)
{
	ASSERT(m_pThrownType);

	if (catchEntry.m_Offset)
	{
		ULONG pDst = m_nEbp + catchEntry.m_Offset;
		PVOID pObj = m_pThrownObj;

		// copy exc there
		switch (8 & catchEntry.m_ValOrRef)
		{
		case 8: // by ref
			ASSERT(!(pDst & 3)); // should be DWORD-aligned, don't care if it's not
			*((PVOID*) pDst) = pObj;
			break;

		case 0: // by val
			{
				PVOID pfnCtor = typeSub.m_pfnCopyCtor;
				if (pfnCtor)
				{
					ExcMon::s_bExcAllowed = true;

					_asm
					{
						mov  ecx, pDst	// this

						CHKREG_PROLOG

						push pObj	// arg
						call pfnCtor

						CHKREG_EPILOG
					}

					ExcMon::s_bExcAllowed = false;
				}
				else
					memcpy(PVOID(pDst), pObj, typeSub.m_nSize);
			}
		}
	}
}

DWORD WorkerForAll::CallCatchRaw(const FrameInfo::Try::Catch& catchEntry)
{
	DWORD dwAddr = catchEntry.m_Catchblock_addr;
	ASSERT(dwAddr);

	DWORD dwEbp = m_nEbp;

	ExcMon::s_bExcAllowed = true;

	_asm
	{
		mov eax, dwAddr
		mov ebx, ebp	// save ebp

		CHKREG_PROLOG

		mov ebp, dwEbp

		// 'Catch' doesn't save esi, edi
		push esi
		push edi

		call eax

		pop edi
		pop esi
		mov ebp, ebx	// restore ebp

		CHKREG_EPILOG

		mov dwAddr, eax // return value (eax) is the address where to pass control to
	}

	ExcMon::s_bExcAllowed = false;

	return dwAddr;
}

__declspec(noreturn) void WorkerForAll::PassCtlAfterCatch(DWORD dwAddr)
{
	ExcMon::s_bExcAllowed = true;

	ASSERT(dwAddr);
	ULONG nEbp = m_nEbp;
	_asm
	{
		mov eax, dwAddr
		mov ebp, nEbp
		mov esp, [ebp-10h] // ?!?
		jmp eax
	}
}

void WorkerForAll::DestroyThrownObjSafe()
{
	if (m_pThrownType && m_pThrownType->m_pfnDtor)
	{
		ASSERT(m_pThrownObj);

		PVOID pObj = m_pThrownObj;
		PVOID pfnDtor = m_pThrownType->m_pfnDtor;

		ExcMon::s_bExcAllowed = true;

		_asm
		{
			mov ecx, pObj

			CHKREG_PROLOG
			call pfnDtor
			CHKREG_EPILOG
		}

		ExcMon::s_bExcAllowed = false;
	}
}

void WorkerForAll::Catch()
{
	long nTryCount = m_pFrame->m_Try.m_nCount;
	if (nTryCount > 0)
	{
		const DWORD dwTryID = m_pExcRegCpp->m_dwTryID;
		const FrameInfo::Try* pTryTable = m_pFrame->m_Try.m_pArr;
		ASSERT(pTryTable);

		ExcMon* const pMon = ExcMon::s_pInst;
		ASSERT(pMon);

		// find the enclosing try block
		long nTry = 0;
		do
		{
			const FrameInfo::Try& tryEntry = m_pFrame->m_Try.m_pArr[nTry];
			long nCatchCount = tryEntry.m_Catch.m_nCount;

			if ((dwTryID >= tryEntry.m_IdStart) &&
				(dwTryID <= tryEntry.m_IdEnd) &&
				(nCatchCount > 0))
			{
				// find the appropriate catch block
				const FrameInfo::Try::Catch* pCatch = tryEntry.m_Catch.m_pArr;
				ASSERT(pCatch);

				long nCatch = 0;
				do {

					for (EXCEPTION_RECORD** ppExc = &pMon->m_pExc; ; )
					{
						EXCEPTION_RECORD* pExc = *ppExc;
						ASSERT(pExc);

						DWORD dwAddr = CatchInternal(tryEntry, pCatch[nCatch], pExc);
						if (dwAddr)
						{
							if (!pMon->m_pExc->ExceptionRecord)
							{
								// finito
								pMon->NormalExit();
								PassCtlAfterCatch(dwAddr);
							}

							*ppExc = pExc->ExceptionRecord;

						} else
							ppExc = &pExc->ExceptionRecord;

						if (! *ppExc)
							break;
					}

				} while (++nCatch < nCatchCount);
			}

		} while (++nTry < nTryCount);
	}
}

DWORD WorkerForAll::CatchInternal(const FrameInfo::Try& tryEntry, const FrameInfo::Try::Catch& catchEntry, EXCEPTION_RECORD* pExc)
{
	// get the C++ exception info
	if ((0xe06d7363 == pExc->ExceptionCode) &&
		(3 == pExc->NumberParameters))
	{
		m_pThrownObj	= (PVOID) pExc->ExceptionInformation[1];
		m_pThrownType	= (ThrownType*) pExc->ExceptionInformation[2];

		ASSERT(
			m_pThrownObj &&
			m_pThrownType &&
			m_pThrownType->m_pTable &&
			(m_pThrownType->m_pTable->m_nCount > 0));

		long nSub = 0, nSubTotal = m_pThrownType->m_pTable->m_nCount;
		ThrownType::Sub** ppSub = m_pThrownType->m_pTable->m_pArr;

		do {
			ThrownType::Sub* pSub = ppSub[nSub];
			ASSERT(pSub && NULL != pSub->ti);
		} while (++nSub < nSubTotal);

	} else
		m_pThrownType = NULL;

	if (catchEntry.m_ti)
	{
		if (m_pThrownType)
		{
			// Check if this type can be cast to the dst one
			long nSub = 0, nSubTotal = m_pThrownType->m_pTable->m_nCount;
			ThrownType::Sub** ppTypeSub = m_pThrownType->m_pTable->m_pArr;

			do
			{
				const ThrownType::Sub& typeSub = *(ppTypeSub[nSub]);
				if (*catchEntry.m_ti == *typeSub.ti)
				{
					UnwindUntil(tryEntry);
					AssignCatchObj(catchEntry, typeSub);
					return CallCatchDestroyObj(catchEntry);
				}

			} while (++nSub < nSubTotal);
		}

	} else
	{
		UnwindUntil(tryEntry);
		ASSERT(!catchEntry.m_Offset);
		return CallCatchDestroyObj(catchEntry);
	}

	return 0;
}

DWORD WorkerForAll::CallCatchDestroyObj(const FrameInfo::Try::Catch& catchEntry)
{
	DWORD dwAddr = CallCatchRaw(catchEntry);

	m_pExcRegCpp->m_dwTryID++; // if exc is re-raised now - give this try block opportunity to handle it
	DestroyThrownObjSafe();

	return dwAddr;
}

EXCEPTION_DISPOSITION __thiscall FrameInfo::Handler(EXCEPTION_RECORD* pExc, ExcRegCpp* pExcRegCpp, CONTEXT* pCpuCtx)
{
	ExcMon::s_bExcAllowed = false;

	ASSERT(this && pExc && pExcRegCpp && pCpuCtx);

	WorkerForAll wrk;
	wrk.m_pFrame = this;
	wrk.m_pExcRegCpp = pExcRegCpp;
	wrk.m_nEbp = (ULONG) (ULONG_PTR) &pExcRegCpp->m_nEbpPlaceholder;

	if (EXC_FLAG_UNWIND & pExc->ExceptionFlags)
		wrk.UnwindLocalWithGuard();
	else
	{
		ExcMon* const pMon = ExcMon::s_pInst;
		if (pMon)
		{
			if (pMon->m_pExc != pExc)
				pMon->AppendExc(pExc);

			wrk.Catch();

		} else
		{
			// probably a SEH exception
			ExcMon excMon(pExc, pCpuCtx);

			wrk.Catch();

			excMon.RaiseExcRaw(pExcRegCpp->m_pPrev);
			excMon.NormalExit();

			ExcMon::s_bExcAllowed = true;
			return ExceptionContinueExecution;
		}
	}

	ExcMon::s_bExcAllowed = true;
	return ExceptionContinueSearch;
}

extern "C" EXCEPTION_DISPOSITION __cdecl __CxxFrameHandler3(int a, int b, int c, int d);
extern "C" void __stdcall _CxxThrowException(void* pObj, _s__ThrowInfo const* pType);


__declspec (naked)
EXCEPTION_DISPOSITION __cdecl V__CxxFrameHandler3(int a, int b, int c, int d)
{
	_asm {
		mov ecx, eax
		jmp FrameInfo::Handler
	};
}

void __stdcall V_CxxThrowException(void* pObj, _s__ThrowInfo const* pType)
{
	if (pObj && pType)
	{
		EXCEPTION_RECORD exc;
		exc.ExceptionCode		= 0xe06d7363;
		exc.ExceptionFlags		= EXCEPTION_NONCONTINUABLE;
		exc.ExceptionRecord		= NULL; // nested exc
		exc.NumberParameters	= 3;
		exc.ExceptionAddress	= _CxxThrowException;

		exc.ExceptionInformation[0]	= 0x19930520;
		exc.ExceptionInformation[1]	= ULONG_PTR(pObj);
		exc.ExceptionInformation[2]	= ULONG_PTR(pType);

		Exc::RaiseExc(exc);

	} else
	{
		// rethrow
		ExcMon::s_bExcAllowed = false;

		ExcMon* const pMon = ExcMon::s_pInst;
		Verify(NULL != pMon); // illegal rethrow?
		pMon->RaiseExcRaw(ExcReg::get_Top());
	}
}

bool __cdecl V_uncaught_exception()
{
	return NULL != ExcMon::s_pInst;
}

namespace Exc
{
#pragma pack(1)
	struct JumpOffset {
		UCHAR	m_JumpCmd;
		DWORD	m_dwOffset;
	};
#pragma pack()

	struct FunctionHack {
		bool		m_bHacked;
		JumpOffset	m_FuncBody;

		void Hack(bool bSet, PVOID pfnOrg, PVOID pfnNew)
		{
			/*if (m_bHacked != bSet)
			{
				DWORD dwProtectionOld = 0;
				VirtualProtect(pfnOrg, sizeof(m_FuncBody), PAGE_READWRITE, &dwProtectionOld);

				if (bSet)
				{

					CopyMemory(&m_FuncBody, pfnOrg, sizeof(m_FuncBody));

					JumpOffset jump;
					jump.m_JumpCmd	= 0xE9;
					jump.m_dwOffset	= DWORD(pfnNew) - DWORD(pfnOrg) - sizeof(jump);

					CopyMemory(pfnOrg, &jump, sizeof(jump));

				} else
					CopyMemory(pfnOrg, &m_FuncBody, sizeof(m_FuncBody));

				m_bHacked = bSet;

				VirtualProtect(pfnOrg, sizeof(m_FuncBody), dwProtectionOld, &dwProtectionOld);
			}*/
			
			if (m_bHacked != bSet)
			{
			
				if (bSet)
				{

					memcpy(&m_FuncBody, pfnOrg, sizeof(m_FuncBody));

					JumpOffset jump;
					jump.m_JumpCmd	= 0xE9;
					jump.m_dwOffset	= DWORD(pfnNew) - DWORD(pfnOrg) - sizeof(jump);

					memcpy(pfnOrg, &jump, sizeof(jump));

				} else
					memcpy(pfnOrg, &m_FuncBody, sizeof(m_FuncBody));

				m_bHacked = bSet;				
			}

		}
	};

	void SetFrameHandler(bool bSet)
	{
		static FunctionHack hack = { 0 };
		hack.Hack(bSet, __CxxFrameHandler3, V__CxxFrameHandler3);
	}

	void SetThrowFunction(bool bSet)
	{
		static FunctionHack hack = { 0 };
		hack.Hack(bSet, _CxxThrowException, V_CxxThrowException);
	}

	void SetUncaughtExc(bool bSet)
	{
		//static FunctionHack hack = { 0 };
		//hack.Hack(bSet, std::uncaught_exception, V_uncaught_exception);
	}

	void RaiseExc(EXCEPTION_RECORD& exc, CONTEXT* pCtx) throw (...)
	{
		ExcMon::s_bExcAllowed = false;

		static CONTEXT ctx = { 0 };

		if (!pCtx)
			pCtx = &ctx;

		ExcMon* const pMon = ExcMon::s_pInst;
		if (pMon)
		{
			pMon->AppendExc(&exc);
			pMon->m_pCpuCtx = pCtx;
			pMon->RaiseExcRaw(ExcReg::get_Top());

		} else
		{
			ExcMon mon(&exc, pCtx);

			mon.RaiseExcRaw(mon.m_pPrev);
			mon.NormalExit();
		}
	}

	EXCEPTION_RECORD* GetCurrentExc()
	{
		return ExcMon::s_pInst ? ExcMon::s_pInst->m_pExc : NULL;
	}
}

