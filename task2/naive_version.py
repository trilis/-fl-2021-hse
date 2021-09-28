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

def nullable(regex: Regexp) -> Regexp:
    if isinstance(regex, Empty):
        return Empty()
    elif isinstance(regex, Epsilon):
        return Epsilon()
    elif isinstance(regex, Char):
        return Empty()
    elif isinstance(regex, Concat):
        return intersect(nullable(regex.s), nullable(regex.t))
    elif isinstance(regex, Alt):
        return union(nullable(regex.s), nullable(regex.t))
    elif isinstance(regex, Star):
        return Epsilon()
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
        return Alt(Concat(derivative(a, regex.s), regex.t), Concat(nullable(regex.s), derivative(a, regex.t)))
    else:
        raise ValueError()

def derivative_(word: str, regex: Regexp) -> Regexp:
    return derivative_(word[1:], derivative(word[0], regex)) if word else regex

def match(word: str, regex: str) -> bool:
    return isinstance(nullable(derivative_(word, regex)), Epsilon)

if __name__ == '__main__':
    r1 = Concat((Star(Concat(Char('a'), Char('b')))), Char('a'))
    r2 = Star(Alt(Char('a'), Char('b')))
    r3 = Concat(Star(Alt(Char('a'), Char('b'))), Char('b'))
    r4 = Epsilon()
    r5 = Alt(Epsilon(), Star(Char('a')))
    
    assert match('aba', r1)
    assert not match('baba', r1)
    
    assert match('aabaa', r2)
    assert not match('caaba', r2)
    
    assert match('aaaaab', r3)
    assert not match('babbba', r3)
    
    assert match('', r4)
    assert not match('abacaf', r4)
    
    assert match('', r5)
    assert match('aaaa', r5)
    assert not match('aabaa', r5)

    for i in range(5):
        start = time.time()
        word = "ab"*i + "a"
        print(f'len = {len(word)}, result = {match(word, r1)}, time elapsed = {time.time() - start:.3f}s')
