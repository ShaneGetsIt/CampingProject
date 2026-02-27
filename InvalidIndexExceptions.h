#pragma once

#include <stdexcept>
#include <string>

// Custom exception for invalid index/removal operations
class InvalidIndexException : public std::runtime_error
{
public:
    explicit InvalidIndexException(const std::string& msg) //Explicit notes for professor
        : std::runtime_error(msg)
    {
    }
};