// /src/thread/context.cpp

#include "task_manager/thread/context.hpp"

#include "task_manager/detail/nt_api.hpp"
#include "task_manager/detail/nt_types.hpp"
#include "task_manager/process/process.hpp"

#include <cstring>

namespace task_manager {
using ctx = context;

auto ctx::current() -> std::expected<context, errc> {
	detail::nt::CONTEXT c{};
	c.ContextFlags = std::to_underlying( context::context_flags::all );
	if ( !detail::nt::get_context( ( void* )-2, &c ) )
		return std::unexpected{ errc::invalid_handle };
	return context{ detail::nt::PCONTEXT( new detail::nt::CONTEXT( c ) ) };
}

auto ctx::get_flags() const noexcept -> ctx::flags {
	if ( ctx_ == nullptr )
		return {};

	return ctx::flags{ .raw = ctx_->EFlags };
}

auto ctx::get_debug_registers() noexcept -> ctx::debug_registers {
	ctx::debug_registers debug_regs{};
	if ( ctx_ == nullptr )
		return debug_regs;

	std::memcpy( &debug_regs.dr0, &ctx_->Dr0, sizeof( ctx::debug_registers ) );
	return debug_regs;
}

auto ctx::get_registers() const noexcept -> ctx::registers {
	ctx::registers regs{};
	if ( ctx_ == nullptr )
		return regs;

	std::memcpy( &regs.rax, &ctx_->Rax, sizeof( ctx::registers ) );
	return regs;
}

void ctx::set_register( context::register_id id, address_t value ) noexcept {
	if ( ctx_ == nullptr )
		return;
	const auto id_val = static_cast<unsigned>( id );
	if ( id_val <= static_cast<unsigned>( context::register_id::rip ) ) {
		std::memcpy(
		    reinterpret_cast<std::byte*>( &ctx_->Rax ) + ( id_val * sizeof( address_t ) ), &value,
		    sizeof( address_t ) );
	}
}

auto ctx::get_native() const noexcept -> detail::nt::PCONTEXT {
	return ctx_;
}

void ctx::set_context_flag( unsigned int f ) noexcept {
	if ( ctx_ == nullptr )
		return;
	ctx_->ContextFlags |= f;
}

void ctx::set_flags( flags f ) noexcept {
	if ( ctx_ == nullptr )
		return;
	ctx_->EFlags = f.raw;
}

void ctx::set_registers( const registers& r ) noexcept {
	if ( ctx_ == nullptr )
		return;
	std::memcpy( &ctx_->Rax, &r.rax, sizeof( ctx::registers ) );
}

void ctx::set_debug_registers( const debug_registers& dr ) noexcept {
	if ( ctx_ == nullptr )
		return;
	std::memcpy( &ctx_->Dr0, &dr.dr0, sizeof( ctx::debug_registers ) );
}

} // namespace task_manager
