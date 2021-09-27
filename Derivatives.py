from typing import Union
import time


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
    return nullable(derivative_(s, r)) is Epsilon


def derivative_(s: str, r):
    derivative_(s[1:], derivative(Char(s[0]), r)) if s else r

# derivative :: Char -> Regexp -> Regexp

def derivative(a: Char, r: Union[Empty, Epsilon]):
    return Empty()


def derivative(a: Char, r: Char):
    if a.value == r.value:
        return Epsilon()
    else:
        return Empty()


def derivative(a: Char, r: Alt):
    return Alt(derivative(a, r.a), derivative(a, r.b))


def derivative(a: Char, r: Star):
    return Concat(derivative(a, r.value), r)


def derivative(a: Char, r: Concat):
    return Alt(Concat(derivative(a, r.a), r.b), Concat(nullable(r.a), derivative(a, r.b)))


# intersect :: Regexp -> Regexp -> Regexp
def intersect(em: Empty, other):
    return Empty()


def intersect(other, em: Empty):
    return Empty()


def intersect(a: Epsilon, b: Epsilon):
    return Epsilon()


# union :: Regexp -> Regexp -> Regexp
def union(eps: Epsilon, other):
    return Epsilon()


def union(other, eps: Epsilon):
    return Epsilon()


def union(a: Empty, b: Empty):
    return Empty()


# nullable :: Regexp -> Regexp
def nullable(r: Union[Empty, Char]):
    return Empty()


def nullable(eps: Epsilon):
    return Epsilon()


def nullable(c: Concat):
    return intersect(nullable(c.a), nullable(c.b))


def nullable(a: Alt):
    return union(nullable(a.a), nullable(a.b))


def nullable(s: Star):
    return Epsilon()
