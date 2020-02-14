#include "gtest/gtest.h"
#include "unit_tests/splitOnCharTest.h"
#include "unit_tests/filterCommentsTest.h"
#include "unit_tests/groupQuotesTest.h"
#include "unit_tests/tokenizeTest.h"
#include "unit_tests/constructExpressionTreeTest.h"
#include "unit_tests/executeVectorTest.h"
#include "unit_tests/checkBuiltinTest.h"
#include "integration_tests/parserTest.h"

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
