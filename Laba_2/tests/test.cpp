#include <gtest/gtest.h>
#include "../include/octal.h"
#include <string>

TEST(ArrayTest, Constructors) {
    Array a;  // default
    EXPECT_EQ(a.size(), 0);

    Array b(3, 5);
    EXPECT_EQ(b.size(), 3);
    EXPECT_EQ(b[0], 5);
    EXPECT_EQ(b[1], 5);
    EXPECT_EQ(b[2], 5);

    Array c({1, 2, 3});
    EXPECT_EQ(c.size(), 3);
    EXPECT_EQ(c[0], 1);
    EXPECT_EQ(c[1], 2);
    EXPECT_EQ(c[2], 3);

    Array d("abc");
    EXPECT_EQ(d.size(), 3);
    EXPECT_EQ(static_cast<int>(d[0]), static_cast<int>('a'));
    EXPECT_EQ(static_cast<int>(d[1]), static_cast<int>('b'));

    Array e = b;  // copy
    EXPECT_EQ(e.size(), 3);
    EXPECT_EQ(e[0], 5);

    Array f = std::move(b);  // move
    EXPECT_EQ(f.size(), 3);
    EXPECT_EQ(f[0], 5);
    EXPECT_EQ(b.size(), 0);
}

TEST(ArrayTest, OperationsAndExceptions) {
    Array a(2, 1);
    Array b(2, 2);

    Array sum = a + b;
    EXPECT_EQ(sum[0], 3);
    EXPECT_EQ(sum[1], 3);

    Array diff = a - b;
    EXPECT_EQ(diff[0], 255U);  // unsigned underflow
    EXPECT_EQ(diff[1], 255U);

    Array prod = a * b;
    EXPECT_EQ(prod[0], 2);
    EXPECT_EQ(prod[1], 2);

    Array neg = -a;
    EXPECT_EQ(neg[0], 254U);  // ~1
    EXPECT_EQ(neg[1], 254U);

    Array pos = +a;
    EXPECT_EQ(pos[0], 1);

    EXPECT_THROW(a[2], std::out_of_range);

    Array c(1, 0);
    EXPECT_THROW(a + c, std::invalid_argument);
}

TEST(OctalTest, Constructors) {
    Octal zero;
    EXPECT_EQ(zero.to_string(), "0");
    EXPECT_EQ(zero.size(), 1);
    EXPECT_EQ(zero[0], 0);

    Octal one("1");
    EXPECT_EQ(one.to_string(), "1");
    EXPECT_EQ(one.size(), 1);
    EXPECT_EQ(one[0], 1);

    Octal ten("10");
    EXPECT_EQ(ten.to_string(), "10");
    EXPECT_EQ(ten.size(), 2);
    EXPECT_EQ(ten[0], 0);
    EXPECT_EQ(ten[1], 1);

    EXPECT_THROW(Octal("8"), std::invalid_argument);  // Invalid digit
}

TEST(OctalTest, Operations) {
    Octal a("10");  // 8 dec
    Octal b("7");

    Octal sum = a + b;  // 15 dec = 17 oct
    EXPECT_EQ(sum.to_string(), "17");
    EXPECT_EQ(sum[0], 7);
    EXPECT_EQ(sum[1], 1);

    Octal diff = a - b;  // 1 dec = 1 oct
    EXPECT_EQ(diff.to_string(), "1");

    Octal prod = a * b;  // 56 dec = 70 oct
    EXPECT_EQ(prod.to_string(), "70");
    EXPECT_EQ(prod[0], 0);
    EXPECT_EQ(prod[1], 7);

    Octal neg = -a;  // Inversion example:for "10" -> digits[0]=0->7, [1]=1->6 -> "67"
    EXPECT_EQ(neg.to_string(), "67");

    Octal pos = +a;
    EXPECT_EQ(pos.to_string(), "10");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}