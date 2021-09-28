import time

class Regexp:
    pass

class Empty(Regexp):
    pass

class Epsilon(Regexp):
    pass

class Char(Regexp):
    def __init__(self, a: str):
        self.a = a
        
class Star(Regexp):
    def __init__(self, s: Regexp):
        self.s = s
        
class Alt(Regexp):
    def __init__(self, s: Regexp, t: Regexp):
        self.s = s
        self.t = t
        
class Concat(Regexp):
    def __init__(self, s: Regexp, t: Regexp):
        self.s = s
        self.t = t
        
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
    return derivative_(word[1:], derivative(word[0], regex)) if word else regex

def match(word: str, regex: str) -> bool:
    return nullable(derivative_(word, regex))


if __name__ == '__main__':
    r1 = Concat((Star(Concat(Char('a'), Char('b')))), Char('a'))

    for i in range(0, 200, 50):
        start = time.time()
        word = "ab"*i + "a"
        print(f'len = {len(word)}, result = {match(word, r1)}, time elapsed = {time.time() - start:.3f}s')
