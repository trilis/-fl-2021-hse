class Empty:
    pass


class Epsilon:
    pass


class Char:
    def __init__(self, c):
        self.value = c


class Star:
    def __init__(self, r):
        self.value = r


class Alt:
    def __init__(self, a, b):
        self.a = a
        self.b = b


class Concat:
    def __init__(self, a, b):
        self.a = a
        self.b = b


def match(r, s: str) -> bool:
    return type(nullable(derivative_(s, r))) is Epsilon


# derivative :: Char -> Regexp -> Regexp
def derivative(a: Char, r):
    r_type = type(r)
    if r_type is Empty or r_type is Epsilon:
        return Empty()
    elif r_type is Char:
        return Epsilon() if a.value == r.value else Empty()
    elif r_type is Alt:
        return Alt(derivative(a, r.a), derivative(a, r.b))
    elif r_type is Star:
        return Concat(derivative(a, r.value), r)
    elif r_type is Concat:
        return Alt(Concat(derivative(a, r.a), r.b), Concat(nullable(r.a), derivative(a, r.b)))


def derivative_(s: str, r):
    if s:
        return derivative_(s[1:], derivative(Char(s[0]), r))
    return r


# intersect :: Regexp -> Regexp -> Regexp
def intersect(a, b):
    a_type = type(a)
    b_type = type(b)
    if a_type is Empty or b_type is Empty:
        return Empty()
    elif a_type is Epsilon and b_type is Epsilon:
        return Epsilon()


# union :: Regexp -> Regexp -> Regexp
def union(a, b):
    a_type = type(a)
    b_type = type(b)
    if a_type is Epsilon or b_type is Epsilon:
        return Epsilon()
    elif a_type is Empty and b_type is Empty:
        return Empty()


# nullable :: Regexp -> Regexp
def nullable(r):
    r_type = type(r)
    if r_type is Empty or r_type is Char:
        return Empty()
    elif r_type is Epsilon:
        return Epsilon()
    elif r_type is Concat:
        return intersect(nullable(r.a), nullable(r.b))
    elif r_type is Alt:
        return union(nullable(r.a), nullable(r.b))
    elif r_type is Star:
        return Epsilon()
