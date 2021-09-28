import time

class Regexp:
    pass

class Empty(Regexp):
    def __eq__(self, other):
        return isinstance(other, Empty)

class Epsilon(Regexp):
    def __eq__(self, other):
        return isinstance(other, Epsilon)

class Char(Regexp):
    def __init__(self, a: str):
        self.a = a
        
    def __eq__(self, other):
        return isinstance(other, Char) and other.a == self.a
        
class Star(Regexp):
    def __init__(self, s: Regexp):
        self.s = s
        
    def __eq__(self, other):
        return isinstance(other, Star) and other.s == self.s
        
class Alt(Regexp):
    def __init__(self, s: Regexp, t: Regexp):
        self.s = s
        self.t = t
        
    def __eq__(self, other):
        return isinstance(other, Alt) and other.s == self.s and other.t == self.t
        
class Concat(Regexp):
    def __init__(self, s: Regexp, t: Regexp):
        self.s = s
        self.t = t
        
    def __eq__(self, other):
        return isinstance(other, Concat) and other.s == self.s and other.t == self.t
        
def short_regexp(regex: Regexp) -> Regexp:
    if isinstance(regex, Star):
        if isinstance(regex.s, Star):
            return short_regexp(regex.s)
        elif isinstance(regex.s, Empty):
            return Empty()
        elif isinstance(regex.s, Epsilon):
            return Epsilon()
        regex.s = short_regexp(regex.s)
    elif isinstance(regex, Alt):
        if isinstance(regex.s, Empty):
            return short_regexp(regex.t)
        elif isinstance(regex.t, Empty):
            return short_regexp(regex.s)
        elif isinstance(regex.s, Epsilon) and nullable(regex.t):
            return short_regexp(regex.t)
        elif isinstance(regex.t, Epsilon) and nullable(regex.s):
            return short_regexp(regex.s)
        elif regex.s == regex.t:
            return short_regexp(regex.s)
    elif isinstance(regex, Concat):
        if isinstance(regex.s, Empty):
            return Empty()
        elif isinstance(regex.t, Empty):
            return Empty()
        elif isinstance(regex.s, Epsilon):
            return short_regexp(regex.t)
        elif isinstance(regex.t, Epsilon):
            return short_regexp(regex.s)
    return regex
        
def intersect(a: Regexp, b: Regexp) -> Regexp:
    if isinstance(a, Empty):
        return Empty()
    elif isinstance(b, Empty):
        return Empty()
    elif isinstance(a, Epsilon) and isinstance(b, Epsilon):
        return Epsilon()
    else:
        raise ValueError()

def union(a: Regexp, b: Regexp) -> Regexp:
    if isinstance(a, Epsilon):
        return Epsilon()
    elif isinstance(b, Epsilon):
        return Epsilon()
    elif isinstance(a, Empty) and isinstance(b, Empty):
        return Empty()
    else:
        raise ValueError()

def nullable(regex: Regexp) -> bool:
    if isinstance(regex, Empty):
        return False
    elif isinstance(regex, Epsilon):
        return True
    elif isinstance(regex, Char):
        return False
    elif isinstance(regex, Concat):
        return nullable(regex.s) and nullable(regex.t)
    elif isinstance(regex, Alt):
        return nullable(regex.s) or nullable(regex.t)
    elif isinstance(regex, Star):
        return True
    else:
        raise ValueError()

def derivative(a: str, regex: Regexp) -> Regexp:
    if isinstance(regex, Empty):
        return Empty()
    elif isinstance(regex, Epsilon):
        return Empty()
    elif isinstance(regex, Char):
        return Epsilon() if regex.a == a else Empty()
    elif isinstance(regex, Alt):
        return Alt(derivative(a, regex.s), derivative(a, regex.t))
    elif isinstance(regex, Star):
        return Concat(derivative(a, regex.s), Star(regex.s))
    elif isinstance(regex, Concat):
        return Alt(Concat(derivative(a, regex.s), regex.t), derivative(a, regex.t)) if nullable(regex.s) else Concat(derivative(a, regex.s), regex.t)
    else:
        raise ValueError()

def derivative_(word: str, regex: Regexp) -> Regexp:
    return short_regexp(derivative_(word[1:], short_regexp(derivative(word[0], regex)))) if word else regex

def match(word: str, regex: str) -> bool:
    return nullable(derivative_(word, regex))


if __name__ == '__main__':
    r1 = Concat((Star(Concat(Char('a'), Char('b')))), Char('a'))

    for i in range(0, 500, 50):
        start = time.time()
        word = "ab"*i + "a"
        print(f'len = {len(word)}, result = {match(word, r1)}, time elapsed = {time.time() - start:.3f}s')
