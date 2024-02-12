#pragma once
#include <optional>

#include <ulib/runtimeerror.h>
#include <ulib/string.h>
#include <ulib/strutility.h>

#ifdef WIN32
#include <Windows.h>
#else
#include <stdlib.h>
#endif

namespace ulib
{
    ULIB_RUNTIME_ERROR(environment_variable_internal_error);

#ifdef WIN32
    constexpr static auto kEnvPathSeparator = u8";";
#else
    constexpr static auto kEnvPathSeparator = u8":";
#endif

    /**
     * @brief Get the specified environment variable.
     *
     * @param key The environment variable's name, like 'PATH' or 'HOME'
     * @return std::optional<ulib::u8string> The environment variable's value or std::nullopt
     */
    inline std::optional<ulib::u8string> getenv(ulib::u8string_view key)
    {
#ifdef WIN32
        auto size = ::GetEnvironmentVariableW(ulib::wstr(key).c_str(), nullptr, 0);

        if (size < 1)
            return std::nullopt;

        std::wstring result;
        result.resize(size);

        if (!::GetEnvironmentVariableW(ulib::wstr(key).c_str(), result.data(), result.size()))
            throw environment_variable_internal_error("failed to call GetEnvironmentVariableW but variable exists");

        // Remove the zero
        result.pop_back();

        return ulib::u8(result);

#else
        auto result = ::getenv(ulib::str(key).c_str());
        return result ? ulib::u8(result) : std::nullopt;
#endif
    }

    /**
     * @brief Sets the specified environment variable.
     *
     * @param key The environment variable's name, like 'PATH' or 'HOME'
     * @param value The environment variable's new value, or std::nullopt to delete the variable
     */
    inline void setenv(ulib::u8string_view key, std::optional<ulib::u8string> value)
    {
#ifdef WIN32
        if (!::SetEnvironmentVariableW(ulib::wstr(key).c_str(),
                                       value && !value->empty() ? ulib::wstr(*value).c_str() : nullptr))
            throw environment_variable_internal_error("failed to call SetEnvironmentVariableW");

        // Also set the cached ::getenv version of the variable - this is important
        ::_wputenv_s(ulib::wstr(key).c_str(), value && !value->empty() ? ulib::wstr(*value).c_str() : L"");
#else
        // TODO: error handling on Unix

        if (value && !value->empty())
            ::setenv(ulib::str(key).c_str(), ulib::str(*value).c_str(), 1);
        else
            ::unsetenv(ulib::str(key).c_str());
#endif
    }

    /**
     * @brief Returns the current environment path as a list of strings.
     *
     * @return ulib::list<ulib::u8string> The current environment path
     */
    inline ulib::list<ulib::u8string> getpath()
    {
        if (auto path = getenv(u8"PATH"))
            return path->split(kEnvPathSeparator);
        else
            return {};
    }

    /**
     * @brief Sets the current environment path.
     *
     * @param path The new environment path
     */
    inline void setpath(ulib::span<ulib::u8string> path)
    {
        setenv(u8"PATH", ulib::join(path, kEnvPathSeparator));
    }

    /**
     * @brief Adds the provided string to the environment path of this process.
     *
     * @param elem The string to add
     *
     * @return true if the string was added, false if it already existed
     */
    inline bool add_path(ulib::u8string_view elem)
    {
        auto path = getpath();

        if (path.find(elem) == npos)
            path.push_back(elem);
        else
            return false;

        setpath(path);
        return true;
    }

    /**
     * @brief Removes the provided string from the environment path of this process.
     *
     * @param elem The string to add
     *
     * @return true if the string was removed, false if it was not present
     */
    inline bool remove_path(ulib::u8string_view elem)
    {
        auto path = getpath();

        if (path.find(elem) != npos)
            path.remove(elem);
        else
            return false;

        setpath(path);
        return true;
    }

} // namespace ulib
