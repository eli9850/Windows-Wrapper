#pragma once

#include <Windows.h>

#include <vector>
#include "Guards/HandleGuard.h"

namespace WindowsWrapper::Wrappers
{
	class Overlapped
	{
	public:
		explicit Overlapped(HANDLE handle);
		explicit Overlapped(HANDLE handle, uint32_t number_of_bytes_to_read);
		[[nodiscard]] bool is_command_finished() const;
		void wait_for_command_to_finish(uint32_t timeout) const;
		[[nodiscard]] std::vector<std::byte>& get_result_data();
		[[nodiscard]] OVERLAPPED& get_overlapped();
		uint32_t get_overlapped_result();


	private:
		HANDLE m_handle;
		OVERLAPPED m_overlapped;
		Guards::HandleGuard m_event;
		std::vector<std::byte> m_result_data;
	};
}
