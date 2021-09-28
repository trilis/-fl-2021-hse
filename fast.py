import time


class Empty:
    pass


class Epsilon:
    pass


class Char:
    def __init__(self, c):
        self.b = c


class Star:
    def __init__(self, r):
        self.s = r


class Alt:
    def __init__(self, a, b):
        self.s = a
        self.t = b


class Concat:
    def __init__(self, a, b):
        self.s = a
        self.t = b


def match(r, s):
    return nullable(derivative_(s, r))


def derivative(a, b):
    s_type = type(b)
    if s_type == Empty or s_type == Epsilon or (s_type == Char and a.b != b.b):
        return Empty()
    elif s_type == Char:
        return Epsilon()
    elif s_type == Alt:
        if type(b.s) == Empty or (type(b.s) == Epsilon and nullable(b.t)):
            return derivative(a, b.t)
        elif type(b.t) == Empty or (type(b.t) == Epsilon and nullable(b.s)):
            return derivative(a, b.s)
        else:
            return Alt(derivative(a, b.s), derivative(a, b.t))
    elif s_type == Star:
        if type(b.s) == Star:
            return derivative(a, b.s)
        elif type(b.s) == Empty:
            return Empty()
        elif type(b.s) == Epsilon:
            return Epsilon()
        else:
            return Concat(derivative(a, b.s), b)
    else:
        if type(b.s) == Empty or type(b.t) == Empty:
            return Empty()
        elif type(b.s) == Epsilon:
            return derivative(a, b.t)
        elif type(b.t) == Epsilon or b.s == b.t:
            return derivative(a, b.s)
        elif nullable(b.s):
            return Alt(Concat(derivative(a, b.s), b.t), derivative(a, b.t))
        else:
            return Concat(derivative(a, b.s), b.t)


def nullable(a):
    s_type = type(a)
    if s_type == Empty or s_type == Char:
        return False
    elif s_type == Epsilon or s_type == Star:
        return True
    elif s_type == Concat:
        return nullable(a.s) and nullable(a.t)
    else:
        return nullable(a.s) or nullable(a.t)


def derivative_(s, r):
    if len(s) > 0:
        return derivative_(s[1:], derivative(Char(s[0]), r))
    return r


if __name__ == "__main__":
    reg_list = [[Concat(Char('0'), Concat(Star(Alt(Char('0'), Char('1'))), Char('0'))), "0(0|1)*0"],
                [Alt(Star(
                    Alt(Concat(Concat(Char('2'), Char('2')), Char('8')),
                        Concat(Concat(Char('1'), Char('1')), Char('1')))),
                    Alt(Alt(Concat(Char('1'), Char('3')), Concat(Char('3'), Star(Char('7')))),
                        Concat(Concat(Char('1'), Char('0')), Char('1')))), "(228|111)*|((13|37*)|101)"],
                [Alt(Alt(Star(Alt(Concat(Char('1'), Char('0')), Char('1'))),
                         Star(Alt(Concat(Char('1'), Char('1')), Char('0')))),
                     Star(Concat(Concat(Char('1'), Char('0')), Char('0')))), "(((10|1)*|(11|0)*)|(100)*)"],
                [Alt(Star(Alt(Alt(Concat(Star(Char('1')), Char('1')), Concat(Char('0'), Char('0'))),
                              Star(Alt(Concat(Concat(Char('1'), Char('1')), Char('1')),
                                       Concat(Concat(Char('0'), Char('0')), Char('0')))))),
                     Alt(Concat(Char('1'), Char('1')), Concat(Char('1'), Star(Char('1'))))),
                 "secret information"]]
    input_list = ["100", "0000000", "10110", "010100", "111000111000111000111000111000"]
    for i in reg_list:
        print(i[1])
        for j in input_list:
            t1 = time.perf_counter()
            res = match(i[0], j)
            t2 = time.perf_counter()
            print(f"Time: {t2 - t1:0.6f}", ' ', j, ' ', res, ' ')
        print('\n')
