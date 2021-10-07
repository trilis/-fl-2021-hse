import time


class Empty:
    pass


class Epsilon:
    pass


class Char:
    def __init__(self, arg):
        self.arg = arg


class Star:
    def __init__(self, arg):
        self.arg = arg


class Alt:
    def __init__(self, arg1, arg2):
        self.arg1 = arg1
        self.arg2 = arg2


class Concat:
    def __init__(self, arg1, arg2):
        self.arg1 = arg1
        self.arg2 = arg2


def intersect(r1, r2):
    if type(r1) == Empty:
        return Empty()
    if type(r2) == Empty:
        return Empty()
    if type(r1) == Epsilon and type(r2) == Epsilon:
        return Epsilon()


def union(r1, r2):
    if type(r1) == Epsilon:
        return Epsilon()
    if type(r2) == Epsilon:
        return Epsilon()
    if type(r1) == Empty and type(r2) == Empty:
        return Empty()


def nullable(r):
    if type(r) == Empty:
        return Empty()
    if type(r) == Epsilon:
        return Epsilon()
    if type(r) == Char:
        return Empty()
    if type(r) == Concat:
        return intersect(nullable(r.arg1), nullable(r.arg2))
    if type(r) == Alt:
        return union(nullable(r.arg1), nullable(r.arg2))
    if type(r) == Star:
        return Epsilon()


def derivative_str(s, r):
    if s == '':
        return r
    else:
        return derivative_str(s[1:], derivative(s[0], r))


def derivative(a: Char, r):
    if type(r) == Empty:
        return Empty()
    if type(r) == Epsilon:
        return Empty()
    if type(r) == Char and a == r.arg:
        return Epsilon()
    if type(r) == Char:
        return Empty()
    if type(r) == Alt:
        return Alt(derivative(a, r.arg1), derivative(a, r.arg2))
    if type(r) == Star:
        return Concat(derivative(a, r.arg), Star(r.arg))
    if type(r) == Concat:
        return Alt(Concat(derivative(a, r.arg1), r.arg2), Concat(nullable(r.arg1), derivative(a, r.arg2)))


def match(r, s):
    return type(nullable(derivative_str(s, r))) == Epsilon


def test(exp, count):
    for i in range(count):
        x = input()
        start_time = time.time()
        res = match(exp, x)
        duration = time.time() - start_time
        print(i + 1, ') ', format(duration, '.6f'), 's|', x, ': ', res, sep='')


test_exp1 = Concat(Star(Concat(Char('a'), Char('b'))), Char('a'))
print('\ntest1: (ab)*a\n')
test(test_exp1, 10)

test_exp2 = Star(Concat(Alt(Char('a'), Char('b')), Alt(Char('a'), Char('b'))))
print('\ntest2: ((a|b)(a|b))*\n')
test(test_exp2, 5)

# Деление на 3 (без учета пустой строки)
pup = Star(Concat(Char('0'), Concat(Star(Char('1')), Char('0'))))
test_exp3 = Star(Alt(Char('0'), Concat(Char('1'), Concat(pup, Char('1')))))
print('\ntest3. Dividing by 3\n')
test(test_exp3, 5)

# Матчит слово for или if
test_exp4 = Alt(Concat(Concat(Char('f'), Char('o')), Char('r')), Concat(Char('i'), Char('f')))
print('\ntest4. Keywords: for, if\n')
test(test_exp4, 5)

print('\ntest5. (a | b | c | d)*\n')
test_exp5 = Star(Alt(Alt(Char('a'), Char('b')), Alt(Char('c'), Char('d'))))
test(test_exp5, 5)
