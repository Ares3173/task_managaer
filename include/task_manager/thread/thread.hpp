// /include/task_manager/thread/thread.hpp

#pragma once
#include "task_manager/detail/unique_handle.hpp"
#include "task_manager/thread/context.hpp"
#include "task_manager/types.hpp"

#include <cstdint>

namespace task_manager {
class thread {
  public:
	enum class state : std::uint32_t;
	enum class priority : std::int32_t;

	thread()                               = default;
	thread( const thread& )                = delete;
	thread& operator=( const thread& )     = delete;
	thread( thread&& ) noexcept            = default;
	thread& operator=( thread&& ) noexcept = default;
	~thread()                              = default;

	// Info
	tid_t tid() const noexcept { return tid_; }
	bool is_alive() const noexcept;
	state state() const noexcept;
	priority priority() const noexcept;
	access_rights access() const noexcept;

  private:
	detail::unique_handle handle_;
	tid_t tid_{};
};

enum class thread::state : std::uint32_t {
	Initialized             = 0,
	Ready                   = 1,
	Running                 = 2,
	Standby                 = 3,
	Terminated              = 4,
	Waiting                 = 5,
	Transition              = 6,
	DeferredReady           = 7,
	GateWaitObsolete        = 8,
	WaitingForProcessInSwap = 9,
	MaximumThreadState      = 10
};

enum class thread::priority : std::int32_t {
	Idle         = -15,
	Lowest       = -2,
	BelowNormal  = -1,
	Normal       = 0,
	AboveNormal  = 1,
	Highest      = 2,
	TimeCritical = 15,

	BackgroundBegin = 0x00010000,
	BackgroundEnd   = 0x00020000
};

} // namespace task_manager
