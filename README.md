# task_manager

A Windows process and thread introspection library in modern C++.

## What it is for

`task_manager` wraps the Windows NT API in a typed, RAII-friendly C++23 surface.
You can open processes, walk their loaded modules, read their memory, suspend
or freeze and thaw them, and read or modify thread CPU state, all without
hand-writing the underlying boilerplate.

The audience is people writing debuggers, profilers, anti-cheat or anti-tamper
tooling, or any program that needs to introspect another process on Windows.

## Project status

Version 0.1.0. Early stage, the API is not yet stable.

### What works today

* Open processes by pid or by executable name; spawn new processes; get a
  handle to the current process.
* Query pid, name, image path, parent pid, architecture (`x86`, `x64`,
  `arm64`).
* Enumerate loaded modules via a PEB / Ldr walk (base, size, name, path,
  entry point) on x64 targets.
* Read virtual memory from a target process.
* Suspend and resume the whole process, with a `suspend_scoped` RAII guard.
* Freeze and thaw via Windows Job Objects, with a `freeze_scoped` RAII guard.
* Read and write thread CPU context: GPRs, `RIP`, `EFLAGS`, debug registers
  (x64).
* Strong types (`pid_t`, `tid_t`, `address_t`, `arch_t`) and a bitwise
  `access_rights` enum.
* `std::expected<T, errc>` based error handling; no exceptions on the API
  boundary.
* RAII `unique_handle` over NT handles.
* Optional PE introspection (sections, exports, imports, headers) behind the
  `TASK_MANAGER_WITH_PE_PARSER` CMake option.

### What is not done yet

* [src/process/memory.cpp](src/process/memory.cpp) is a stub. Only
  `read_virtual_memory` works today; no write, protect, or query helpers.
* [src/thread/thread.cpp](src/thread/thread.cpp) is a stub. The `thread` class
  is declared but its accessors (`is_alive`, `state`, `priority`) are not
  implemented yet.
* [src/system/privileges.cpp](src/system/privileges.cpp) and
  [src/system/token.cpp](src/system/token.cpp) are stubs. No privilege
  adjustment or token APIs yet.
* Module enumeration is x64-only. The 32-bit PEB / WoW64 walk is not
  implemented.
* Thread context is x64-only. `arm64` and `x86` register surfaces are not
  exposed.
* Integration tests under `tests/integration/` are reserved but not yet
  written. Only unit tests run today.
* PE-parser wiring in CMake is a `TODO`. Consumers who enable
  `TASK_MANAGER_WITH_PE_PARSER` must currently supply the `pe_parser::pe_parser`
  target themselves.
* The example CMake assumes MSVC and does not cover `arm64`.

## Quick example

```cpp
#include "task_manager/process/process.hpp"
#include <print>

int main() {
    auto p = task_manager::process::current();
    if (!p) {
        std::println(stderr, "current() failed: {}",
                     task_manager::to_string(p.error()));
        return 1;
    }
    std::println("pid:  {}", static_cast<unsigned>(p->pid()));
    if (auto n = p->name())       std::println("name: {}", *n);
    if (auto i = p->image_path()) std::println("path: {}", i->string());
}
```

See [examples/list_current_process.cpp](examples/list_current_process.cpp) for
the full version.

## Examples

Each example lives in [examples/](examples/) and is built by default.

* `list_current_process`: read-only query of the running process.
* `list_modules [pid]`: enumerate loaded modules via the PEB walk.
* `open_by_name [exe]`: find a process by executable name.
* `scoped_suspend <pid> [seconds]`: RAII suspend and resume.
* `scoped_freeze  <pid> [seconds]`: RAII freeze and thaw via a Job Object.

## Requirements

* Windows.
* MSVC with C++23 support (tested with the Visual Studio 17 2022 toolset).
* CMake 3.25 or newer.
* The `ntdll` import library, which ships with the Windows SDK.
* Optional: a `pe_parser::pe_parser` target if you enable
  `TASK_MANAGER_WITH_PE_PARSER`.

## Building

Using the bundled CMake presets:

```pwsh
cmake --preset msvc-release
cmake --build --preset msvc-release
ctest   --preset msvc-debug --output-on-failure
```

Or directly:

```pwsh
cmake -B build -S . -G "Visual Studio 17 2022"
cmake --build build --config Release
```

### CMake options

| Option                          | Default | Effect                                              |
| ------------------------------- | ------- | --------------------------------------------------- |
| `TASK_MANAGER_BUILD_TESTS`      | `ON`    | Build the Catch2 unit tests.                        |
| `TASK_MANAGER_BUILD_EXAMPLES`   | `ON`    | Build the example executables under `examples/`.    |
| `TASK_MANAGER_WITH_PE_PARSER`   | `OFF`   | Link `pe_parser` and expose PE methods on modules.  |

When the library is consumed via `add_subdirectory` or `FetchContent`, tests
and examples are forced off automatically.

## Consuming as a dependency

After `cmake --install`:

```cmake
find_package(task_manager CONFIG REQUIRED)
target_link_libraries(my_app PRIVATE task_manager::task_manager)
```

Via `FetchContent`:

```cmake
include(FetchContent)
FetchContent_Declare(
    task_manager
    GIT_REPOSITORY https://github.com/Ares3173/task_manager.git
    GIT_TAG        main)
FetchContent_MakeAvailable(task_manager)

target_link_libraries(my_app PRIVATE task_manager::task_manager)
```

## Tests

Catch2 v3 is pulled in via `FetchContent`. Unit tests live in
[tests/unit/](tests/unit/) and cover the error category and the strong-typed
ID layer. Run them with:

```pwsh
ctest --test-dir build -C Debug --output-on-failure
```

Filter by Catch2 tag if you want to run a subset:

```pwsh
./build/tests/Debug/task_manager_tests.exe "[unit]"
```

## Public headers

```
include/
└── task_manager/
    ├── error.hpp
    ├── types.hpp
    ├── detail/
    │   ├── nt_api.hpp
    │   ├── nt_types.hpp
    │   └── unique_handle.hpp
    ├── process/
    │   ├── process.hpp
    │   ├── modules.hpp
    │   └── memory.hpp
    ├── thread/
    │   ├── thread.hpp
    │   └── context.hpp
    └── system/
        ├── privileges.hpp
        └── token.hpp
```

The umbrella header [include/task_manager.hpp](include/task_manager.hpp)
pulls in the process and thread APIs.

## License

MIT. See [LICENSE](LICENSE).
