import time

class tre(): #type of regexp
    empty = 0
    epsilon = 1
    symb = 2
    star = 3
    alt = 4
    concat = 5

class Regexp:
    def __init__(self, type, *args):
        self.type = type
        if (type == tre.symb):
            self.s = args[0]
        elif (type == tre.star):
            if args[0].type == tre.empty: self.type = tre.empty
            elif args[0].type == tre.epsilon: self.type = tre.epsilon
            elif args[0].type == tre.star: self = args[0]
            else: self.r = args[0]
        elif (type == tre.alt):
            i = -1
            if args[0].type == tre.empty: i = 1
            elif args[1].type == tre.empty: i = 0
            elif args[0].type == tre.epsilon and nullable(args[1]): i = 1
            elif args[1].type == tre.epsilon and nullable(args[0]): i = 0
            elif equally(args[0], args[1]): i = 0

            if i != -1:
                self.type = args[i].type
                if args[i].type == tre.symb: self.s = args[i].s
                elif args[i].type == tre.star: self.r = args[i].r
                elif args[i].type == tre.concat or args[i].type == tre.alt:
                    self.r1, self.r2 = args[i].r1, args[i].r2
            else:
                self.r1, self.r2 = args[0], args[1]
        elif (type == tre.concat):
            if args[0].type == tre.empty or args[1].type == tre.empty:
                self.type = tre.empty
            elif args[0].type == tre.epsilon or args[1].type == tre.epsilon:
                i = 0 if args[1].type == tre.epsilon else 1
                self.type = args[i].type
                if args[i].type == tre.symb: self.s = args[i].s
                elif args[i].type == tre.star: self.r = args[i].r
                elif args[i].type == tre.concat or args[i].type == tre.alt:
                    self.r1, self.r2 = args[i].r1, args[i].r2
            else:
                self.r1, self.r2 = args[0], args[1]


def equally(r, q):
    if r.type != q.type: return False
    elif r.type == tre.empty or r.type == tre.epsilon: return True
    elif r.type == tre.symb: return True if r.s == q.s else False
    elif r.type == tre.star: return equally(r.r, q.r)
    elif r.type == tre.concat: return equally(r.r1, q.r1) & equally(r.r2, q.r2)
    else: return ((equally(r.r1, q.r1) & equally(r.r2, q.r2))
                | (equally(r.r1, q.r2) & equally(r.r2, q.r1)))

def nullable(r):
    if r.type == tre.empty or r.type == tre.symb:
        return False
    elif r.type == tre.epsilon or r.type == tre.star:
        return True
    elif r.type == tre.concat:
        return nullable(r.r1) & nullable(r.r2)
    elif r.type == tre.alt:
        return nullable(r.r1) | nullable(r.r2)

def der(s, r):
    if (r.type == tre.empty
            or r.type == tre.epsilon
            or (r.type == tre.symb and r.s != s)):
        return Regexp(tre.empty)
    elif r.type == tre.symb:
        return Regexp(tre.epsilon)
    elif r.type == tre.alt:
        return Regexp(tre.alt, der(s, r.r1), der(s, r.r2))
    elif r.type == tre.star:
        return Regexp(tre.concat, der(s, r.r), r)
    elif r.type == tre.concat and nullable(r.r1):
        return Regexp(tre.alt,
            Regexp(tre.concat, der(s, r.r1), r.r2),
            der(s, r.r2))
    else:
        return Regexp(tre.concat, der(s, r.r1), r.r2)

def derStr(str, r):
    if (str == ''): return r
    else:
        return derStr(str[1:], der(str[0], r))

def match(r, str):
    return nullable(derStr(str, r))

#Regular Expressions
"""r = Regexp(tre.concat, Regexp(tre.symb, '1'),
            Regexp(tre.concat,
                Regexp(tre.star, Regexp(tre.alt, Regexp(tre.symb, '0'), Regexp(tre.symb, '1'))),
                Regexp(tre.symb, '0')))"""
    #1(0|1)*0
"""r = Regexp(tre.concat,
    Regexp(tre.star,
        Regexp(tre.concat,
            Regexp(tre.symb, 'a'), Regexp(tre.symb, 'b'))),
    Regexp(tre.symb, 'a'))"""
    #(ab)*a
"""r = Regexp(tre.concat, Regexp(tre.star, Regexp(tre.symb, '1')),
    Regexp(tre.concat, Regexp(tre.star, Regexp(tre.symb, '0')), Regexp(tre.star, Regexp(tre.symb, '1'))))"""
    #1*0*1*
