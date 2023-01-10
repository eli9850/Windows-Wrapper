#pragma once

#include "Guards/HandleGuard.h"

#include<optional>
#include <vector>

namespace WindowsWrapper::API::WaitAPI
{
	bool wait_for_single_object(HANDLE object, uint32_t timeout = 0);

	std::optional<uint32_t> wait_for_multiple_objects(const std::vector<HANDLE>& objects, bool wait_all, uint32_t timeout);
}

