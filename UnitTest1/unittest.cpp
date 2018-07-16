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

		TEST_METHOD(TestResolveParams)
		{
			Resolver r;
			Assert::IsTrue(r.addParam("name", "nam"));
			Assert::IsTrue(r.addParam("surname", "sur"));
			Assert::IsTrue(r.addParam("Mr", "{{name}} {{surname}}"));
			Assert::IsTrue(r.resolveParams());
			Assert::AreEqual(r.getParam("Mr").value, std::string("nam sur"));
		}

		TEST_METHOD(TestResolveStream)
		{
			Resolver r;
			std::stringstream i;
			std::stringstream o;
			i << "My name is {{name}} {{surname}}\n";
			Assert::IsTrue(r.addParam("name", "nam"));
			Assert::IsTrue(r.addParam("surname", "sur"));
			Assert::IsTrue(r.resolveParams());
			r.resolveStream(i, o);
			Assert::AreEqual(o.str(), std::string("My name is nam sur\n"));
		}
	};
}