#include "WinAPI/EventAPI.h"

#include "Exceptions/WinException.h"


namespace WindowsWrapper::API::EventAPI
{
	Guards::HandleGuard create_event(const std::wstring& name, bool manual_reset, bool initial_state,
		                                LPSECURITY_ATTRIBUTES security_attributes)
	{
		HANDLE result = nullptr;
		if (name.empty())
		{
			result = CreateEventW(security_attributes, manual_reset, initial_state, nullptr);
		}
		else
		{
			result = CreateEventW(security_attributes, manual_reset, initial_state, name.c_str());
		}

		if (!result)
		{
			THROW_WINDOWS_ERROR(CreateEventException, "Could not create event with error: ");
		}
		return Guards::HandleGuard{result, &CloseHandle};
	}

	Guards::HandleGuard open_event(const std::wstring& name, uint32_t desired_access,
		                            bool inherit_handle)
	{
		const auto result = OpenEventW(desired_access, inherit_handle, name.c_str());
		if (!result)
		{
			THROW_WINDOWS_ERROR(OpenEventException, "Could not open event with error: ");
		}
		return Guards::HandleGuard{result, &CloseHandle};
	}

	void reset_event(HANDLE event_handle)
	{
		const auto result = ResetEvent(event_handle);
		if (!result)
		{
			THROW_WINDOWS_ERROR(ResetEventException, "Could not reset event with error: ");
		}
	}

	void set_event(HANDLE event_handle)
	{
		const auto result = SetEvent(event_handle);
		if (!result)
		{
			THROW_WINDOWS_ERROR(SetEventException, "Could not set event with error: ");
		}
	}

}
