#pragma once

#include <string>
#include <vector>

#include "Guards/HandleGuard.h"
#include "WinAPI/WinAPIConsts.h"


namespace WindowsWrapper::Wrappers
{
	class WaitNamedPipeServer final
	{
	public:
		explicit WaitNamedPipeServer(const std::wstring& pipe_name,
		                             uint32_t open_mode = Consts::PIPE_DEFAULT_MODE,
		                             uint32_t pipe_mode = Consts::PIPE_DEFAULT_TYPE,
		                             uint32_t max_instances = Consts::PIPE_MAX_INSTANCES,
		                             uint32_t out_buffer_size = Consts::PIPE_OUT_BUFFER_SIZE,
		                             uint32_t in_buffer_size = Consts::PIPE_IN_BUFFER_SIZE,
		                             uint32_t default_timeout = 0,
		                             LPSECURITY_ATTRIBUTES security_attributes = nullptr);

		void connect_named_pipe() const;
		[[nodiscard]] bool is_pipe_connected() const;
		void disconnect_named_pipe() const;

		void write_named_pipe(const std::vector<std::byte>& buffer) const;
		[[nodiscard]] std::vector<std::byte> read_named_pipe(uint32_t number_of_bytes_to_read) const;
		[[nodiscard]] std::vector<std::byte> peek_named_pipe(uint32_t number_of_bytes_to_read) const;
		[[nodiscard]] uint32_t get_number_of_bytes_to_read() const;
		[[nodiscard]] uint32_t get_number_of_bytes_to_read_from_message() const;

	private:
		Guards::HandleGuard m_pipe_handle;
		bool m_is_byte_mood;
	};
}
