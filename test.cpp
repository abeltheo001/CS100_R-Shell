#include "gtest/gtest.h"
#include "unit_tests/splitOnCharTest.h"
#include "unit_tests/shuntingTest.h"

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
