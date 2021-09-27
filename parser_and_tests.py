class regtype():
    empty = 0
    epsilon = 1
    char = 2
    star = 3
    alt = 4
    concat = 5
class regexp:
    def __init__(self, type, *args):
        self.t = type
        if (type == regtype.char):
            self.a1 = args[0]
        elif (type == regtype.star):
            self.a1 = args[0]
        elif (type == regtype.alt):
            self.a1 = args[0]
            self.a2 = args[1]
        elif (type == regtype.concat):
            self.a1 = args[0]
            self.a2 = args[1]
def intersect(r1, r2):
    if(r1.t == regtype.empty or r2.t == regtype.empty):
        return regtype.empty
    if(r1.t == regtype.epsilon and r2.t == regtype.epsilon):
        return regtype.epsilon
def union(r1, r2):
    if(r1.t == regtype.epsilon or r2.t == regtype.epsilon):
        return regtype.epsilon
    if(r1.t == regtype.empty and r2.t == regtype.empty):
        return regtype.empty
def nullable(r):
    if(r.t == regtype.empty or r.t == regtype.char):
        return regexp(regtype.empty)
    if(r.t == regtype.epsilon or r.t == regtype.star):
        return regexp(regtype.epsilon)
    if(r.t == regtype.concat):
        return regexp(intersect(nullable(r.a1), nullable(r.a2)))
    if(r.t == regtype.alt):
        return regexp(union(nullable(r.a1), nullable(r.a2)))

def derivative(s, r):
    if(r.t == regtype.empty or r.t == regtype.epsilon or (r.t == regtype.char and r.a1 != s)):
        return regexp(regtype.empty)
    if(r.t == regtype.char and r.a1 == s):
        return regexp(regtype.epsilon)
    if(r.t == regtype.alt):
        return regexp(regtype.alt, derivative(s, r.a1), derivative(s, r.a2))
    if(r.t == regtype.star):
        return regexp(regtype.concat, derivative(s, r.a1), r)
    if(r.t == regtype.concat):
        return regexp(regtype.alt, regexp(regtype.concat, derivative(s, r.a1), r.a2),
                      regexp(regtype.concat, nullable(r.a1), derivative(s, r.a2)))

def derivative1(s, r):
    if(s == ''):
        return r
    else:
        return derivative1(s[1:], derivative(s[0], r))
def match(string, regexp):
    return nullable(derivative1(string, regexp)).t == regtype.epsilon
#(ab)*
r1 = regexp(regtype.star, regexp(regtype.concat, regexp(regtype.char, 'a'), regexp(regtype.char, 'b')))
#a*b
r2 = regexp(regtype.concat, regexp(regtype.star, regexp(regtype.char, 'a')), regexp(regtype.char, 'b'))
#(ab*|ba*)
r3 = regexp(regtype.alt,
            regexp(regtype.concat,
                   regexp(regtype.char, 'a'), regexp(regtype.star, regexp(regtype.char, 'b'))),
            regexp(regtype.concat,
                   regexp(regtype.char, 'b'), regexp(regtype.star, regexp(regtype.char, 'a'))))
#(abacaba)
r4 = regexp(regtype.concat,
            regexp(regtype.char, 'a'), regexp(regtype.concat,
                                        regexp(regtype.char, 'b'), regexp(regtype.concat,
                                                                          regexp(regtype.char, 'a'), regexp(regtype.concat,
                                                                                                            regexp(regtype.char, 'c'), regexp(regtype.concat,
                                                                                                                                              regexp(regtype.char, 'a'), regexp(regtype.concat,
                                                                                                                                                                                regexp(regtype.char, 'b'), regexp(regtype.char, 'a')))))))
#(a*b*|b*a*)
r5 = regexp(regtype.alt,
            regexp(regtype.concat,
                   regexp(regtype.star, regexp(regtype.char, 'a')), regexp(regtype.star, regexp(regtype.char, 'b'))),
            regexp(regtype.concat,
                   regexp(regtype.star, regexp(regtype.char, 'b')), regexp(regtype.star, regexp(regtype.char, 'a'))))
                                 

while(1):
    a = input()
    print(match(a, r5))
