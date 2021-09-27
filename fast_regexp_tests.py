import unittest
from fast_regexps import *


class SmallRegexp(unittest.TestCase):
    """a"""

    r = char("a")

    def test1(self):
        self.assertEqual(self.r.matches(""), False)

    def test2(self):
        self.assertEqual(self.r.matches("b"), False)

    def test3(self):
        self.assertEqual(self.r.matches("a"), True)


class LongRegexp(unittest.TestCase):
    """a*"""

    r = star(char("a"))

    def test1(self):
        self.assertEqual(self.r.matches(""), True)

    def test2(self):
        self.assertEqual(self.r.matches("a" * 10), True)

    def test3(self):
        self.assertEqual(self.r.matches("a" * 10 + "b"), False)


class MultiContentRegexp(unittest.TestCase):
    """b|c(a|b)*"""

    r = alt(char("b"), concat(char("c"), star(alt(char("a"), char("b")))))

    def test1(self):
        self.assertEqual(self.r.matches("b"), True)

    def test2(self):
        self.assertEqual(self.r.matches("c"), True)

    def test3(self):
        self.assertEqual(self.r.matches("cabba"), True)

    def test4(self):
        self.assertEqual(self.r.matches("d"), False)

    def test5(self):
        self.assertEqual(self.r.matches("ba"), False)

    def test6(self):
        self.assertEqual(self.r.matches("aaaa"), False)


