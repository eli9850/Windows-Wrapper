#pragma once
#include <cstdint>
#include <string>
#include <Windows.h>

namespace WindowsWrapper::Consts
{
	constexpr uint32_t PIPE_OUT_BUFFER_SIZE = 1024;
	constexpr uint32_t PIPE_IN_BUFFER_SIZE = 1024;
	constexpr uint32_t PIPE_MAX_INSTANCES = 255;
	constexpr uint32_t PIPE_DEFAULT_TYPE = PIPE_TYPE_BYTE;
	constexpr uint32_t PIPE_DEFAULT_MODE = PIPE_ACCESS_DUPLEX;

	constexpr uint32_t NO_SHARE = 0;
}
