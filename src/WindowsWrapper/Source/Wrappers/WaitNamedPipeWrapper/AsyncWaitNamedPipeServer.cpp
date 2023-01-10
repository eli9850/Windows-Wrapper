#include "Wrappers/WaitNamedPipeServer/AsyncWaitNamedPipeServer.h"

#include "WinAPI/PipeAPI.h"
#include "WinAPI/IOAPI.h"
#include "Exceptions/WinException.h"
#include "Exceptions/PipeServerExceptions.h"
#include "Wrappers/Overlapped/Overlapped.h"

namespace WindowsWrapper::Wrappers
{
	AsyncWaitNamedPipeServer::AsyncWaitNamedPipeServer(const std::wstring& pipe_name, uint32_t open_mode,
	                                                   uint32_t pipe_mode,
	                                                   uint32_t max_instances, uint32_t out_buffer_size,
	                                                   uint32_t in_buffer_size, uint32_t default_timeout,
	                                                   LPSECURITY_ATTRIBUTES security_attributes)
	try :
		m_pipe_handle(API::PipeAPI::create_named_pipe(pipe_name, open_mode, pipe_mode, max_instances,
		                                              out_buffer_size, in_buffer_size,
		                                              default_timeout, security_attributes)),
		m_is_byte_mood(PIPE_TYPE_BYTE == pipe_mode)
	{
		if (!(open_mode & FILE_FLAG_OVERLAPPED))
		{
			throw PipeServerOverlappedException("Pipe must have the overlapped flag");
		}
	}
	catch (const PipeException& e)
	{
		throw PipeServerException(e.what());
	}

	Overlapped AsyncWaitNamedPipeServer::connect_named_pipe() const
	{
		try
		{
			return API::PipeAPI::connect_named_pipe_async(m_pipe_handle.get());
		}
		catch (const PipeException& e)
		{
			throw PipeServerConnectException(e.what());
		}
	}

	bool AsyncWaitNamedPipeServer::is_pipe_connected() const
	{
		try
		{
			API::PipeAPI::get_number_of_bytes_to_read(m_pipe_handle.get());
			return true;
		}
		catch (const PipeException&)
		{
			return false;
		}
	}

	void AsyncWaitNamedPipeServer::disconnect_named_pipe() const
	{
		try
		{
			API::PipeAPI::disconnect_named_pipe(m_pipe_handle.get());
		}
		catch (const PipeException& e)
		{
			throw PipeServerDisconnectException(e.what());
		}
	}

	Overlapped AsyncWaitNamedPipeServer::write_named_pipe(const std::vector<std::byte>& buffer) const
	{
		try
		{
			return API::IOAPI::write_file_async(m_pipe_handle.get(), buffer);
		}
		catch (const IOException& e)
		{
			throw PipeServerWriteException(e.what());
		}
	}

	Overlapped AsyncWaitNamedPipeServer::read_named_pipe(uint32_t number_of_bytes_to_read) const
	{
		try
		{
			return API::IOAPI::read_file_async(m_pipe_handle.get(), number_of_bytes_to_read);
		}
		catch (const IOException& e)
		{
			throw PipeServerWriteException(e.what());
		}
	}

	std::vector<std::byte> AsyncWaitNamedPipeServer::peek_named_pipe(uint32_t number_of_bytes_to_read) const
	{
		try
		{
			return API::PipeAPI::peek_named_pipe(m_pipe_handle.get(), number_of_bytes_to_read);
		}
		catch (const PipeException& e)
		{
			throw PipeServerWriteException(e.what());
		}
	}

	uint32_t AsyncWaitNamedPipeServer::get_number_of_bytes_to_read() const
	{
		try
		{
			return API::PipeAPI::get_number_of_bytes_to_read(m_pipe_handle.get());
		}
		catch (const PipeException& e)
		{
			throw PipeServerPeekException(e.what());
		}
	}

	uint32_t AsyncWaitNamedPipeServer::get_number_of_bytes_to_read_from_message() const
	{
		try
		{
			if (m_is_byte_mood)
			{
				throw PipeServerPeekException("The server is in byte mood. There are no messages");
			}
			return API::PipeAPI::get_number_of_bytes_to_read_from_message(m_pipe_handle.get());
		}
		catch (const PipeException& e)
		{
			throw PipeServerWriteException(e.what());
		}
	}
}
