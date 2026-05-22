#include <gtest/gtest.h>

extern "C" int add(int left, int right);

// Demonstrate some basic assertions.
TEST(RustLibTest, AddTest) {
  // Expect equality.
  int result = add(2, 2);
  EXPECT_EQ(result, 4);
}
