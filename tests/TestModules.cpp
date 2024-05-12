#include <catch_amalgamated.hpp>

#include "Hook.h"

class TestModule : public Module
{
public:
	int GetValue() const noexcept { return 42; }
};

TEST_CASE("can register and get modules")
{
	auto& hook = Hook::GetInstance();
	hook.RegisterModule<TestModule>();

	auto test = hook.GetModule<TestModule>();

	REQUIRE_FALSE(test == nullptr);

	auto value = test->GetValue();

	REQUIRE(value == 42);
}