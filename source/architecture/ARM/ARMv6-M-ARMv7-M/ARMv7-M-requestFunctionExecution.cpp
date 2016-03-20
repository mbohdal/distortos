/**
 * \file
 * \brief requestFunctionExecution() implementation for ARMv7-M (Cortex-M3 / Cortex-M4)
 *
 * \author Copyright (C) 2015-2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/architecture/requestFunctionExecution.hpp"

#include "distortos/chip/CMSIS-proxy.h"

#if __FPU_PRESENT == 1 && __FPU_USED == 1
#include "ARMv7-M-ExceptionFpuStackFrame.hpp"
#endif	// __FPU_PRESENT == 1 && __FPU_USED == 1

#include "ARMv6-M-ARMv7-M-StackFrame.hpp"
#include "supervisorCall.hpp"

#include "distortos/internal/scheduler/Scheduler.hpp"
#include "distortos/internal/scheduler/getScheduler.hpp"

#include <cstring>

namespace distortos
{

namespace architecture
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Removes stack frame inserted by requestFunctionExecution() in case of interrupt -> current thread request.
 *
 * \param [in] savedStackPointer is the stack pointer value before new stack frame was created
 * \param [in] fpuContextActive (only when FPU is enabled) tells whether thread has active FPU context (true) or not
 * (false)
 */

#if __FPU_PRESENT == 1 && __FPU_USED == 1
void removeStackFrame(const void* const savedStackPointer, const bool fpuContextActive)
#else	// __FPU_PRESENT != 1 || __FPU_USED != 1
void removeStackFrame(const void* const savedStackPointer)
#endif	// __FPU_PRESENT != 1 || __FPU_USED != 1
{
#if __FPU_PRESENT == 1 && __FPU_USED == 1

	if (fpuContextActive == true)
		asm volatile ("vmov s0, s0");	// force stacking of FPU context

#endif	// __FPU_PRESENT == 1 && __FPU_USED == 1

	__set_PSP(reinterpret_cast<uint32_t>(savedStackPointer));
}

/**
 * \brief Trampoline used to execute function on new stack frame.
 *
 * After the function returns stack frame is removed and old stack pointer value is restored.
 *
 * \param [in] function is a reference to function that will be executed
 * \param [in] savedStackPointer is the stack pointer value before new stack frame was created
 * \param [in] fullContext is an information about type of previous stack frame - "full", when the thread was not
 * running (true) or "interrupt", when the thread was running and was interrupted (false)
 */

