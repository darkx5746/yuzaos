// 
// Copyright 1998-2012 Jeff Bush
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// 

#include "cpu_asm.h"
#include "Scheduler.h"
#include "Semaphore.h"
#include <stringdef.h>
#include "Thread.h"
#include "Debugger.h"
#include "math.h"
#include <stringdef.h>
#include "intrinsic.h"
#include "systemcall.h"
#include "PlatformAPI.h"

const int kQuantum = 8000;

_Scheduler gScheduler;

_Scheduler::_Scheduler()
: fHighestReadyThread(0)
, fReadyThreadCount(0)
{
}

void _Scheduler::Reschedule()
{
	int st = DisableInterrupts();
	Thread *thread = Thread::GetRunningThread();
	if (thread->GetState() == kThreadRunning) 
	{
		if (fHighestReadyThread <= thread->GetCurrentPriority() && thread->GetQuantumUsed() < kQuantum) 
		{
			// This thread hasn't used its entire timeslice, and there
			// isn't a higher priority thread ready to run.  Don't reschedule.
			// Instead, continue running this thread.  Try to let the thread
			// use its entire timeslice whenever possible for better performance.
			RestoreInterrupts(st);
			return;
		}

		EnqueueReadyThread(thread);
	}

	CancelTimeout();		
	SetTimeout(SystemTime() + kQuantum, kOneShotTimer);
	
	while (1)
	{
		//���� ������ ���ð� ���ÿ� �ش� ������� ť���� ���ŵ�
		Thread* nextThread = PickNextThread();
		
		if (nextThread->GetState() == kThreadSuspended)
		{
			nextThread->SetCurrentPriority(0);
			EnqueueReadyThread(nextThread);
			continue;
		}

		nextThread->SwitchTo(thread);
		break;
	}

	RestoreInterrupts(st);
}

void _Scheduler::EnqueueReadyThread(Thread *thread)
{
	if (thread->GetState() == kThreadDead)
		return;

	int st = DisableInterrupts();
	if (thread->GetSleepTime() > kQuantum * 4) {
		// Boost this thread's priority if it has been blocked for a while
		if (thread->GetCurrentPriority() < MIN(kMaxPriority, thread->GetBasePriority() + 3))
			thread->SetCurrentPriority(thread->GetCurrentPriority() + 1);
	} else {
		// This thread has run for a while.  If it's priority was boosted,
		// lower it back down.
		if (thread->GetCurrentPriority() > thread->GetBasePriority())
			thread->SetCurrentPriority(thread->GetCurrentPriority() - 1);
	}
	
	if (thread->GetCurrentPriority() > fHighestReadyThread)
		fHighestReadyThread = thread->GetCurrentPriority();
			
	fReadyThreadCount++;

	if(thread->GetState() != kThreadSuspended)
		thread->SetState(kThreadReady);

	fReadyQueue[thread->GetCurrentPriority()].Enqueue(thread);
	
	RestoreInterrupts(st);
}

Thread *_Scheduler::PickNextThread() 
{		
	fReadyThreadCount--;
	Thread *nextThread = static_cast<Thread*>(fReadyQueue[fHighestReadyThread].Dequeue());
	while (fReadyQueue[fHighestReadyThread].GetHead() == 0)
		fHighestReadyThread--;

	ASSERT(nextThread);			
	return nextThread;
}

InterruptStatus _Scheduler::HandleTimeout()
{
	return InterruptStatus::kReschedule;
}
