#include "WinAPI/WaitAPI.h"

#include "Exceptions/WinException.h"

namespace WindowsWrapper::API::WaitAPI
{
	bool wait_for_single_object(HANDLE object, uint32_t timeout)
	{
		const auto result = WaitForSingleObject(object, timeout);
		switch (result)
		{
		case WAIT_OBJECT_0:
			return true;
		case WAIT_TIMEOUT:
			return false;
		case WAIT_ABANDONED:
			THROW_WINDOWS_ERROR(WaitAbandonedException, "Could not wait for single object with error: ");
		case WAIT_FAILED:
			THROW_WINDOWS_ERROR(WaitForSingleObjectException, "Could not wait for single object with error: ");
		default:
			THROW_WINDOWS_ERROR(SynchronizeException, "Could not wait for single object with error: ");
		}
	}

	std::optional<uint32_t> wait_for_multiple_objects(const std::vector<HANDLE>& objects, bool wait_all, uint32_t timeout)
	{
		const auto result = WaitForMultipleObjects(objects.size(), objects.data(), wait_all, timeout);
		switch (result)
		{
		case WAIT_OBJECT_0:
			return result - WAIT_OBJECT_0;
		case WAIT_TIMEOUT:
			return std::nullopt;
		case WAIT_ABANDONED:
			THROW_WINDOWS_ERROR(WaitAbandonedException, "Could not wait for single object with error: ");
		case WAIT_FAILED:
			THROW_WINDOWS_ERROR(WaitForSingleObjectException, "Could not wait for single object with error: ");
		default:
			THROW_WINDOWS_ERROR(SynchronizeException, "Could not wait for single object with error: ");
		}
	}
}
