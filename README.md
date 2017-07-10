# CppBdd #
CppBdd is a [BDD](https://en.wikipedia.org/wiki/Behavior-driven_development) style unit test extension for popular C++ unit testing frameworks licensed under MIT open source license.


## Introduction ##
There are number of unit testing frameworks available like Google Test, CppUnit, QTest, Catch and many others. Only few of them support BDD in some form (e.g. Catch, Igloo) and typically come as their own framework making it harder to adopt for projects that are already using something else. CppBdd is a collection of extensions for popular unit testing frameworks that offer
writing in BDD style inside your existing framework. So you can mix approaches or adopt BDD gradually or just take it for a spin and see how things turn out.


## Overview ##

* include/cppbdd

Contains header-only implementation of BDD for the respective unit testing framework.

* examples/

Contains example source files demonstrating the usage. Requires the respective framework to compile.


## Supported Frameworks ##

* Google Test

Available as **include/cppbdd/gtestbdd.h**


## Requirements ##

* C++11 capable compiler
* Unit testing framework of your choosing (that is supported by cppbdd)


## Reference ##

* FEATURE(description)

(Optional) Scenarios are typically bundled into features. This lets you declare the feature.

* SCENARIO(description, fixture)

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

```
#include <cppbdd/gtestbdd.h>

class Fixture : public ::testing::Test
{
};

FEATURE("BDD style unit tests with Google Test")

SCENARIO("Demonstratrating core features", Fixture)
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

What we did here is creating a `Fixture` class that glues our tests together providing for example mock objects or other initializations. It can of course contain usual Google Test SetUpTestCase, SetUp, TearDown and TearDownTestCase methods and they will be executed as usual. SCENARIO then declares TEST_F as you would normally adding the ability to call BDD style methods that track the scenario making sure it is well formed (correct sequence of steps) and contain all the required steps.
