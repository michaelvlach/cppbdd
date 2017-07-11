#pragma once

#include <QTest>
#include <QDebug>
#include <QString>

namespace qtestbdd
{
    class Scenario
    {
    public:
        Scenario(const QString &description) :
            mDescription(description)
        {
            qInfo() << "SCENARIO" << description;
        }

        void given(const QString &description)
        {
            mGiven = true;
            printGiven(description);
        }

        void when(const QString &description)
        {
            if(mGiven)
            {
                mWhen = true;
                printWhen(description);
            }
            else
            {
                QFAIL("GIVEN clause missing.");
            }
        }

        void then(const QString &description)
        {
            if(mWhen)
            {
                mThen = true;
                printThen(description);
            }
            else
            {
                QFAIL("WHEN clause missing.");
            }
        }

        void et(const QString &description)
        {
            if(mGiven || mWhen || mThen)
            {
                printAnd(description);
            }
            else
            {
                QFAIL("AND must follow one of GIVEN, WHEN or THEN");
            }
        }

    private:
        void printError(const QString &message)
        {
            qError() << mDescription << " " << message;
        }

        void printScenario()
        {
            qInfo() << "    SCENARIO " << mDescription;
        }

        void printGiven(const QString &description)
        {
            qInfo() << mIndentStep << "GIVEN " << description;
        }

        void printWhen(const QString &description)
        {
            qInfo() << mIndentStep << "WHEN " << description;
        }

        void printThen(const QString &description)
        {
            qInfo() << mIndentStep << "THEN " << description;
        }

        void printAnd(const QString &description)
        {
            qInfo() << mIndentStep << "  AND " << description;
        }

        const QString mDescription;
        const QString mIndentStep = "        ";
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
    qInfo() << "FEATURE " << description;

#define SCENARIO(description)\
    qtestbdd::Scenario s(description);

#define GIVEN(description)\
    given(description);

#define WHEN(description)\
    when(description);

#define THEN(description)\
    then(description);

#define AND(description)\
    et(description);