"""r = Regexp(tre.star, Regexp(tre.alt,
                    Regexp(tre.concat, Regexp(tre.symb, 'b'), Regexp(tre.symb, 'b')),
                    Regexp(tre.concat, Regexp(tre.symb, 'a'), Regexp(tre.symb, 'a'))))"""
    #(bb|aa)*
r = Regexp(tre.concat, Regexp(tre.star, Regexp(tre.alt, Regexp(tre.symb, 'a'), Regexp(tre.symb, 'b'))),
        Regexp(tre.concat, Regexp(tre.symb, 'c'),
        Regexp(tre.star, Regexp(tre.alt, Regexp(tre.symb, 'a'), Regexp(tre.symb, 'b')))))
    #(a|b)*c(a|b)*
"""r = Regexp(tre.concat, Regexp(tre.star, Regexp(tre.symb, 'a')),
    Regexp(tre.concat, Regexp(tre.star, Regexp(tre.symb, 'b')),
        Regexp(tre.concat, Regexp(tre.star, Regexp(tre.alt, Regexp(tre.star, Regexp(tre.symb, 'c')), Regexp(tre.star, Regexp(tre.symb, 'd')))),
            Regexp(tre.concat, Regexp(tre.star, Regexp(tre.symb, 'e')),
                Regexp(tre.concat, Regexp(tre.star, Regexp(tre.alt, Regexp(tre.star, Regexp(tre.symb, 'f')), Regexp(tre.star, Regexp(tre.symb, 'g')))),
                    Regexp(tre.concat, Regexp(tre.star, Regexp(tre.symb, 'h')),
                    Regexp(tre.star, Regexp(tre.alt, Regexp(tre.star, Regexp(tre.symb, 'i')), Regexp(tre.star, Regexp(tre.symb, 'j'))))))))))"""
"""r = Regexp(tre.concat,
        Regexp(tre.concat,
            Regexp(tre.concat,
                Regexp(tre.concat,
                    Regexp(tre.concat, Regexp(tre.symb, 'a'), Regexp(tre.symb, 'a')),
                    Regexp(tre.concat, Regexp(tre.symb, 'a'), Regexp(tre.symb, 'a'))),
                Regexp(tre.concat,
                    Regexp(tre.concat, Regexp(tre.symb, 'a'), Regexp(tre.symb, 'a')),
                    Regexp(tre.concat, Regexp(tre.symb, 'a'), Regexp(tre.symb, 'a')))),
            Regexp(tre.concat,
                Regexp(tre.concat,
                    Regexp(tre.concat, Regexp(tre.symb, 'a'), Regexp(tre.symb, 'a')),
                    Regexp(tre.concat, Regexp(tre.symb, 'a'), Regexp(tre.symb, 'a'))),
                Regexp(tre.concat,
                    Regexp(tre.concat, Regexp(tre.symb, 'a'), Regexp(tre.symb, 'a')),
                    Regexp(tre.concat, Regexp(tre.symb, 'a'), Regexp(tre.symb, 'a'))))),
        Regexp(tre.concat,
            Regexp(tre.concat,
                Regexp(tre.concat,
                    Regexp(tre.concat, Regexp(tre.symb, 'a'), Regexp(tre.symb, 'a')),
                    Regexp(tre.concat, Regexp(tre.symb, 'a'), Regexp(tre.symb, 'a'))),
                Regexp(tre.concat,
                    Regexp(tre.concat, Regexp(tre.symb, 'a'), Regexp(tre.symb, 'a')),
                    Regexp(tre.concat, Regexp(tre.symb, 'a'), Regexp(tre.symb, 'a')))),
            Regexp(tre.concat,
                Regexp(tre.concat,
                    Regexp(tre.concat, Regexp(tre.symb, 'a'), Regexp(tre.symb, 'a')),
                    Regexp(tre.concat, Regexp(tre.symb, 'a'), Regexp(tre.symb, 'a'))),
                Regexp(tre.concat,
                    Regexp(tre.concat, Regexp(tre.symb, 'a'), Regexp(tre.symb, 'a')),
                    Regexp(tre.concat, Regexp(tre.symb, 'a'), Regexp(tre.symb, 'a'))))))"""


x = input()
while(x != 'exit'):
    start = time.perf_counter()
    res = match(r, x)
    finish = time.perf_counter()
    print(f"Время: {finish - start:0.6f}")
    print(res)
    x = input()
