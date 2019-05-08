#pragma once

#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <cassert>

namespace gtestbdd
{
    class Scenario
    {
    public:
        Scenario(const std::string &description) :
            mDescription(description)
        {
            printScenario();
        }

        virtual ~Scenario()
        {
            if (!mGiven && !mWhen && !mThen)
            {
                return;
            }

            if(!mGiven)
            {
                printError("GIVEN clause missing.");
                assert(false);
            }

            if(!mWhen)
            {
                printError("WHEN clause missing.");
                assert(false);
            }

            if(!mThen)
            {
                printError("THEN clause missing.");
                assert(false);
            }
        }

        void given(const std::string &description)
        {
            mGiven = true;
            printGiven(description);
        }

        void when(const std::string &description)
        {
            if(mGiven)
            {
                mWhen = true;
                printWhen(description);
            }
            else
            {
                printError("GIVEN clause missing.");
                assert(false);
            }
        }

        void then(const std::string &description)
        {
            if(mWhen)
            {
                mThen = true;
                printThen(description);
            }
            else
            {
                printError("WHEN clause missing");
                assert(false);
            }
        }

        void et(const std::string &description)
        {
            if(mGiven || mWhen || mThen)
            {
                printAnd(description);
            }
            else
            {
                printError("AND must follow one of GIVEN, WHEN or THEN");
                assert(false);
            }
        }

    private:
        void printError(const std::string &message)
        {
            std::cout << "ERROR " << mDescription << " " << message << std::endl;
        }

        void printScenario()
        {
            std::cout << std::right << std::setw(16) << "SCENARIO " << mDescription << std::endl;
        }

        void printGiven(const std::string &description)
        {
            std::cout << std::right << std::setw(16) << "GIVEN " << description << std::endl;
        }

        void printWhen(const std::string &description)
        {
            std::cout << std::right << std::setw(16) << "WHEN " << description << std::endl;
        }

        void printThen(const std::string &description)
        {
            std::cout << std::right << std::setw(16) << "THEN " << description << std::endl;
        }

        void printAnd(const std::string &description)
        {
            std::cout << std::right << std::setw(16) << "AND " << description << std::endl;
        }

        const std::string mDescription;
        bool mGiven = false;
        bool mWhen = false;
        bool mThen = false;
    };
}

//utility
#define CONCATENATE_DETAIL(x, y) x##y
#define CONCATENATE(x, y) CONCATENATE_DETAIL(x, y)
#define MAKE_UNIQUE(x) CONCATENATE(x, __LINE__)
#define GTEST_STRINGIFY_(name) #name

//gtestbdd
#define FEATURE(description)\
    TEST(MAKE_UNIQUE(Feature), Init)\
    {\
        std::cout << "FEATURE " << description << std::endl;\
    }\
    namespace MAKE_UNIQUE(Feature)


#define MAKE_SCENARIO(TestClass, description, FixtureClass) \
    class TestClass : public gtestbdd::Scenario, public FixtureClass\
    {\
    public:\
        TestClass() :\
            gtestbdd::Scenario(description)\
        {\
        }\
    private:\
        virtual void TestBody();\
        static ::testing::TestInfo* const test_info_ GTEST_ATTRIBUTE_UNUSED_;\
        GTEST_DISALLOW_COPY_AND_ASSIGN_(TestClass);\
    };\
    \
    ::testing::TestInfo* const TestClass\
      ::test_info_ =\
        ::testing::internal::MakeAndRegisterTestInfo(\
            #FixtureClass, description, NULL, NULL, \
            ::testing::internal::CodeLocation(__FILE__, __LINE__), \
            (::testing::internal::GetTypeId<FixtureClass>()), \
            FixtureClass::SetUpTestCase, \
            FixtureClass::TearDownTestCase, \
            new ::testing::internal::TestFactoryImpl<\
            TestClass>);\
    void TestClass::TestBody()



#define MAKE_SCENARIO_P(TestClass, TestName, Description, FixtureClass, Values)\
    class TestClass : public gtestbdd::Scenario, public FixtureClass\
    {\
    public:\
        TestClass() :\
            gtestbdd::Scenario(Description)\
        {\
        }\
        virtual void TestBody();\
    private:\
        static int AddToRegistry() {\
            ::testing::UnitTest::GetInstance()->parameterized_test_registry().\
                GetTestCasePatternHolder<FixtureClass>(\
                    #FixtureClass, __FILE__, __LINE__)->AddTestPattern(\
                        #FixtureClass,\
                        Description,\
                        new ::testing::internal::TestMetaFactory<TestClass>());\
            return 0;\
        }\
        static int gtest_registering_dummy_;\
        GTEST_DISALLOW_COPY_AND_ASSIGN_(TestClass);\
    };\
    \
    int TestClass::gtest_registering_dummy_ =\
        TestClass::AddToRegistry();\
    INSTANTIATE_TEST_CASE_P(TestName, FixtureClass, Values);\
    void TestClass::TestBody()

#define SCENARIO_P_VERBOSE(testname, description, fixture, values) \
    MAKE_SCENARIO_P(\
        MAKE_UNIQUE(Scenario_##fixture##_Line), \
        testname, \
        description, \
        fixture, \
        values)

#define SCENARIO_P(description, fixture, values) \
    SCENARIO_P_VERBOSE(\
        TEST_P, \
        description, \
        fixture, \
        values)

#define SCENARIO_F(description, fixture) MAKE_SCENARIO(MAKE_UNIQUE(Scenario_##fixture##_Line), description, fixture)

#define SCENARIO(description) MAKE_SCENARIO(MAKE_UNIQUE(Scenario_Fixture_Line), description, ::testing::Test)


#define GIVEN(description)\
    given(description);

#define WHEN(description)\
    when(description);

#define THEN(description)\
    then(description);

#define AND(description)\
    et(description);
