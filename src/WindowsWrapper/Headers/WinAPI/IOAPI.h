#pragma once

#include <Windows.h>
#include <filesystem>

#include "WinAPIConsts.h"
#include "Guards/HandleGuard.h"
#include "Wrappers/Overlapped/Overlapped.h"


namespace WindowsWrapper::API::IOAPI
{
	Guards::HandleGuard create_file(const std::filesystem::path& file_path,
	                                uint32_t desired_access = FILE_ALL_ACCESS,
	                                uint32_t share_mode = Consts::NO_SHARE,
	                                LPSECURITY_ATTRIBUTES security_attributes = nullptr,
	                                uint32_t creation_disposition = OPEN_ALWAYS,
	                                uint32_t flags_and_attributes = FILE_ATTRIBUTE_NORMAL,
	                                HANDLE temple_file = nullptr);

	std::vector<std::byte> read_file(HANDLE file,
	                                 uint32_t number_of_bytes_to_read);

	void write_file(HANDLE file,
	                const std::vector<std::byte>& buffer,
	                uint32_t* number_of_bytes_written);

	Wrappers::Overlapped read_file_async(HANDLE file,
	                                     uint32_t number_of_bytes_to_read);

	Wrappers::Overlapped write_file_async(HANDLE file,
	                                      const std::vector<std::byte>& buffer);
}
