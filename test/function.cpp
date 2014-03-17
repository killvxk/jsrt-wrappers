#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace jsrtwrapperstest
{
    TEST_CLASS(function)
    {
    public:
        MY_TEST_METHOD(empty_handle, "Test an empty function handle.")
        {
            jsrt::function_base handle;
            Assert::AreEqual(handle.handle(), JS_INVALID_REFERENCE);
            Assert::IsFalse(handle.is_valid());
        }

        MY_TEST_METHOD(no_context, "Test calls with no context.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();

            jsrt::function_base base_function;
            TEST_NO_CONTEXT_CALL(jsrt::function_base::create(nullptr));
            TEST_NO_CONTEXT_CALL(base_function(jsrt::value(), {}));
            TEST_NO_CONTEXT_CALL(base_function.construct(jsrt::value(), {}));

            runtime.dispose();
        }

        MY_TEST_METHOD(invalid_handle, "Test calls with an invalid handle.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::function_base func;
                TEST_NULL_ARG_CALL(func(jsrt::value(), {}));
                TEST_NULL_ARG_CALL(func.construct(jsrt::value(), {}));
            }
            runtime.dispose();
        }

        static jsrt::value base_callback(const jsrt::call_info &info, const std::vector<jsrt::value> &arguments)
        {
            Assert::AreEqual(info.callee().type(), JsFunction);
            Assert::AreEqual(info.this_value().type(), JsObject);
            if (((jsrt::object)info.this_value()).is_external())
            {
                void *data = ((jsrt::external_object)info.this_value()).data();
                Assert::AreEqual(data, (void *) 0xdeadbeef);
                Assert::IsFalse(info.is_construct_call());
                return jsrt::number::create(2);
            }
            else
            {
                Assert::IsTrue(info.is_construct_call());
                return jsrt::external_object::create((void *)0xdeadc0de);
            }
        }

        MY_TEST_METHOD(call_info, "Test call_info class.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::call_info info;
                Assert::IsFalse(info.callee().is_valid());
                Assert::IsFalse(info.this_value().is_valid());
                Assert::IsFalse(info.is_construct_call());
                info = jsrt::call_info(jsrt::number::create(1), jsrt::number::create(2), true);
                Assert::IsTrue(info.callee().is_valid());
                Assert::IsTrue(info.this_value().is_valid());
                Assert::IsTrue(info.is_construct_call());
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(base, "Test function_base.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::function_base func = jsrt::function_base::create(base_callback);
                jsrt::value result = func(jsrt::external_object::create((void *) 0xdeadbeef), { jsrt::number::create(1) });
                Assert::AreEqual(result.type(), JsNumber);
                Assert::AreEqual(((jsrt::number)result).data(), 2.0);

                result = func.construct(jsrt::external_object::create((void *) 0xdeadc0de), { jsrt::number::create(1) });
                Assert::AreEqual(result.type(), JsObject);
                Assert::IsTrue(((jsrt::object)result).is_external());
                Assert::AreEqual(((jsrt::external_object)result).data(), (void *) 0xdeadc0de);
            }
            runtime.dispose();
        }

        static int callback8(const jsrt::call_info &info, std::wstring p1, int p2, bool p3, std::wstring p4, int p5, bool p6, std::wstring p7, int p8)
        {
            Assert::AreEqual(info.callee().type(), JsFunction);
            Assert::AreEqual(info.this_value().type(), JsObject);
            Assert::IsTrue(((jsrt::object)info.this_value()).is_external());
            void *data = ((jsrt::external_object)info.this_value()).data();
            Assert::AreEqual(data, (void *) 0xdeadbeef);
            Assert::IsFalse(info.is_construct_call());

            Assert::AreEqual(p1, (std::wstring)L"foo");
            Assert::AreEqual(p2, 2);
            Assert::AreEqual(p3, true);
            Assert::AreEqual(p4, (std::wstring)L"bar");
            Assert::AreEqual(p5, 5);
            Assert::AreEqual(p6, false);
            Assert::AreEqual(p7, (std::wstring)L"baz");
            Assert::AreEqual(p8, 8);

            return 8;
        }

        MY_TEST_METHOD(strongly_typed, "Test strongly typed functions.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                //jsrt::object this_value = jsrt::external_object::create((void *) 0xdeadbeef);
                //auto f8 = jsrt::function<int, std::wstring, int, bool, std::wstring, int, bool, std::wstring, int>::create(callback8);
                //int result = f8(this_value, L"foo", 2, true, L"bar", 5, false, L"baz", 8);
                //Assert::AreEqual(result, 8);
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(strongly_typed_constructors, "Test strongly typed constructor functions.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
            }
            runtime.dispose();
        }
    };
}