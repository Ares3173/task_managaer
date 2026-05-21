// /include/task_manager/thread/context.hpp

#pragma once
#include "task_manager/error.hpp"
#include "task_manager/types.hpp"

#include <expected>

namespace task_manager {
namespace detail::nt {
struct _CONTEXT; // opaque Windows struct
using PCONTEXT = struct _CONTEXT*;
} // namespace detail::nt

class thread;

class context {
  public:
	// Flags
	struct flags;
	enum class context_flags : unsigned int;

	// Debug registers (x86/x64 only)
	struct dr6;
	struct dr7;
	struct debug_registers;

	// Integer registers (x86/x64 only)
	struct registers;
	enum class register_id : unsigned {
		rax,
		rcx,
		rdx,
		rbx,
		rsp,
		rbp,
		rsi,
		rdi,
		r8,
		r9,
		r10,
		r11,
		r12,
		r13,
		r14,
		r15,
		rip
	};

	static auto current() -> std::expected<context, errc>;
	static auto from_thread( const thread& t ) -> std::expected<context, errc>;
	static auto from_thread_id( tid_t tid ) -> std::expected<context, errc>;
	static auto from_thread_handle( void* handle ) -> std::expected<context, errc>;
	static auto from_ctx( detail::nt::PCONTEXT ctx ) noexcept -> context;

	context()                                = default;
	context( const context& )                = delete;
	context& operator=( const context& )     = delete;
	context( context&& ) noexcept            = default;
	context& operator=( context&& ) noexcept = default;
	~context()                               = default;

	context( detail::nt::PCONTEXT ctx ) noexcept : ctx_( ctx ) {}

	flags get_flags() const noexcept;
	debug_registers get_debug_registers() noexcept;
	registers get_registers() const noexcept;
	detail::nt::PCONTEXT get_native() const noexcept;

	void set_flags( flags f ) noexcept;
	void set_debug_registers( const debug_registers& dr ) noexcept;
	void set_registers( const registers& r ) noexcept;
	void set_context_flag( unsigned int f ) noexcept;

	// Add better support for setting hardware breakpoints

	void set_register( context::register_id id, address_t value ) noexcept;
	void set_rax( address_t value ) noexcept { set_register( context::register_id::rax, value ); }
	void set_rcx( address_t value ) noexcept { set_register( context::register_id::rcx, value ); }
	void set_rdx( address_t value ) noexcept { set_register( context::register_id::rdx, value ); }
	void set_rbx( address_t value ) noexcept { set_register( context::register_id::rbx, value ); }
	void set_rsp( address_t value ) noexcept { set_register( context::register_id::rsp, value ); }
	void set_rbp( address_t value ) noexcept { set_register( context::register_id::rbp, value ); }
	void set_rsi( address_t value ) noexcept { set_register( context::register_id::rsi, value ); }
	void set_rdi( address_t value ) noexcept { set_register( context::register_id::rdi, value ); }
	void set_r8( address_t value ) noexcept { set_register( context::register_id::r8, value ); }
	void set_r9( address_t value ) noexcept { set_register( context::register_id::r9, value ); }
	void set_r10( address_t value ) noexcept { set_register( context::register_id::r10, value ); }
	void set_r11( address_t value ) noexcept { set_register( context::register_id::r11, value ); }
	void set_r12( address_t value ) noexcept { set_register( context::register_id::r12, value ); }
	void set_r13( address_t value ) noexcept { set_register( context::register_id::r13, value ); }
	void set_r14( address_t value ) noexcept { set_register( context::register_id::r14, value ); }
	void set_r15( address_t value ) noexcept { set_register( context::register_id::r15, value ); }
	void set_rip( address_t value ) noexcept { set_register( context::register_id::rip, value ); }

  private:
	detail::nt::PCONTEXT ctx_ = nullptr; // non-owning pointer to an opaque Windows struct
};

enum class context::context_flags : unsigned int {
	none = 0,

	i386 = 0x00010000L,
	i486 = 0x00010000L, // identical to i386