__attribute__ ((naked))
void functionTrampoline(void (& function)(), const void* const savedStackPointer, const bool fullContext)
{
	using SupervisorCall = int(int (&)(int, int, int, int), int, int, int, int);	// type of supervisorCall()

#ifdef __ARM_ARCH_6M__

	asm volatile
	(
			"	push		{r1-r2}						\n"	// push last two arguments to stack
			"	blx			%[function]					\n"	// execute function
			"	pop			{r1-r2}						\n"	// restore last two arguments
			"	cmp			r2, #0						\n"
			"	beq			1f							\n"	// if (fullContext == true) {
			"	ldmia		r1!, {r4-r7}				\n"	// load upper half of thread's context
			"	mov			r8, r4						\n"
			"	mov			r9, r5						\n"
			"	mov			r10, r6						\n"
			"	mov			r11, r7						\n"
			"	ldmia		r1!, {r4-r7}				\n"	// load lower half of thread's context
			"	mov			r2, #0						\n"	// 3rd supervisorCall() argument - 0
			"1:											\n"	// }
			"	ldr			r0, =%[removeStackFrame]	\n"	// 1st supervisorCall() argument - removeStackFrame
			"	ldr			r3, =%[supervisorCall]		\n"
			"	mov			r12, r3						\n"
			"	mov			r3, #0						\n"	// 4th supervisorCall() argument - 0
			"	sub			sp, #8						\n"
			"	str			r3, [sp]					\n"	// 5th supervisorCall() argument - 0
			"	bx			r12							\n"	// jump to supervisorCall(), this does not return

			::	[function] "r" (function),
				[savedStackPointer] "r" (savedStackPointer),
				[fullContext] "r" (fullContext),
				[removeStackFrame] "i" (removeStackFrame),
				[supervisorCall] "i" (static_cast<SupervisorCall&>(supervisorCall))
	);

#else	// !def __ARM_ARCH_6M__

	asm volatile
	(
#if __FPU_PRESENT == 1 && __FPU_USED == 1
			"	mrs			r3, CONTROL					\n"	// save current value of CONTROL register
			"	push		{r0-r3}						\n"	// push all arguments and value of CONTROL register to stack
#else	// __FPU_PRESENT != 1 || __FPU_USED != 1
			"	push		{r1-r2}						\n"	// push last two arguments to stack
#endif	// __FPU_PRESENT != 1 || __FPU_USED != 1
			"	blx			%[function]					\n"	// execute function
#if __FPU_PRESENT == 1 && __FPU_USED == 1
			// restore all arguments and value of CONTROL register, don't update SP
			"	ldm			sp, {r0-r3}					\n"
			// restore previous value of CONTROL register, possibly deactivating FPU context
			"	msr			CONTROL, r3					\n"
#else	// __FPU_PRESENT != 1 || __FPU_USED != 1
			"	ldm			sp, {r1-r2}					\n"	// restore last two arguments, don't update SP
#endif	// __FPU_PRESENT != 1 || __FPU_USED != 1
			"	cmp			r2, #0						\n"
#if __FPU_PRESENT == 1 && __FPU_USED == 1
			"	itt			eq							\n"	// if (fullContext == false) {
			// 3rd supervisorCall() argument - extracted CONTROL.FPCA, 1 if FPU context is active, 0 otherwise
			"	ubfxeq		r2, r3, #2, #1				\n"
			"	beq			1f							\n"	// } else {
			"	ldmia		r1!, {r4-r12, lr}			\n"	// load "regular" context of thread
			"	ubfx		r2, lr, #4, #1				\n"	// was floating-point used by the thread?
			// 3rd supervisorCall() argument - 1 if FPU context is active, 0 otherwise
			"	eors		r2, #(1 << 0)				\n"
			"	it			ne							\n"
			"	vldmiane	r1!, {s16-s31}				\n"	// load "floating-point" context of thread
			"1:											\n"	// }
#else	// __FPU_PRESENT != 1 || __FPU_USED != 1
			"	itt			ne							\n"	// if (fullContext == true) {
			"	ldmiane		r1!, {r4-r11}				\n"	// load context of thread
			"	movne		r2, #0						\n"	// 3rd supervisorCall() argument - 0
			"											\n"	// }
#endif	// __FPU_PRESENT != 1 || __FPU_USED != 1
			"	mov			r3, #0						\n"	// 4th supervisorCall() argument - 0
			"	str			r3, [sp]					\n"	// 5th supervisorCall() argument - 0
			"	ldr			r0, =%[removeStackFrame]	\n"	// 1st supervisorCall() argument - removeStackFrame
			"	b			%[supervisorCall]			\n"	// jump to supervisorCall(), this does not return

			::	[function] "r" (function),
				[savedStackPointer] "r" (savedStackPointer),
				[fullContext] "r" (fullContext),
				[removeStackFrame] "i" (removeStackFrame),
				[supervisorCall] "i" (static_cast<SupervisorCall&>(supervisorCall))
	);

#endif	// !def __ARM_ARCH_6M__

	__builtin_unreachable();
}

/**
 * \brief Handles request from current thread to itself.
 *
 * \param [in] function is a reference to function that should be executed in current thread
 */

void fromCurrentThreadToCurrentThread(void (& function)())
{
#if __FPU_PRESENT == 1 && __FPU_USED == 1
	const auto control = __get_CONTROL();
#endif	// __FPU_PRESENT == 1 && __FPU_USED == 1

	function();	// execute function right away

#if __FPU_PRESENT == 1 && __FPU_USED == 1
	__set_CONTROL(control);	// restore previous value of CONTROL register, possibly deactivating FPU context
#endif	// __FPU_PRESENT == 1 && __FPU_USED == 1
}

/**
 * \brief Handles request coming from interrupt context to execute provided function in current thread.
 *
 * \param [in] function is a reference to function that should be executed in current thread
 */

