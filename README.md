# ulib-env - C++ environment variables done right

**ulib-env** is a platform-agnostic single-header library to access environment variables in C++.

ulib-env is part of [ulib](https://github.com/k2rate/ulib) - a collection of fast, cross-platform utility classes for C++.

## Features
- **Cross-platform API:** you can now get AND set environment variables without any platform-specific API calls!
- **PATH support:** ulib-env includes helper functions to manipulate the `$PATH` environment variable *as a list of paths* in a cross-platform way.
- **Fully tested:** every feature of ulib-env is covered by unit tests.

## Usage
```cpp
#include <ulib/env.h>
#include <cstdio>

int main()
{
    // Will throw std::bad_optional_access if the variable is missing from the environment
    const auto api_key = ulib::getenv("FOO_SERVICE_API_KEY").value();

    // Will set the variable in this process and any subsequently spawned child processes
    ulib::setenv("IS_RUNNING_FOO_CLIENT", "true");
    
    // Will remove the variable from this process
    ulib::setenv("IS_RUNNING_FOO_CLIENT", std::nullopt);

    // Will list all directories in $PATH
    for (const auto& p : ulib::getpath())
    {
        std::printf("%s\n", p.c_str());
    }

    // Will add the specified directory to $PATH
    ulib::add_path("/home/bob/new/path/bin");

    // Will remove the specified directory from $PATH
    ulib::remove_path("/home/bob/new/path/bin");
}
```

## Installation
Start using ulib-env in one line via [Re](https://github.com/osdeverr/rebs) - *the* build system for C++:
```yaml
# In your target's 're.yml' file:

deps:
    # <other dependencies...>
    - github:osdeverr/ulib-env ^1.0.0
```

Re will take care of dependencies, include paths and everything else automatically.

Alternatively, you can add ulib-env and [ulib](https://github.com/k2rate/ulib) (which ulib-env depends on) as Git submodules to your project using another build system.

## API Reference
```cpp
namespace ulib
{
    /**
     * @brief Get the specified environment variable.
     *
     * @param key The environment variable's name, like 'PATH' or 'HOME'
     * @return std::optional<ulib::u8string> The environment variable's value or std::nullopt
     */
    std::optional<ulib::u8string> getenv(ulib::u8string_view key);

    /**
     * @brief Sets the specified environment variable.
     *
     * @param key The environment variable's name, like 'PATH' or 'HOME'
     * @param value The environment variable's new value, or std::nullopt to delete the variable
     */
    void setenv(ulib::u8string_view key, std::optional<ulib::u8string> value);

    /**
     * @brief Returns the current environment path as a list of strings.
     *
     * @return ulib::list<ulib::u8string> The current environment path
     */
    ulib::list<ulib::u8string> getpath();
    
    /**
     * @brief Adds the provided string to the environment path of this process.
     *
     * @param elem The string to add
     *
     * @return true if the string was added, false if it already existed
     */
    bool add_path(ulib::u8string_view elem);

    /**
     * @brief Removes the provided string from the environment path of this process.
     *
     * @param elem The string to add
     *
     * @return true if the string was removed, false if it was not present
     */
    bool remove_path(ulib::u8string_view elem);
}
```
