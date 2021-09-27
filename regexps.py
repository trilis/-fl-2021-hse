from __future__ import annotations


class Regexp:
    def matches(self, s: str) -> bool:
        return isinstance(self.string_derivative(s).nullable(), Epsilon)

    def derivative(self, c: str) -> Regexp:
        pass

    def string_derivative(self, s: str) -> Regexp:
        r = self
        for c in s:
            r = r.derivative(c)
        return r

    def nullable(self) -> Regexp:
        pass


def intersect(a: Regexp, b: Regexp) -> Regexp:
    if isinstance(a, Empty) or isinstance(b, Empty):
        return Empty()
    elif isinstance(a, Epsilon) and isinstance(b, Epsilon):
        return Epsilon()
    else:
        raise NotImplementedError


def union(a: Regexp, b: Regexp) -> Regexp:
    if isinstance(a, Epsilon) or isinstance(b, Epsilon):
        return Epsilon()
    elif isinstance(a, Empty) and isinstance(b, Empty):
        return Empty()
    else:
        raise NotImplementedError


class Empty(Regexp):
    def derivative(self, c: str) -> Regexp:
        return Empty()

    def nullable(self) -> Regexp:
        return Empty()


class Epsilon(Regexp):
    def derivative(self, c: str) -> Regexp:
        return Empty()

    def nullable(self) -> Regexp:
        return Epsilon()


class Char(Regexp):
    def __init__(self, c: str):
        self.c = c

    def derivative(self, c: str) -> Regexp:
        if c == self.c:
            return Epsilon()
        else:
            return Empty()

    def nullable(self) -> Regexp:
        return Empty()


class Star(Regexp):
    def __init__(self, r: Regexp):
        self.regexp = r

    def derivative(self, c: str) -> Regexp:
        return Concat(self.regexp.derivative(c), self)

    def nullable(self) -> Regexp:
        return Epsilon()


class Alt(Regexp):
    def __init__(self, l: Regexp, r: Regexp):
        self.left_regexp = l
        self.right_regexp = r

    def derivative(self, c: str) -> Regexp:
        return Alt(self.left_regexp.derivative(c), self.right_regexp.derivative(c))

    def nullable(self) -> Regexp:
        return union(self.left_regexp.nullable(), self.right_regexp.nullable())


class Concat(Regexp):
    def __init__(self, l: Regexp, r: Regexp):
        self.left_regexp = l
        self.right_regexp = r

    def derivative(self, c: str) -> Regexp:
        return Alt(
            Concat(self.left_regexp.derivative(c), self.right_regexp),
            Concat(self.left_regexp.nullable(), self.right_regexp.derivative(c)),
        )

    def nullable(self) -> Regexp:
        return intersect(self.left_regexp.nullable(), self.right_regexp.nullable())
