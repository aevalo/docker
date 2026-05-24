#include <gtest/gtest.h>

#include "rust_lib/rust_lib.h"

// Demonstrate some basic assertions.
TEST(RustLibTest, AddTest) {
  // Expect equality.
  int result = add(2, 2);
  EXPECT_EQ(result, 4);
}
