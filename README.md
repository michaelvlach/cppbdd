# CppBdd #
CppBdd is a [BDD](https://en.wikipedia.org/wiki/Behavior-driven_development) style unit test extension for popular C++ unit testing frameworks licensed under MIT open source license.


## Introduction ##
There are number of unit testing frameworks available like [Google Test]( https://github.com/google/googletest ), CppUnit, QTest, Catch and many others. Only few of them support BDD in some form (e.g. Catch, Igloo) and typically come as their own framework making it harder to adopt for projects that are already using something else. CppBdd is a collection of extensions for popular unit testing frameworks that offer
writing in BDD style inside your existing framework. So you can mix approaches or adopt BDD gradually or just take it for a spin and see how things turn out.


## Overview ##

* include/cppbdd

Contains header-only implementation of BDD for the respective unit testing framework.

* examples/

Contains example source files demonstrating the usage. Requires the respective framework to compile.


## Supported Frameworks ##

* [Google Test]( https://github.com/google/googletest )

Available as **include/cppbdd/gtestbdd.h**

* [QTest]( http://doc.qt.io/qt-5/qtest-overview.html )

Available as **include/cppbdd/qtestbdd.h**

* [CppUnit]( https://sourceforge.net/projects/cppunit/ )

WIP

## Requirements ##

* C++11 capable compiler
* Unit testing framework of your choosing (that is supported by cppbdd, see above)


## Reference ##

* FEATURE(description)

(Optional) Scenarios are typically bundled into features. This lets you declare the feature.

* SCENARIO_F(description, fixture)

Declares the test using the fixture. Must be followed by {} that will hold all of the steps.

* GIVEN(description)

(Mandatory) Description of first step of the scenario typically setting things up. Code can precede this if it does not fit into its description.

* WHEN(description)

(Mandatory) Description of action that is being taken. Must follow at least one GIVEN.

* THEN(description)

(Mandatory) Verification of outputs.

* AND

(Optional) Can follow any of GIVEN, WHEN or THEN. Cannot be first. Helps separate logical sections of the scenario or given step.


## Usage ##

CppBdd is header only based set of macros that are named after BDD custom and enforces structuring of tests for human readability and understandability: 

Example using BDD with Google Test:

```cpp
#include <cppbdd/gtestbdd.h>

class Fixture : public ::testing::Test
{
};

FEATURE("BDD style unit tests with Google Test")

SCENARIO_F("Demonstratrating core features", Fixture)
{
    //indentation is there for readability but is otherwise irrelevant
    int x, y;

    GIVEN("Value of 1")
        x = 1;
    AND("Value of 2")
        y = 2;
    WHEN("The values are added together")
        int result = x + y;
    THEN("The result should be 3")
        ASSERT_EQ(3, result);
}
```

What we did here is creating a `Fixture` class that glues our tests together providing for example mock objects or other initializations. It can of course contain usual Google Test SetUpTestCase, SetUp, TearDown and TearDownTestCase methods and they will be executed as usual. SCENARIO_F then declares TEST_F as you would normally adding the ability to call BDD style methods that track the scenario making sure it is well formed (correct sequence of steps) and contain all the required steps.

## Framework Specific Notes ##

* Google Test

### Parameterized SCENARIO

Parameterized scenarios require a fixture and can be achieved using either of 

```cpp
SCENARIO_P("This is my scenario description", MyUniqueFixture, Values()) {
}
```
or
```cpp
SCENARIO_P_VERBOSE(ExplicitTestName, "This is my scenario description", MyFixture, Values(..)) {
}
```

Both of these macros deal with `INSTANTIATE_TEST_CASE_P` so that it is not required in your tests.
The difference between them is just what appears in the test output. `SCENARIO_P` will by default prefix test output with
`TEST_P` and presumes the fixture name and description are sufficient enough to give meaningful test output.
Where this is not the case, you can substitute additional test information for the `ExplicitTestName` placeholder.

Currently fixtures need to be unique to the parameterized scenario, otherwise it can lead to cross-talk between tests that share
fixtures in a way that may not be desirable. The simplest way to avoid this happening in the case where a fixture is shared
is to prefix `SCENARIO_P` with

```cpp
using UniqueFixtureName = SharedFixture;
```

for example 

```cpp
using UniqueFixtureName = SharedFixture;
const auto emails = Values("£not#valid!!", "valid@email.com")
SCENARIO_P("User updates their email address", UniqueFixtureName, emails) {
}
```

### DISABLED tests

To disable a `SCENARIO`, just prefix the scenario string with `"DISABLED_"` as you would for a regular TEST
In the output you will see the number of tests disabled as usual.

```bash
  YOU HAVE 1 DISABLED TESTS
```

* QTest

BDD style macros do not replace or wrap any functionality of QTest and you write the infrastructure of the test as usual. FEATURE should typically be placed into the setUpTestCase() method while SCENARIO at the top of each test method you want to BDD-ify. By declaring SCENARIO in any of your tests you effectively makes it into a BDD test and enables the features listed above, e.g.:

```
class Test : public QObject 
{ 
    Q_OBJECT
	Q_SLOT void setUpTestCase() { FEATURE("My awesome feature") }
	Q_SLOT void myTest()
	{
		SCENARIO("My testing scenario")
		//Now you shall provide GIVEN, WHEN and THEN
	}
};
```

* CppUnit

None
