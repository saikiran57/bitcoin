#pragma once

#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <variant>

using TimeVariant =
    std::variant<std::chrono::seconds, std::chrono::milliseconds, std::chrono::microseconds, std::chrono::nanoseconds>;

template <class T = std::chrono::milliseconds>
std::string TimeVariantType()
{
    std::string type;
    if constexpr (std::is_same_v<T, std::chrono::seconds>) {
        type = "seconds";
    } else if constexpr (std::is_same_v<T, std::chrono::milliseconds>) {
        type = "milliseconds";
    } else if constexpr (std::is_same_v<T, std::chrono::microseconds>) {
        type = "microseconds";
    } else if constexpr (std::is_same_v<T, std::chrono::nanoseconds>) {
        type = "nanoseconds";
    } else {
        throw std::runtime_error("unknown type");
    }

    return type;
}

template <class Resolution = std::chrono::milliseconds>
class ExecutionTimer
{
public:
    using Clock = std::conditional_t<std::chrono::high_resolution_clock::is_steady, std::chrono::high_resolution_clock,
                                     std::chrono::steady_clock>;

private:
    Clock::time_point mStart = Clock::now();
    std::string m_message = "";
    std::atomic_bool m_isStopped = false;

public:
    ExecutionTimer() = default;
    ExecutionTimer(const std::string& message) : m_message(message) {}
    ~ExecutionTimer()
    {
        if (!m_isStopped) {
            stop();
        }
    }

    void start(const std::string& message)
    {
        m_message = message;
        m_isStopped = false;
        mStart = Clock::now();
    }

    void stop()
    {
        const auto end = Clock::now();
        std::ostringstream strStream;
        strStream << "Time Elapsed for " << m_message << ":" << std::chrono::duration_cast<Resolution>(end - mStart).count() << " "
                  << TimeVariantType<Resolution>();
        std::cout << strStream.str() << std::endl;
        m_isStopped = true;
    }
}; // ExecutionTimer