#pragma once

#include "gtest/gtest.h"
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
                printError("missing WHEN clause");
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
            std::cout << "    SCENARIO " << mDescription << std::endl;
        }

        void printGiven(const std::string &description)
        {
            std::cout << mIndentStep << "GIVEN " << description << std::endl;
        }

        void printWhen(const std::string &description)
        {
            std::cout << mIndentStep << "WHEN " << description << std::endl;
        }

        void printThen(const std::string &description)
        {
            std::cout << mIndentStep << "THEN " << description << std::endl;
        }
        
        void printAnd(const std::string &description)
        {
            std::cout << mIndentStep << "  AND" << description << std::endl; 
        }

        const std::string mDescription;
        const std::string mIndentStep = "        ";
        bool mGiven = false;
        bool mWhen = false;
        bool mThen = false;
    };
}

//utility
#define CONCATENATE_DETAIL(x, y) x##y
#define CONCATENATE(x, y) CONCATENATE_DETAIL(x, y)
#define MAKE_UNIQUE(x) CONCATENATE(x, __LINE__)

//gtestbdd
#define FEATURE(description)\
    TEST(MAKE_UNIQUE(Feature), Init)\
    {\
        std::cout << "FEATURE " << description << std::endl;\
    }\
    namespace MAKE_UNIQUE(Feature)

#define SCENARIO(description, fixture)\
    class MAKE_UNIQUE(Scenario_##fixture##_Line) : public gtestbdd::Scenario, public fixture\
    {\
    public:\
        MAKE_UNIQUE(Scenario_##fixture##_Line)() :\
            gtestbdd::Scenario(description)\
        {\
        }\
    };\
    TEST_F(MAKE_UNIQUE(Scenario_##fixture##_Line), MAKE_UNIQUE(Scenario_Line))

#define GIVEN(description)\
    given(description);

#define WHEN(description)\
    when(description);

#define THEN(description)\
    then(description);

#define AND(description)\
    et(description)
