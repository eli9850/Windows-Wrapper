#include "Wrappers/Overlapped/Overlapped.h"

#include "WinAPI/EventAPI.h"
#include "WinAPI/WaitAPI.h"
#include "Exceptions/WinException.h"

namespace WindowsWrapper::Wrappers
{

	Overlapped::Overlapped(HANDLE handle): m_handle(handle), m_overlapped{0}, m_event(API::EventAPI::create_event()),
	                                       m_result_data({})
	{
		m_overlapped.hEvent = m_event.get();
	}

	Overlapped::Overlapped(HANDLE handle, uint32_t number_of_bytes_to_read) : m_handle(handle), m_overlapped{0},
	                                                                          m_event(API::EventAPI::create_event()),
	                                                                          m_result_data(number_of_bytes_to_read)
	{
		m_overlapped.hEvent = m_event.get();
	}

	bool Overlapped::is_command_finished() const
	{
		return API::WaitAPI::wait_for_single_object(m_event.get());
	}

	void Overlapped::wait_for_command_to_finish(uint32_t timeout) const
	{
		API::WaitAPI::wait_for_single_object(m_event.get(), timeout);
	}

	std::vector<std::byte>& Overlapped::get_result_data()
	{
		return m_result_data;
	}

	OVERLAPPED& Overlapped::get_overlapped()
	{
		return m_overlapped;
	}

	uint32_t Overlapped::get_overlapped_result()
	{
		DWORD data_transfer = 0;
		const auto result = GetOverlappedResult(m_handle, &m_overlapped, &data_transfer, false);
		if (!result)
		{
			THROW_WINDOWS_ERROR(GetOverlappedResultException, "Failed to get overlapped result with error: ");
		}

		return data_transfer;
	}

}
