#include <gtest/gtest.h>
#include <iostream>
#include <ulib/env.h>

TEST(UlibEnv, CanGetAndSetEnv)
{
    constexpr auto kVar1 = u8"ULIB_ENV_TEST_UNIQUE_VAR";
    constexpr auto kVar1Val = u8"foobar123";

    // The variable is not yet present, so it's supposed to return nullopt
    ASSERT_EQ(ulib::getenv(kVar1), std::nullopt);

    // Set the variable to the test value
    ASSERT_NO_THROW(ulib::setenv(kVar1, kVar1Val));

    // Make sure it is in fact equal to the test value now
    ASSERT_EQ(ulib::getenv(kVar1), kVar1Val);

    // Remove the value completely
    ASSERT_NO_THROW(ulib::setenv(kVar1, std::nullopt));

    // Make sure it returns nullopt once again
    ASSERT_EQ(ulib::getenv(kVar1), std::nullopt);
}

TEST(UlibEnv, CanGetAndSetPath)
{
    constexpr auto kDir = u8"/ulib-env-test/some/unique/directory";

    ASSERT_TRUE(ulib::add_path(kDir));
    ASSERT_FALSE(ulib::add_path(kDir)); // Can't add it more than once!

    ASSERT_TRUE(ulib::getpath().find(kDir) != ulib::npos); // It's in the path now

    ASSERT_TRUE(ulib::remove_path(kDir));
    ASSERT_FALSE(ulib::remove_path(kDir)); // Can't remove it more than once either!
}
