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
    return nullable(derivative_(s, r))


# derivative :: Char -> Regexp -> Regexp
def derivative(a: Char, r):
    r_type = type(r)
    #print(r_type)
    if r_type is Empty or r_type is Epsilon:
        return Empty()
    elif r_type is Char:
        return Epsilon() if a.value == r.value else Empty()
    elif r_type is Alt:
        if type(r.a) is Empty:
            return derivative(a, r.b)
        elif type(r.b) is Empty:
            return derivative(a, r.a)
        elif type(r.a) is Epsilon and nullable(r.b):
            return derivative(a, r.b)
        elif type(r.b) is Epsilon and nullable(r.a):
            return derivative(a, r.a)
        else:
            return Alt(derivative(a, r.a), derivative(a, r.b))
    elif r_type is Star:
        if type(r.value) is Star:
            return derivative(a, r.value)
        elif type(r.value) is Empty:
            return Empty()
        elif type(r.value) is Epsilon:
            return Epsilon()
        return Concat(derivative(a, r.value), r)
    elif r_type is Concat:
        if type(r.a) is Empty or type(r.b) is Empty:
            return Empty()
        elif type(r.a) is Epsilon:
             return derivative(a, r.b)
        elif type(r.b) is Epsilon:
             return derivative(a, r.a)
        elif r.a == r.b:
            return derivative(a, r.a)
        elif nullable(r.a):
            return Alt(Concat(derivative(a, r.a), r.b), derivative(a, r.b))
        else:
            return Concat(derivative(a, r.a), r.b)

def derivative_(s: str, r):
    if s:
        return derivative_(s[1:], derivative(Char(s[0]), r))
    return r


# nullable :: Regexp -> bool
def nullable(r):
    r_type = type(r)
    if r_type is Empty or r_type is Char:
        return False
    elif r_type is Epsilon:
        return True
    elif r_type is Concat:
        return nullable(r.a) and nullable(r.b)
    elif r_type is Alt:
        return nullable(r.a) or nullable(r.b)
    elif r_type is Star:
        return True
