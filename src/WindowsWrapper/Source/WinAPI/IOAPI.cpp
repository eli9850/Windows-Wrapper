#include "Exceptions/WinException.h"

#include "WinAPI/IOAPI.h"

namespace WindowsWrapper::API::IOAPI
{
	Guards::HandleGuard create_file(const std::filesystem::path& file_path,
	                                uint32_t desired_access,
	                                uint32_t share_mode,
	                                LPSECURITY_ATTRIBUTES security_attributes,
	                                uint32_t creation_disposition,
	                                uint32_t flags_and_attributes,
	                                HANDLE temple_file)
	{
		const auto result = CreateFileW(file_path.c_str(), desired_access, share_mode, security_attributes,
		                                creation_disposition, flags_and_attributes, temple_file);

		if (INVALID_HANDLE_VALUE == result)
		{
			THROW_WINDOWS_ERROR(CreateFileException, "Failed to create file with error: ");
		}
		return Guards::HandleGuard{result, CloseHandle};
	}

	std::vector<std::byte> read_file(HANDLE file,
	                                 uint32_t number_of_bytes_to_read)
	{
		std::vector<std::byte> data(number_of_bytes_to_read);
		DWORD number_of_bytes_read = 0;
		const auto result = ReadFile(file, data.data(), number_of_bytes_to_read,
		                             &number_of_bytes_read,
		                             nullptr);
		if (!result)
		{
			THROW_WINDOWS_ERROR(ReadFileException, "Failed to read file with error: ");
		}
		if (number_of_bytes_read < number_of_bytes_to_read)
		{
			data.resize(number_of_bytes_read);
			data.shrink_to_fit();
		}
		return data;
	}

	void write_file(HANDLE file,
	                const std::vector<std::byte>& buffer,
	                uint32_t* number_of_bytes_written = nullptr)
	{
		DWORD dummy = 0;
		const auto result = WriteFile(file, buffer.data(), buffer.size(), &dummy, nullptr);
		if (!result)
		{
			THROW_WINDOWS_ERROR(WriteFileException, "Failed to write file with error: ");
		}
		if (nullptr != number_of_bytes_written)
		{
			*number_of_bytes_written = dummy;
		}
	}

	Wrappers::Overlapped read_file_async(HANDLE file, uint32_t number_of_bytes_to_read)
	{
		Wrappers::Overlapped overlapped_result(file, number_of_bytes_to_read);


		const auto result = ReadFile(file, overlapped_result.get_result_data().data(), number_of_bytes_to_read,
		                             nullptr, &overlapped_result.get_overlapped());
		if (!result)
		{
			const auto error = GetLastError();
			if (ERROR_IO_INCOMPLETE != error && ERROR_IO_PENDING != error)
			{
				THROW_WINDOWS_ERROR(ReadFileException, "Failed to read file with error: ");
			}
		}

		return overlapped_result;
	}

	Wrappers::Overlapped write_file_async(HANDLE file, const std::vector<std::byte>& buffer)
	{
		Wrappers::Overlapped overlapped_result(file);

		const auto result = WriteFile(file, buffer.data(), buffer.size(), nullptr, &overlapped_result.get_overlapped());
		if (!result)
		{
			THROW_WINDOWS_ERROR(WriteFileException, "Failed to write file with error: ");
		}

		return overlapped_result;
	}
}
