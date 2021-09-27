
empty = 1           
epsilon = 2
char = 3
star = 4           
alt = 5
concat = 6

class Regexp:
    class Empty:
        pass
    class Epsilon:
        pass
    class Char:
        def __init__(self, char):
            self.c = char
    class Star:
        def __init__(self, reg):
            self.iReg = reg
    class Alt:
        def __init__(self, firstReg, secondReg):
            self.fReg = firstReg
            self.sReg = secondReg
    class Concat:
        def __init__(self, firstReg, secondReg):
            self.fReg = firstReg
            self.sReg = secondReg

    def __init__ (self, type, *params):
        self.type = type
        if (type == empty):
            self.r = Regexp.Empty()
        elif (type == epsilon):
            self.r = Regexp.Epsilon()  
        elif (type == char): 
            self.r = Regexp.Char(params[0])
        elif (type == star): 
            self.r = Regexp.Star(params[0])
        elif (type == alt): 
            self.r = Regexp.Alt(params[0], params[1])
        elif (type == concat): 
            self.r = Regexp.Concat(params[0], params[1])            


def derivativeChar(c, reg):
    if (reg.type == empty or reg.type == epsilon):
        return Regexp(empty)
    elif (reg.type == char and reg.r.c == c):
        return Regexp(epsilon)
    elif (reg.type == char and reg.r.c != c):
        return Regexp(empty)
    elif (reg.type == alt):
        return Regexp(alt, derivativeChar(c, reg.r.fReg),  derivativeChar(c, reg.r.sReg))
    elif (reg.type == star):
        return Regexp(concat, derivativeChar(c, reg.r.iReg),  Regexp(star, reg.r.iReg))
    elif (reg.type == concat):
        return Regexp(alt, Regexp(concat, derivativeChar(c, reg.r.fReg),reg.r.sReg), 
                           Regexp(concat, nullable(reg.r.fReg), derivativeChar(c, reg.r.sReg)))   


def derivativeString(s, r):
    if (s == ""):
        return r
    else:
        return derivativeString(s[1:], derivativeChar(s[0], r)) 

def intersect(r1, r2):
    if ((r1.type == empty) or (r2.type == empty)):
        return Regexp(empty)
    elif ((r1.type == epsilon) and (r2.type == epsilon)):
        return Regexp(epsilon) 

def union(r1, r2):
    if ((r1.type == epsilon) or (r2.type == epsilon)):
        return Regexp(epsilon)
    elif ((r1.type == empty) and (r2.type == empty)):
        return Regexp(empty)   


def nullable(r):
    if (r.type == empty):
        return Regexp(empty)
    elif (r.type == epsilon):
        return Regexp(epsilon)
    elif (r.type == char):
        return Regexp(empty)
    elif (r.type == concat):
        return intersect((nullable(r.r.fReg)),(nullable(r.r.sReg)))
    elif (r.type == alt):
        return union((nullable(r.r.fReg)),(nullable(r.r.sReg)))    
    elif (r.type == star):
        return Regexp(epsilon)    

def match(r, s):
    return ((nullable(derivativeString(s, r))).type == epsilon)
                              