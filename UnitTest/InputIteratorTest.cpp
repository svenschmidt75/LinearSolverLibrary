#include "pch.h"

#include "InputIteratorTest.h"


using namespace LinearSolverLibrary_NS;


void
InputIteratorTest::setUp() {}

void
InputIteratorTest::tearDown() {}

void
InputIteratorTest::TestForwardIterator() {
    VariableSet variable_set;
    variable_set.add(1);
    variable_set.add(2);
    variable_set.add(3);
    variable_set.add(4);
    variable_set.add(5);
    auto it = std::begin(variable_set);
    decltype(*it) expected = 1;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Iterator error", expected, *it);
}

void
InputIteratorTest::TestPostIncrement() {
    VariableSet variable_set;
    variable_set.add(1);
    variable_set.add(2);
    variable_set.add(3);
    variable_set.add(4);
    variable_set.add(5);
    auto it = std::begin(variable_set);
    it++;
    decltype(*it) expected = 2;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Iterator error", expected, *it);
}

void
InputIteratorTest::TestPostIncrementWithAssignment() {
    VariableSet variable_set;
    variable_set.add(1);
    variable_set.add(2);
    variable_set.add(3);
    variable_set.add(4);
    variable_set.add(5);
    auto it = std::begin(variable_set);
    auto it_prev = it++;
    decltype(*it) expected = 1;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Iterator error", expected, *it_prev);
}

void
InputIteratorTest::TestPreIncrement() {
    VariableSet variable_set;
    variable_set.add(1);
    variable_set.add(2);
    variable_set.add(3);
    variable_set.add(4);
    variable_set.add(5);
    auto it = std::begin(variable_set);
    ++it;
    decltype(*it) expected = 2;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Iterator error", expected, *it);
}

void
InputIteratorTest::TestPreIncrementWithAssignment() {
    VariableSet variable_set;
    variable_set.add(1);
    variable_set.add(2);
    variable_set.add(3);
    variable_set.add(4);
    variable_set.add(5);
    auto it = std::begin(variable_set);
    auto it_prev = ++it;
    decltype(*it) expected = 2;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Iterator error", expected, *it);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Iterator error", expected, *it_prev);
}

void
InputIteratorTest::TestIteratorEquality() {
    VariableSet variable_set;
    variable_set.add(1);
    variable_set.add(2);
    variable_set.add(3);
    variable_set.add(4);
    variable_set.add(5);
    auto it = std::begin(variable_set);
    auto it_prev = ++it;
    CPPUNIT_ASSERT_MESSAGE("Iterator error", it == it_prev);
}

void
InputIteratorTest::TestIteratorInEquality() {
    VariableSet variable_set;
    variable_set.add(1);
    variable_set.add(2);
    variable_set.add(3);
    variable_set.add(4);
    variable_set.add(5);
    auto it = std::begin(variable_set);
    auto it_prev = it++;
    CPPUNIT_ASSERT_MESSAGE("Iterator error", it != it_prev);
}

void
InputIteratorTest::TestIteratorCopyConstruction() {
    VariableSet variable_set;
    variable_set.add(1);
    variable_set.add(2);
    variable_set.add(3);
    variable_set.add(4);
    variable_set.add(5);
    auto it = std::begin(variable_set);
    auto it_prev = it;
    CPPUNIT_ASSERT_MESSAGE("Iterator error", it == it_prev);

    ++it;
    decltype(*it) expected = 2;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Iterator error", expected, *it);
    expected = 1;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Iterator error", expected, *it_prev);
}

void
InputIteratorTest::TestIteratorAssignment() {
    VariableSet variable_set;
    variable_set.add(1);
    variable_set.add(2);
    variable_set.add(3);
    variable_set.add(4);
    variable_set.add(5);
    auto it = std::begin(variable_set);
    decltype(it) it_prev(std::end(variable_set));
    it_prev = it;
    CPPUNIT_ASSERT_MESSAGE("Iterator error", it == it_prev);

    ++it;
    decltype(*it) expected = 2;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Iterator error", expected, *it);
    expected = 1;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Iterator error", expected, *it_prev);
}

void
InputIteratorTest::TestIteratorEnd() {
    VariableSet variable_set;
    variable_set.add(1);
    variable_set.add(2);
    variable_set.add(3);
    variable_set.add(4);
    variable_set.add(5);
    auto it = std::begin(variable_set);
    auto it_end = std::end(variable_set);
    for (size_t i = 0; it != it_end; ++i, ++it)
        ;
    decltype(*it) expected = 5;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Iterator end error", expected, 5ull);
}
