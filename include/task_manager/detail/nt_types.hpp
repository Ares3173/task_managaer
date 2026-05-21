// /include/task_manager/detail/nt_types.hpp
// NOLINTBEGIN
#pragma once

#include <cstdint>

namespace task_manager::detail::nt {

using DWORD64 = std::uint64_t;
using DWORD   = std::uint32_t;
using WORD    = std::uint16_t;
using BYTE    = std::uint8_t;

enum class PROCESSINFOCLASS : unsigned int {
	ProcessBasicInformation           = 0,
	ProcessQuotaLimits                = 1,
	ProcessIoCounters                 = 2,
	ProcessVmCounters                 = 3,
	ProcessTimes                      = 4,
	ProcessBasePriority               = 5,
	ProcessRaisePriority              = 6,
	ProcessDebugPort                  = 7,
	ProcessExceptionPort              = 8,
	ProcessAccessToken                = 9,
	ProcessLdrInformation             = 10,
	ProcessLdtSize                    = 11,
	ProcessDefaultHardErrorMode       = 12,
	ProcessIoPortHandlers             = 13,
	ProcessPooledUsageAndLimits       = 14,
	ProcessWorkingSetWatch            = 15,
	ProcessUserModeIOPL               = 16,
	ProcessEnableAlignmentFaultFixup  = 17,
	ProcessPriorityClass              = 18,
	ProcessWx86Information            = 19,
	ProcessHandleCount                = 20,
	ProcessAffinityMask               = 21,
	ProcessPriorityBoost              = 22,
	ProcessDeviceMap                  = 23,
	ProcessSessionInformation         = 24,
	ProcessForegroundInformation      = 25,
	ProcessWow64Information           = 26,
	ProcessImageFileName              = 27,
	ProcessLUIDDeviceMapsEnabled      = 28,
	ProcessBreakOnTermination         = 29,
	ProcessDebugObjectHandle          = 30,
	ProcessDebugFlags                 = 31,
	ProcessHandleTracing              = 32,
	ProcessIoPriority                 = 33,
	ProcessExecuteFlags               = 34,
	ProcessTlsInformation             = 35,
	ProcessCookie                     = 36,
	ProcessImageInformation           = 37,
	ProcessCycleTime                  = 38,
	ProcessPagePriority               = 39,
	ProcessInstrumentationCallback    = 40,
	ProcessThreadStackAllocation      = 41,
	ProcessWorkingSetWatchEx          = 42,
	ProcessImageFileNameWin32         = 43,
	ProcessImageFileMapping           = 44,
	ProcessAffinityUpdateMode         = 45,
	ProcessMemoryAllocationMode       = 46,
	ProcessGroupInformation           = 47,
	ProcessTokenVirtualizationEnabled = 48,
	ProcessConsoleHostProcess         = 49,
	ProcessWindowInformation          = 50,
	ProcessHandleInformation          = 51,
	MaxProcessInfoClass               = 52
};

enum class JOBOBJECTINFOCLASS : unsigned int {
	JobObjectBasicAccountingInformation         = 1,
	JobObjectBasicLimitInformation              = 2,
	JobObjectBasicProcessIdList                 = 3,
	JobObjectBasicUIRestrictions                = 4,
	JobObjectSecurityLimitInformation           = 5, // deprecated
	JobObjectEndOfJobTimeInformation            = 6,
	JobObjectAssociateCompletionPortInformation = 7,
	JobObjectBasicAndIoAccountingInformation    = 8,
	JobObjectExtendedLimitInformation           = 9,
	JobObjectJobSetInformation                  = 10,
	JobObjectGroupInformation                   = 11,
	JobObjectNotificationLimitInformation       = 12,
	JobObjectLimitViolationInformation          = 13,
	JobObjectGroupInformationEx                 = 14,
	JobObjectCpuRateControlInformation          = 15,
	JobObjectCompletionFilter                   = 16,
	JobObjectCompletionCounter                  = 17,

	//
	//

