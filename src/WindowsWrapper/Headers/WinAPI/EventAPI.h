#pragma once

#include "Guards/HandleGuard.h"

#include <string>

namespace WindowsWrapper::API::EventAPI
{
	Guards::HandleGuard create_event(const std::wstring& name = L"", bool manual_reset = true,
	                                 bool initial_state = false,
	                                 LPSECURITY_ATTRIBUTES security_attributes = nullptr);

	Guards::HandleGuard open_event(const std::wstring& name, uint32_t desired_access,
	                               bool inherit_handle = false);

	void reset_event(HANDLE event_handle);

	void set_event(HANDLE event_handle);
}
