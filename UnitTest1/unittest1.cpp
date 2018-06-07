#include "stdafx.h"
#include "CppUnitTest.h"
#include "../src/Resolver.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			Resolver r;
			r.addParam("name", "value");
		}

	};
}