void fromInterruptToCurrentThread(void (& function)())
{
	const auto stackPointer = __get_PSP();

#if __FPU_PRESENT == 1 && __FPU_USED == 1

	// it's not possible to know whether the thread has active FPU context, so the only option is to assume it does
	const auto exceptionFpuStackFrame = reinterpret_cast<ExceptionFpuStackFrame*>(stackPointer) - 1;
	const auto exceptionStackFrame = &exceptionFpuStackFrame->exceptionStackFrame;

	const auto fpccr = FPU->FPCCR;
	// last FPU stack frame was allocated in thread mode and the stacking is still pending?
	// this condition will be false in following situations:
	// - thread doesn't use FPU - there was no FPU stack frame allocated in thread mode,
	// - thread uses FPU, but the registers are already stacked.
	if ((fpccr & FPU_FPCCR_THREAD_Msk) != 0 && (fpccr & FPU_FPCCR_LSPACT_Msk) != 0)
		asm volatile ("vmov s0, s0");	// force stacking of FPU context

	memset(exceptionFpuStackFrame, 0, sizeof(*exceptionFpuStackFrame));
	exceptionFpuStackFrame->fpscr = reinterpret_cast<void*>(FPU->FPDSCR);

#else	// __FPU_PRESENT != 1 || __FPU_USED != 1

	const auto exceptionStackFrame = reinterpret_cast<ExceptionStackFrame*>(stackPointer) - 1;

#endif	// __FPU_PRESENT != 1 || __FPU_USED != 1

	exceptionStackFrame->r0 = reinterpret_cast<void*>(&function);
	exceptionStackFrame->r1 = reinterpret_cast<void*>(stackPointer);
	exceptionStackFrame->r2 = reinterpret_cast<void*>(false);
	exceptionStackFrame->r3 = reinterpret_cast<void*>(0x33333333);
	exceptionStackFrame->r12 = reinterpret_cast<void*>(0xcccccccc);
	exceptionStackFrame->lr = nullptr;
	exceptionStackFrame->pc = reinterpret_cast<void*>(&functionTrampoline);
	exceptionStackFrame->xpsr = ExceptionStackFrame::defaultXpsr;

	__set_PSP(reinterpret_cast<uint32_t>(exceptionStackFrame));
}

/**
 * \brief Handles request to execute provided function in non-current thread.
 *
 * \param [in] threadControlBlock is a reference to internal::ThreadControlBlock of thread in which \a function should
 * be executed
 * \param [in] function is a reference to function that should be executed in thread associated with
 * \a threadControlBlock
 */

void toNonCurrentThread(internal::ThreadControlBlock& threadControlBlock, void (& function)())
{
	const auto stackPointer = threadControlBlock.getStack().getStackPointer();
	const auto stackFrame = reinterpret_cast<StackFrame*>(stackPointer) - 1;

	stackFrame->softwareStackFrame.r4 = reinterpret_cast<void*>(0x44444444);
	stackFrame->softwareStackFrame.r5 = reinterpret_cast<void*>(0x55555555);
	stackFrame->softwareStackFrame.r6 = reinterpret_cast<void*>(0x66666666);
	stackFrame->softwareStackFrame.r7 = reinterpret_cast<void*>(0x77777777);
	stackFrame->softwareStackFrame.r8 = reinterpret_cast<void*>(0x88888888);
	stackFrame->softwareStackFrame.r9 = reinterpret_cast<void*>(0x99999999);
	stackFrame->softwareStackFrame.r10 = reinterpret_cast<void*>(0xaaaaaaaa);
	stackFrame->softwareStackFrame.r11 = reinterpret_cast<void*>(0xbbbbbbbb);
#if __FPU_PRESENT == 1 && __FPU_USED == 1
	stackFrame->softwareStackFrame.exceptionReturn = SoftwareStackFrame::defaultExceptionReturn;
#endif	// __FPU_PRESENT == 1 && __FPU_USED == 1

	stackFrame->exceptionStackFrame.r0 = reinterpret_cast<void*>(&function);
	stackFrame->exceptionStackFrame.r1 = reinterpret_cast<void*>(stackPointer);
	stackFrame->exceptionStackFrame.r2 = reinterpret_cast<void*>(true);
	stackFrame->exceptionStackFrame.r3 = reinterpret_cast<void*>(0x33333333);
	stackFrame->exceptionStackFrame.r12 = reinterpret_cast<void*>(0xcccccccc);
	stackFrame->exceptionStackFrame.lr = nullptr;
	stackFrame->exceptionStackFrame.pc = reinterpret_cast<void*>(&functionTrampoline);
	stackFrame->exceptionStackFrame.xpsr = ExceptionStackFrame::defaultXpsr;

	threadControlBlock.getStack().setStackPointer(stackFrame);
}

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

void requestFunctionExecution(internal::ThreadControlBlock& threadControlBlock, void (& function)())
{
	const auto& currentThreadControlBlock = internal::getScheduler().getCurrentThreadControlBlock();
	if (&threadControlBlock == &currentThreadControlBlock)	// request to current thread?
	{
		const auto inInterrupt = __get_IPSR() != 0;
		if (inInterrupt == false)	// current thread is sending the request to itself?
			fromCurrentThreadToCurrentThread(function);
		else						// interrupt is sending the request to current thread?
			fromInterruptToCurrentThread(function);
	}
	else	// request to non-current thread
		toNonCurrentThread(threadControlBlock, function);
}

}	// namespace architecture

}	// namespace distortos
