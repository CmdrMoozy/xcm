#define CATCH_CONFIG_RUNNER
#include <catch/catch.hpp>

int main(int argc, char *const argv[])
{
	return Catch::Session().run(argc, argv);
}
