#pragma once

#include <Windows.h>
#include "BaseException.h"

class WinException : public BaseException
{
public:
	explicit WinException(const std::string& message, uint32_t error) :
		BaseException(message), m_error(error)
	{
	}

	[[nodiscard]] uint32_t get_error() const
	{
		return m_error;
	}
private:
	uint32_t m_error;
};


#define CREATE_WIN32_EXCEPTION(driven_exception, base_exception)                      \
class driven_exception : public base_exception                                        \
{                                                                                     \
public:                                                                               \
	driven_exception(const std::string& message, uint32_t error):                     \
				     base_exception(message.c_str(), error)                           \
	{                                                                                 \
	}                                                                                 \
}

#define THROW_WINDOWS_ERROR(WinException, message) throw WinException(get_error_message(message, GetLastError()), GetLastError())

static std::string get_error_message(const std::string& message, uint32_t error_code)
{
	LPSTR messageBuffer = nullptr;

	//Ask Win32 to give us the string version of that message ID.
	//The parameters we pass in, tell Win32 to create the buffer that holds the message for us (because we don't yet know how long the message string will be).
	size_t size = FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, error_code, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

	//Copy the error message into a std::string.
	std::string error_message(messageBuffer, size);

	//Free the Win32's string's buffer.
	LocalFree(messageBuffer);

	return message + error_message;
}

// Pipe exceptions

CREATE_WIN32_EXCEPTION(PipeException, WinException);

CREATE_WIN32_EXCEPTION(CreateNamedPipeException, PipeException);

CREATE_WIN32_EXCEPTION(DisconnectNamedPipeException, PipeException);

CREATE_WIN32_EXCEPTION(GetNamedPipeHandleStateException, PipeException);

CREATE_WIN32_EXCEPTION(PeekNamedPipeException, PipeException);

CREATE_WIN32_EXCEPTION(WaitNamedPipeException, PipeException);

CREATE_WIN32_EXCEPTION(NoWaitPipeListeningException, PipeException);

CREATE_WIN32_EXCEPTION(NoWaitPipeConnectedException, PipeException);

CREATE_WIN32_EXCEPTION(NoWaitPipeNoDataException, PipeException);

CREATE_WIN32_EXCEPTION(ConnectWaitPipeException, PipeException);

CREATE_WIN32_EXCEPTION(WaitPipeStillPendingException, PipeException);

CREATE_WIN32_EXCEPTION(WaitPipeConnectedException, PipeException);

// IO Exceptions

CREATE_WIN32_EXCEPTION(IOException, WinException);

CREATE_WIN32_EXCEPTION(CreateFileException, IOException);

CREATE_WIN32_EXCEPTION(ReadFileException, IOException);

CREATE_WIN32_EXCEPTION(WriteFileException, IOException);

// Synchronize Exceptions

CREATE_WIN32_EXCEPTION(SynchronizeException, WinException);

CREATE_WIN32_EXCEPTION(GetOverlappedResultException, SynchronizeException);

CREATE_WIN32_EXCEPTION(CreateEventException, SynchronizeException);

CREATE_WIN32_EXCEPTION(OpenEventException, SynchronizeException);

CREATE_WIN32_EXCEPTION(SetEventException, SynchronizeException);

CREATE_WIN32_EXCEPTION(ResetEventException, SynchronizeException);

CREATE_WIN32_EXCEPTION(WaitAbandonedException, SynchronizeException);

CREATE_WIN32_EXCEPTION(WaitForSingleObjectException, SynchronizeException);

CREATE_WIN32_EXCEPTION(WaitForMultipleObjectsException, SynchronizeException);
