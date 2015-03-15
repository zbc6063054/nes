#include "gtest/gtest.h"

TEST(firsttest, dull) {
    EXPECT_EQ(2, 3);
}

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
