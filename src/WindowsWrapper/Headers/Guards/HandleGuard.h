#pragma once

#include <Windows.h>
#include <memory>

namespace WindowsWrapper::Guards
{
	using HandleGuard = std::unique_ptr<std::remove_pointer_t<HANDLE>, decltype(&CloseHandle)>;
}
