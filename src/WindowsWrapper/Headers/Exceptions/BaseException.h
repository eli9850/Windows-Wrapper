#pragma once

#include <exception>
#include <string>

class BaseException : public std::exception
{
public:
	explicit BaseException(const std::string& message):
		std::exception(message.c_str())
	{
	}
};

#define CREATE_EXCEPTION(driven_exception, base_exception)                            \
class driven_exception : public base_exception                                        \
{                                                                                     \
public:                                                                               \
	driven_exception(const std::string& message):                                     \
				     base_exception(message.c_str())                                  \
	{                                                                                 \
	}                                                                                 \
}