	JobObjectFreezeInformation              = 18,
	JobObjectExtendedAccountingInformation  = 19,
	JobObjectWakeInformation                = 20,
	JobObjectBackgroundInformation          = 21,
	JobObjectSchedulingRankBiasInformation  = 22,
	JobObjectTimerVirtualizationInformation = 23,
	JobObjectCycleTimeNotification          = 24,
	JobObjectClearEvent                     = 25,
	JobObjectInterferenceInformation        = 26,
	JobObjectClearPeakJobMemoryUsed         = 27,
	JobObjectMemoryUsageInformation         = 28,
	JobObjectSharedCommit                   = 29,
	JobObjectContainerId                    = 30,
	JobObjectIoRateControlInformation       = 31,
	JobObjectNetRateControlInformation      = 32,
	JobObjectNotificationLimitInformation2  = 33,
	JobObjectLimitViolationInformation2     = 34,
	JobObjectCreateSilo                     = 35,
	JobObjectSiloBasicInformation           = 36,
	JobObjectReserved15Information          = 37,
	JobObjectReserved16Information          = 38,
	JobObjectReserved17Information          = 39,
	JobObjectReserved18Information          = 40,
	JobObjectReserved19Information          = 41,
	JobObjectReserved20Information          = 42,
	JobObjectReserved21Information          = 43,
	JobObjectReserved22Information          = 44,
	JobObjectReserved23Information          = 45,
	JobObjectReserved24Information          = 46,
	JobObjectReserved25Information          = 47,
	JobObjectReserved26Information          = 48,
	JobObjectReserved27Information          = 49,
	MaxJobObjectInfoClass                   = 50
};
#pragma warning( push )
#pragma warning( disable : 4201 )

struct _JOBOBJECT_WAKE_FILTER {
	unsigned long HighEdgeFilter;
	unsigned long LowEdgeFilter;
};
using JOBOBJECT_WAKE_FILTER  = _JOBOBJECT_WAKE_FILTER;
using PJOBOBJECT_WAKE_FILTER = _JOBOBJECT_WAKE_FILTER*;

// private
struct _JOBOBJECT_FREEZE_INFORMATION {
	union {
		unsigned long Flags;
		struct {
			unsigned long FreezeOperation : 1;
			unsigned long FilterOperation : 1;
			unsigned long SwapOperation : 1;
			unsigned long Reserved : 29;
		};
	};
	bool Freeze;
	bool Swap;
	bool Reserved0[ 2 ];
	JOBOBJECT_WAKE_FILTER WakeFilter;
};
#pragma warning( pop )

using JOBOBJECT_FREEZE_INFORMATION  = _JOBOBJECT_FREEZE_INFORMATION;
using PJOBOBJECT_FREEZE_INFORMATION = _JOBOBJECT_FREEZE_INFORMATION*;

struct alignas( 16 ) M128A {
	std::uint64_t Low;
	std::int64_t High;
};

struct XMM_SAVE_AREA32 {
	WORD ControlWord;
	WORD StatusWord;
	BYTE TagWord;
	BYTE Reserved1;
	WORD ErrorOpcode;
	DWORD ErrorOffset;
	WORD ErrorSelector;
	WORD Reserved2;
	DWORD DataOffset;
	WORD DataSelector;
	WORD Reserved3;
	DWORD MxCsr;
	DWORD MxCsr_Mask;
	M128A FloatRegisters[ 8 ];
	M128A XmmRegisters[ 16 ];
	BYTE Reserved4[ 96 ];
};

typedef struct alignas( 16 ) _CONTEXT {

	//
	// Register parameter home addresses.
	//
	// N.B. These fields are for convience - they could be used to extend the
	//      context record in the future.
	//

	DWORD64 P1Home;
	DWORD64 P2Home;
	DWORD64 P3Home;
	DWORD64 P4Home;
	DWORD64 P5Home;
	DWORD64 P6Home;

	//
	// Control flags.
	//

	DWORD ContextFlags;
	DWORD MxCsr;

	//
	// Segment Registers and processor flags.
	//

	WORD SegCs;
	WORD SegDs;
	WORD SegEs;
	WORD SegFs;
	WORD SegGs;
	WORD SegSs;
	DWORD EFlags;

	//
	// Debug registers
	//

	DWORD64 Dr0;
	DWORD64 Dr1;
	DWORD64 Dr2;
	DWORD64 Dr3;
	DWORD64 Dr6;
	DWORD64 Dr7;

	//
	// Integer registers.
	//

	DWORD64 Rax;
	DWORD64 Rcx;
	DWORD64 Rdx;
	DWORD64 Rbx;
	DWORD64 Rsp;
	DWORD64 Rbp;
	DWORD64 Rsi;
	DWORD64 Rdi;
	DWORD64 R8;
	DWORD64 R9;
	DWORD64 R10;
	DWORD64 R11;
	DWORD64 R12;
	DWORD64 R13;
	DWORD64 R14;
	DWORD64 R15;

	//
	// Program counter.
	//

	DWORD64 Rip;

	//
	// Floating point state.
	//

	union {
		XMM_SAVE_AREA32 FltSave;
		struct {
			M128A Header[ 2 ];
			M128A Legacy[ 8 ];
			M128A Xmm0;
			M128A Xmm1;
			M128A Xmm2;
			M128A Xmm3;
			M128A Xmm4;
			M128A Xmm5;
			M128A Xmm6;
			M128A Xmm7;
			M128A Xmm8;
			M128A Xmm9;
			M128A Xmm10;
			M128A Xmm11;
			M128A Xmm12;
			M128A Xmm13;
			M128A Xmm14;
			M128A Xmm15;
		} DUMMYSTRUCTNAME;
	} DUMMYUNIONNAME;

	//
	// Vector registers.
	//

	M128A VectorRegister[ 26 ];
	DWORD64 VectorControl;

	//
	// Special debug control registers.
	//

	DWORD64 DebugControl;
	DWORD64 LastBranchToRip;
	DWORD64 LastBranchFromRip;
	DWORD64 LastExceptionToRip;
	DWORD64 LastExceptionFromRip;
} CONTEXT, *PCONTEXT;

} // namespace task_manager::detail::nt
// NOLINTEND