	control            = i386 | 0x00000001L, // SS:SP, CS:IP, FLAGS, BP
	integer            = i386 | 0x00000002L, // AX, BX, CX, DX, SI, DI
	segments           = i386 | 0x00000004L, // DS, ES, FS, GS
	floating_point     = i386 | 0x00000008L, // 387 state
	debug_registers    = i386 | 0x00000010L, // DR 0-3, 6, 7
	extended_registers = i386 | 0x00000020L, // CPU-specific extensions

	full = control | integer | segments,

	all = control | integer | segments | floating_point | debug_registers | extended_registers,

	xstate = i386 | 0x00000040L,

	exception_active    = 0x08000000L,
	service_active      = 0x10000000L,
	exception_request   = 0x40000000L,
	exception_reporting = 0x80000000L,
};

struct context::registers {
	address_t rax;
	address_t rcx;
	address_t rdx;
	address_t rbx;
	address_t rsp;
	address_t rbp;
	address_t rsi;
	address_t rdi;
	address_t r8;
	address_t r9;
	address_t r10;
	address_t r11;
	address_t r12;
	address_t r13;
	address_t r14;
	address_t r15;
	address_t rip;
};

#pragma warning( push )
#pragma warning( disable : 4201 )

struct context::dr6 {
	union {
		struct {
			unsigned int b0 : 1;
			unsigned int b1 : 1;
			unsigned int b2 : 1;
			unsigned int b3 : 1;
			unsigned int rsvd1 : 8;
			unsigned int b12 : 1;
			unsigned int bld : 1;
			unsigned int bd : 1;
			unsigned int bs : 1;
			unsigned int bt : 1;
			unsigned int rtm : 1;
			unsigned int rsvd2 : 14;
			unsigned int rsvd3 : 32;
		};
		unsigned long long raw;
	};

	dr6( unsigned long long raw ) : raw( raw ) {}
	dr6() : raw( 0 ) {}
};

struct context::dr7 {
	union {
		struct {
			unsigned int dr0_local : 1;
			unsigned int dr0_global : 1;
			unsigned int dr1_local : 1;
			unsigned int dr1_global : 1;
			unsigned int dr2_local : 1;
			unsigned int dr2_global : 1;
			unsigned int dr3_local : 1;
			unsigned int dr3_global : 1;
			unsigned int local_enabled : 1;
			unsigned int global_enabled : 1;
			unsigned int reserved_10 : 1;
			unsigned int rtm : 1;
			unsigned int reserved_12 : 1;
			unsigned int gd : 1;
			unsigned int reserved_14_15 : 2;
			unsigned int dr0_break : 2;
			unsigned int dr0_len : 2;
			unsigned int dr1_break : 2;
			unsigned int dr1_len : 2;
			unsigned int dr2_break : 2;
			unsigned int dr2_len : 2;
			unsigned int dr3_break : 2;
			unsigned int dr3_len : 2;
		};
		unsigned long long raw;
	};
	dr7( unsigned long long raw ) : raw( raw ) {}
	dr7() : raw( 0 ) {}
};

struct context::flags {
	union {
		struct {
			bool carry : 1;
			bool reserved_1 : 1;
			bool parity : 1;
			bool reserved_2 : 1;
			bool auxiliary_carry : 1;
			bool reserved_3 : 1;
			bool zero : 1;
			bool sign : 1;
			bool trap : 1;
			bool interrupt_enable : 1;
			bool direction : 1;
			bool overflow : 1;
			bool iopl : 2;
			bool nested_task : 1;
			bool reserved_4 : 1;
			bool resume : 1;
			bool virtual_8086_mode : 1;
			bool alignment_check : 1;
			bool virtual_interrupt : 1;
			bool virtual_interrupt_pending : 1;
			bool id : 1;
		};
		unsigned int raw;
	};
};
#pragma warning( pop )

struct context::debug_registers {
	address_t dr0;
	address_t dr1;
	address_t dr2;
	address_t dr3;
	context::dr6 dr6;
	context::dr7 dr7;
};

} // namespace task_manager
