// Copyright 2015 Paul Vick
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace jsrtwrapperstest
{
    TEST_CLASS(data_view)
    {
    public:
        MY_TEST_METHOD(empty_handle, "Test an empty data_view handle.")
        {
            jsrt::data_view<> handle;
            Assert::AreEqual(handle.handle(), JS_INVALID_REFERENCE);
            Assert::IsFalse(handle.is_valid());
        }

        MY_TEST_METHOD(no_context, "Test calls with no context.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            jsrt::array_buffer buffer;
            {
				jsrt::context::scope scope(context);
				buffer = jsrt::array_buffer::create(256);
            }
            jsrt::data_view<> view;

			// JsCreateDataView
            TEST_NO_CONTEXT_CALL(jsrt::data_view<>::create(buffer, 0, 0));
			{
				jsrt::context::scope scope(context);
				view = jsrt::data_view<>::create(buffer, 0, 0);
			}

			// JsGetDataViewStorage doesn't require a context
			view.data();
            view.size();
            runtime.dispose();
        }

        MY_TEST_METHOD(invalid_handle, "Test calls with an invalid handle.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::data_view<> view;
                TEST_INVALID_ARG_CALL(view.size());
                TEST_INVALID_ARG_CALL(view.data());
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(create, "Test ::create method.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::array_buffer buffer = jsrt::array_buffer::create(0);
                jsrt::value value = jsrt::data_view<>::create(buffer, 0, 0);
                static_cast<jsrt::data_view<>>(value);
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(data, "Test data and size.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::array_buffer buffer = jsrt::array_buffer::create(20);
                jsrt::data_view<> view = jsrt::data_view<>::create(buffer, 5, 10);
                Assert::AreEqual(view.size(), 10u);
                Assert::IsNotNull(view.data());
            }
            runtime.dispose();
        }

        MY_TEST_METHOD(accessors, "Test get and set methods.")
        {
            jsrt::runtime runtime = jsrt::runtime::create();
            jsrt::context context = runtime.create_context();
            {
                jsrt::context::scope scope(context);
                jsrt::array_buffer buffer = jsrt::array_buffer::create(12);
                jsrt::data_view<> view = jsrt::data_view<>::create(buffer);
                view.set<int>(0, 42);
                Assert::AreEqual(view.get<int>(0), 42);
                view.set<double>(4, 42.0);
                Assert::AreEqual(view.get<double>(4), 42.0);
            }
            runtime.dispose();
        }
    };
}