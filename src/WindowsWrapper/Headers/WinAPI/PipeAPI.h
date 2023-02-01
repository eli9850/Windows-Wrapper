#pragma once

#include "WinAPIConsts.h"
#include "Guards/HandleGuard.h"
#include "Wrappers/Overlapped/Overlapped.h"

#include <string>
#include <vector>

namespace WindowsWrapper::API::PipeAPI
{
	Guards::HandleGuard create_named_pipe(const std::wstring& pipe_name,
	                                      uint32_t open_mode = Consts::PIPE_DEFAULT_MODE,
	                                      uint32_t pipe_mode = Consts::PIPE_DEFAULT_TYPE,
	                                      uint32_t max_instances = Consts::PIPE_MAX_INSTANCES,
	                                      uint32_t out_buffer_size = Consts::PIPE_OUT_BUFFER_SIZE,
	                                      uint32_t in_buffer_size = Consts::PIPE_IN_BUFFER_SIZE,
	                                      uint32_t default_timeout = 0,
	                                      LPSECURITY_ATTRIBUTES security_attributes = nullptr);

	uint32_t get_named_pipe_handle_state(HANDLE pipe);

	void connect_named_pipe(HANDLE pipe);

	Wrappers::Overlapped connect_named_pipe_async(HANDLE pipe);

	void wait_named_pipe(const std::wstring& pipe_name,
	                     uint32_t timeout = NMPWAIT_WAIT_FOREVER);

	uint32_t get_number_of_bytes_to_read(HANDLE pipe);

	uint32_t get_number_of_bytes_to_read_from_message(HANDLE pipe);

	std::vector<std::byte> peek_named_pipe(HANDLE pipe, uint32_t number_of_bytes_to_read);

	void disconnect_named_pipe(HANDLE pipe);
}
