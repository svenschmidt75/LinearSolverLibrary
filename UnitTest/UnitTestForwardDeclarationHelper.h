/*
 * Name  : UnitTestForwardDeclarationHelper
 * Path  : 
 * Use   : Used for forward declaraing of Google tests.
 * Author: Sven Schmidt
 * Date  : 07/04/2014
 */
#pragma once


#define GOOGLE_TEST(TestFixture, TestName) class TestFixture##_##TestName##_##Test;
#define FRIEND_GOOGLE_TEST(TestFixture, TestName) friend GOOGLE_TEST(TestFixture, TestName)
