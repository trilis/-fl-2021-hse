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
    return isinstance(nullable(derivative_(s, r)), Epsilon)


def derivative(a, b):
    s_type = type(b)
    if s_type == Empty or s_type == Epsilon or (s_type == Char and a.b != b.b):
        return Empty()
    elif s_type == Char:
        return Epsilon()
    elif s_type == Alt:
        return Alt(derivative(a, b.s), derivative(a, b.t))
    elif s_type == Star:
        return Concat(derivative(a, b.s), b)
    else:
        return Alt(Concat(derivative(a, b.s), b.t), Concat(nullable(b.s), derivative(a, b.t)))


def intersect(a, b):
    if type(a) == Empty or type(b) == Empty:
        return Empty()
    elif type(a) == Epsilon and type(b) == Epsilon:
        return Epsilon()


def union(a, b):
    if type(a) == Epsilon or type(b) == Epsilon:
        return Epsilon()
    elif type(a) == Empty and type(b) == Empty:
        return Empty()


def nullable(a):
    s_type = type(a)
    if s_type == Empty or s_type == Char:
        return Empty()
    elif s_type == Epsilon or s_type == Star:
        return Epsilon()
    elif s_type == Concat:
        return intersect(nullable(a.s), nullable(a.t))
    else:
        return union(nullable(a.s), nullable(a.t))


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
                [Alt(Alt(Star(Concat(Concat(Char('1'), Char('0')), Char('1'))),
                         Star(Concat(Concat(Char('1'), Char('1')), Char('0')))),
                     Star(Concat(Concat(Char('1'), Char('0')), Char('0')))), "(((101)*|(110)*)|(100)*)"]]
    input_list = ["100", "0000000", "10110", "010100"]
    for i in reg_list:
        print(i[1])
        for j in input_list:
            t1 = time.perf_counter()
            res = match(i[0], j)
            t2 = time.perf_counter()
            print(f"Time: {t2 - t1:0.6f}", ' ', j, ' ', res, ' ')
        print('\n')
