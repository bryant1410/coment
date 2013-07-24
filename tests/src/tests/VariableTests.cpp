#include "tests/VariableTests.h"

#include <coment/World.h>

namespace coment
{
	namespace tests
	{
		VariableTests::VariableTests()
			: Test("Variable")
		{

		}

		void VariableTests::run()
		{
			const int TEST_INT = 9000;
			const float TEST_FLOAT = 0.1f;

			coment::World world;

			begintest("New variable is default initialised");
			endtest(world.getValue<TestStruct>("test").value == TestStruct().value);

			begintest("Custom value can be set");
				world.setValue<TestStruct>("test", TestStruct(TEST_INT));
			endtest(world.getValue<TestStruct>("test").value == TEST_INT);

			begintest("Setting world delta affects VariableManager");
				world.setDelta(TEST_FLOAT);
			endtest(world.getDelta() == TEST_FLOAT && world.getValue<float>("delta") == TEST_FLOAT);

			return;
		}
	}
}
