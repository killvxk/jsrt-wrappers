#include "stdafx.h"
#include <utility>
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace jsrtwrapperstest
{
    TEST_CLASS(pinned)
    {
    public:
        MY_TEST_METHOD(empty_handle, "Test a pinned empty handle.")
        {
            jsrt::pinned<jsrt::reference> handle;
            Assert::AreEqual(handle->handle(), JS_INVALID_REFERENCE);
            Assert::IsFalse(handle->is_valid());
        }

        MY_TEST_METHOD(context_handle, "Test a pinned context handle.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::pinned<jsrt::context> context = runtime.create_context();
            Assert::IsTrue(context->is_valid());
            context.release();
            Assert::IsFalse(context->is_valid());
            runtime.dispose();
        }

        MY_TEST_METHOD(handle_refcounting, "Test refcounting on a pinned handle.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::pinned<jsrt::context> pinned_context = context;
                Assert::AreEqual(context.add_reference(), (unsigned int) 2);
                jsrt::pinned<jsrt::context> another_pinned_context = pinned_context;
                jsrt::pinned<jsrt::context> yet_another_pinned_context = std::move(pinned_context);
                Assert::IsTrue(yet_another_pinned_context->is_valid());
                Assert::AreEqual(context.release(), (unsigned int) 2);
            }
            Assert::AreEqual(context.add_reference(), (unsigned int) 1);
            context.release();
            runtime.dispose();
        }
    };
}