#include "pch.h"
#include "CppUnitTest.h"

#include "../media/src/core/enum_operator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace LJMP;

enum TestLevel { //}; : unsigned int {
	None = 0,
	Local = 1 << 0,
	Server = 1 << 1,
	All = Local | Server,
	E_Remove = 1 << 3,
	E_Add = 1 << 4
};

namespace Microsoft {
	namespace VisualStudio {
		namespace CppUnitTestFramework {
			template<> inline std::wstring ToString<TestLevel>(const TestLevel& t) { RETURN_WIDE_STRING(t); }
		}
	}
}

namespace enumoptest
{
	TEST_CLASS(enumoptest)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			TestLevel level = TestLevel::None;
			level |= TestLevel::Local;
			level |= TestLevel::Server;


			Assert::AreEqual(enum_cast(level & TestLevel::Local), 1);
			Assert::AreEqual(level, TestLevel::All);
			Assert::AreNotEqual(enum_cast(level & TestLevel::Local), 0);
			Assert::AreNotEqual(enum_cast(level & TestLevel::Server), 0);

			TestLevel level1 = TestLevel::None;
			level1 |= TestLevel::E_Remove;

			unsigned int test1 = enum_cast(level1 & TestLevel::E_Add);
			Logger::WriteMessage(std::to_wstring(test1).c_str());
			Assert::AreNotEqual(enum_cast(level1 & TestLevel::E_Add), 0);
		}

	};
}
