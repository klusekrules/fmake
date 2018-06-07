#include "stdafx.h"
#include "CppUnitTest.h"
#include "../src/Resolver.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace fmake
{		
	TEST_CLASS(TestResolver)
	{
	public:
		
		TEST_METHOD(TestAddParam)
		{
			Resolver r;
			Assert::IsTrue(r.addParam("name", "value"));
		}

	};
}