#include "WinAPI/PipeAPI.h"

#include "Exceptions/WinException.h"

namespace WindowsWrapper::API::PipeAPI
{
	Guards::HandleGuard create_named_pipe(const std::wstring& pipe_name,
	                                      uint32_t open_mode,
	                                      uint32_t pipe_mode,
	                                      uint32_t max_instances,
	                                      uint32_t out_buffer_size,
	                                      uint32_t in_buffer_size,
	                                      uint32_t default_timeout,
	                                      LPSECURITY_ATTRIBUTES security_attributes)
	{
		HANDLE result = CreateNamedPipeW(pipe_name.c_str(), open_mode,
		                                 pipe_mode, max_instances, out_buffer_size,
		                                 in_buffer_size, default_timeout,
		                                 security_attributes);
		if (INVALID_HANDLE_VALUE == result)
		{
			THROW_WINDOWS_ERROR(CreateNamedPipeException, "Failed to create named pipe with error: ");
		}
		return Guards::HandleGuard{result, CloseHandle};
	}

	uint32_t get_named_pipe_handle_state(HANDLE pipe)
	{
		DWORD state = 0;
		auto result = GetNamedPipeHandleStateW(pipe, &state, nullptr, nullptr, nullptr, nullptr, 0);
		if (!result)
		{
			THROW_WINDOWS_ERROR(GetNamedPipeHandleStateException,
			                    "Could not get named pipe handle state with error: ");
		}
		return state;
	}

	void connect_named_pipe(HANDLE pipe)
	{
		auto result = ConnectNamedPipe(pipe, nullptr);
		const auto pipe_state = get_named_pipe_handle_state(pipe);
		if (PIPE_NOWAIT == pipe_state)
		{
			if (!result)
			{
				const auto error = GetLastError();
				switch (error)
				{
				case ERROR_PIPE_LISTENING:
					THROW_WINDOWS_ERROR(NoWaitPipeListeningException, "Could not connect pipe with error: ");
				case ERROR_PIPE_CONNECTED:
					THROW_WINDOWS_ERROR(NoWaitPipeConnectedException, "Could not connect pipe with error: ");
				case ERROR_NO_DATA:
					THROW_WINDOWS_ERROR(NoWaitPipeNoDataException, "Could not connect pipe with error: ");
				default:
					break;
				}
			}
		}
		else
		{
			if (!result)
			{
				THROW_WINDOWS_ERROR(ConnectWaitPipeException, "Could not connect pipe with error: ");
			}
		}
	}

	Wrappers::Overlapped connect_named_pipe_async(HANDLE pipe)
	{
		Wrappers::Overlapped overlapped_result(pipe);
		const auto result = ConnectNamedPipe(pipe, &overlapped_result.get_overlapped());
		if (!result)
		{
			const auto error = GetLastError();
			switch (error)
			{
			case ERROR_IO_PENDING:
			case ERROR_PIPE_CONNECTED:
				return overlapped_result;
			default:
				THROW_WINDOWS_ERROR(ConnectWaitPipeException, "Could not connect pipe with error: ");
			}
		}
		return overlapped_result;
	}

	void wait_named_pipe(const std::wstring& pipe_name, uint32_t timeout)
	{
		auto result = WaitNamedPipeW(pipe_name.c_str(), timeout);
		if (!result)
		{
			THROW_WINDOWS_ERROR(WaitNamedPipeException, "Failed to wait for named pipe with error: ");
		}
	}

	uint32_t get_number_of_bytes_to_read(HANDLE pipe)
	{
		DWORD number_of_bytes_to_read;
		const auto result = PeekNamedPipe(pipe, nullptr, 0, nullptr, &number_of_bytes_to_read, nullptr);
		if (!result)
		{
			THROW_WINDOWS_ERROR(PeekNamedPipeException, "Could not peek named pipe with error: ");
		}
		return number_of_bytes_to_read;
	}

	uint32_t get_number_of_bytes_to_read_from_message(HANDLE pipe)
	{
		DWORD number_of_bytes_left_in_message;
		const auto result = PeekNamedPipe(pipe, nullptr, 0, nullptr, nullptr, &number_of_bytes_left_in_message);
		if (!result)
		{
			THROW_WINDOWS_ERROR(PeekNamedPipeException, "Could not peek named pipe with error: ");
		}
		return number_of_bytes_left_in_message;
	}

	std::vector<std::byte> peek_named_pipe(HANDLE pipe, uint32_t number_of_bytes_to_read)
	{
		std::vector<std::byte> data(number_of_bytes_to_read);
		DWORD number_of_bytes_read = 0;
		const auto result = PeekNamedPipe(pipe, data.data(), number_of_bytes_to_read, &number_of_bytes_read, nullptr,
		                                  nullptr);
		if (!result)
		{
			THROW_WINDOWS_ERROR(PeekNamedPipeException, "Could not peek named pipe with error: ");
		}
		if (number_of_bytes_read < number_of_bytes_to_read)
		{
			data.resize(number_of_bytes_read);
			data.shrink_to_fit();
		}
		return data;
	}

	void disconnect_named_pipe(HANDLE pipe)
	{
		auto result = DisconnectNamedPipe(pipe);
		if (!result)
		{
			THROW_WINDOWS_ERROR(DisconnectNamedPipeException, "Could not diconnect named pipe with error: ");
		}
	}
}