class DivisionRegexp(unittest.TestCase):
    """(0|1(01*0)*1)*"""

    one = char("1")
    zero = char("0")
    r = star(alt(zero, concat(one, concat(star(concat(zero, concat(star(one), zero))), one))))

    def test0(self):
        self.assertEqual(self.r.matches("0"), True)

    def test1(self):
        self.assertEqual(self.r.matches("1"), False)

    def test2(self):
        self.assertEqual(self.r.matches("10"), False)

    def test3(self):
        self.assertEqual(self.r.matches("11"), True)

    def test4(self):
        self.assertEqual(self.r.matches("100"), False)

    def test5(self):
        self.assertEqual(self.r.matches("101"), False)

    def test6(self):
        self.assertEqual(self.r.matches("110"), True)

    def test7(self):
        self.assertEqual(self.r.matches("111"), False)

    def test8(self):
        self.assertEqual(self.r.matches("1000"), False)

    def test9(self):
        self.assertEqual(self.r.matches("1001"), True)

    def test10(self):
        self.assertEqual(self.r.matches("1010"), False)

    def test11(self):
        self.assertEqual(self.r.matches("1011"), False)

    def test12(self):
        self.assertEqual(self.r.matches("1100"), True)

    def test13(self):
        self.assertEqual(self.r.matches("1101"), False)

    def test14(self):
        self.assertEqual(self.r.matches("1110"), False)

    def test15(self):
        self.assertEqual(self.r.matches("1111"), True)

    def test16(self):
        self.assertEqual(self.r.matches("10000"), False)

    def test17(self):
        self.assertEqual(self.r.matches("10001"), False)

    def test18(self):
        self.assertEqual(self.r.matches("10010"), True)

    def test19(self):
        self.assertEqual(self.r.matches("10011"), False)

    def test20(self):
        self.assertEqual(self.r.matches("10100"), False)

    def test21(self):
        self.assertEqual(self.r.matches("10101"), True)

    def test22(self):
        self.assertEqual(self.r.matches("10110"), False)

    def test23(self):
        self.assertEqual(self.r.matches("10111"), False)

    def test24(self):
        self.assertEqual(self.r.matches("11000"), True)

    def test25(self):
        self.assertEqual(self.r.matches("11001"), False)

    def test26(self):
        self.assertEqual(self.r.matches("11010"), False)

    def test27(self):
        self.assertEqual(self.r.matches("11011"), True)

    def test28(self):
        self.assertEqual(self.r.matches("11100"), False)

    def test29(self):
        self.assertEqual(self.r.matches("11101"), False)

    def test30(self):
        self.assertEqual(self.r.matches("11110"), True)

    def test31(self):
        self.assertEqual(self.r.matches("11111"), False)

    def test32(self):
        self.assertEqual(self.r.matches("100000"), False)

    def test33(self):
        self.assertEqual(self.r.matches("100001"), True)

    def test34(self):
        self.assertEqual(self.r.matches("100010"), False)

    def test35(self):
        self.assertEqual(self.r.matches("100011"), False)

    def test36(self):
        self.assertEqual(self.r.matches("100100"), True)

    def test37(self):
        self.assertEqual(self.r.matches("100101"), False)

    def test38(self):
        self.assertEqual(self.r.matches("100110"), False)

    def test39(self):
        self.assertEqual(self.r.matches("100111"), True)

    def test40(self):
        self.assertEqual(self.r.matches("101000"), False)

    def test41(self):
        self.assertEqual(self.r.matches("101001"), False)

    def test42(self):
        self.assertEqual(self.r.matches("101010"), True)

    def test43(self):
        self.assertEqual(self.r.matches("101011"), False)

    def test44(self):
        self.assertEqual(self.r.matches("101100"), False)

    def test45(self):
        self.assertEqual(self.r.matches("101101"), True)

    def test46(self):
        self.assertEqual(self.r.matches("101110"), False)

    def test47(self):
        self.assertEqual(self.r.matches("101111"), False)

    def test48(self):
        self.assertEqual(self.r.matches("110000"), True)

    def test49(self):
        self.assertEqual(self.r.matches("110001"), False)

    def test50(self):
        self.assertEqual(self.r.matches("110010"), False)

    def test51(self):
        self.assertEqual(self.r.matches("110011"), True)

    def test52(self):
        self.assertEqual(self.r.matches("110100"), False)

    def test53(self):
        self.assertEqual(self.r.matches("110101"), False)

    def test54(self):
        self.assertEqual(self.r.matches("110110"), True)

    def test55(self):
        self.assertEqual(self.r.matches("110111"), False)

    def test56(self):
        self.assertEqual(self.r.matches("111000"), False)

    def test57(self):
        self.assertEqual(self.r.matches("111001"), True)

    def test58(self):
        self.assertEqual(self.r.matches("111010"), False)

    def test59(self):
        self.assertEqual(self.r.matches("111011"), False)

    def test60(self):
        self.assertEqual(self.r.matches("111100"), True)

    def test61(self):
        self.assertEqual(self.r.matches("111101"), False)

    def test62(self):
        self.assertEqual(self.r.matches("111110"), False)

    def test63(self):
        self.assertEqual(self.r.matches("111111"), True)

    def test64(self):
        self.assertEqual(self.r.matches("1000000"), False)

    def test65(self):
        self.assertEqual(self.r.matches("1000001"), False)

    def test66(self):
        self.assertEqual(self.r.matches("1000010"), True)

    def test67(self):
        self.assertEqual(self.r.matches("1000011"), False)

    def test68(self):
        self.assertEqual(self.r.matches("1000100"), False)

    def test69(self):
        self.assertEqual(self.r.matches("1000101"), True)

    def test70(self):
        self.assertEqual(self.r.matches("1000110"), False)

    def test71(self):
        self.assertEqual(self.r.matches("1000111"), False)

    def test72(self):
        self.assertEqual(self.r.matches("1001000"), True)

    def test73(self):
        self.assertEqual(self.r.matches("1001001"), False)

    def test74(self):
        self.assertEqual(self.r.matches("1001010"), False)

    def test75(self):
        self.assertEqual(self.r.matches("1001011"), True)

    def test76(self):
        self.assertEqual(self.r.matches("1001100"), False)

    def test77(self):
        self.assertEqual(self.r.matches("1001101"), False)

    def test78(self):
        self.assertEqual(self.r.matches("1001110"), True)

    def test79(self):
        self.assertEqual(self.r.matches("1001111"), False)

    def test80(self):
        self.assertEqual(self.r.matches("1010000"), False)

    def test81(self):
        self.assertEqual(self.r.matches("1010001"), True)

    def test82(self):
        self.assertEqual(self.r.matches("1010010"), False)

    def test83(self):
        self.assertEqual(self.r.matches("1010011"), False)

    def test84(self):
        self.assertEqual(self.r.matches("1010100"), True)

    def test85(self):
        self.assertEqual(self.r.matches("1010101"), False)

    def test86(self):
        self.assertEqual(self.r.matches("1010110"), False)

    def test87(self):
        self.assertEqual(self.r.matches("1010111"), True)

    def test88(self):
        self.assertEqual(self.r.matches("1011000"), False)

    def test89(self):
        self.assertEqual(self.r.matches("1011001"), False)

    def test90(self):
        self.assertEqual(self.r.matches("1011010"), True)

    def test91(self):
        self.assertEqual(self.r.matches("1011011"), False)

    def test92(self):
        self.assertEqual(self.r.matches("1011100"), False)

    def test93(self):
        self.assertEqual(self.r.matches("1011101"), True)

    def test94(self):
        self.assertEqual(self.r.matches("1011110"), False)

    def test95(self):
        self.assertEqual(self.r.matches("1011111"), False)

    def test96(self):
        self.assertEqual(self.r.matches("1100000"), True)

    def test97(self):
        self.assertEqual(self.r.matches("1100001"), False)

    def test98(self):
        self.assertEqual(self.r.matches("1100010"), False)

    def test99(self):
        self.assertEqual(self.r.matches("1100011"), True)


class SomeRegexp(unittest.TestCase):
    """(ba)*|(ca)*"""

    r = alt(star(concat(char("b"), char("a"))), star(concat(char("c"), char("a"))))

    def test1(self):
        self.assertEqual(self.r.matches("ba"), True)

    def test2(self):
        self.assertEqual(self.r.matches("ca"), True)

    def test3(self):
        self.assertEqual(self.r.matches("ba" * 4), True)

    def test4(self):
        self.assertEqual(self.r.matches("ca" * 4), True)

    def test5(self):
        self.assertEqual(self.r.matches("ba" * 4 + "b"), False)

    def test6(self):
        self.assertEqual(self.r.matches("ba" * 4 + "ca"), False)

    def test7(self):
        self.assertEqual(self.r.matches("bac"), False)

    def test8(self):
        self.assertEqual(self.r.matches("caba"), False)


if __name__ == "__main__":
    unittest.main()
