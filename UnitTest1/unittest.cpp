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

		TEST_METHOD(TestGetParam)
		{
			Resolver r;
			Assert::IsTrue(r.addParam("name", "value"));
			Assert::AreEqual(r.getParam("name").name, std::string("name"));
			Assert::AreEqual(r.getParam("name").value, std::string("value"));
		}

		TEST_METHOD(TestCreateAndGetParam)
		{
			std::stringstream i;
			i << "value\n";
			Resolver r(std::cout, i);
			Assert::AreEqual(r.getParam("name").name, std::string("name"));
			Assert::AreEqual(r.getParam("name").value, std::string("value"));
		}
	};
}