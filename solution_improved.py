
import time

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
        
    def __init__(self, expr, *params):
        self.type = expr
        if (expr == empty):
            self.r = Regexp.Empty()
        elif (expr == epsilon):
            self.r = Regexp.Epsilon()  
        if (expr == char):
            self.r = Regexp.Char(params[0])

        elif (expr == star):
            if (params[0].type == empty):
                self.type = empty
                self.r = Regexp.Empty()
            elif (params[0].type == epsilon):    
                self.type = epsilon
                self.r = Regexp.Epsilon()  
            elif (params[0].type == star):    
                self.r = Regexp.Star(params[0].r.iReg)
            else:
                self.r = Regexp.Star(params[0])

        elif (expr == alt):
            if (params[0].type == empty or params[1].type == empty):
                if (params[0].type == empty):
                    temp = 1
                else:
                    temp = 0
                self.type = params[temp].type
                if (params[temp].type == empty):
                    self.r = Regexp.Empty()
                elif (params[temp].type == epsilon):
                    self.r = Regexp.Epsilon()
                elif (params[temp].type == char):
                    self.r = Regexp.Char(params[temp].r.c)
                elif (params[temp].type == star): 
                    self.r = Regexp.Star(params[temp].r.iReg)
                elif (params[temp].type == alt):
                    self.r = Regexp.Alt(params[temp].r.fReg, params[temp].r.sReg)
                elif  (params[temp].type == concat):
                    self.r = Regexp.Concat(params[temp].r.fReg, params[temp].r.sReg)
            elif (params[0].type == epsilon and nullable(params[1]) or params[1].type == epsilon and nullable(params[0])):
                if (params[0].type == epsilon and nullable(params[1])):
                    temp = 1
                else:
                    temp = 0
                if (params[temp].type == empty):
                    self.r = Regexp.Empty()
                elif (params[temp].type == epsilon):
                    self.r = Regexp.Epsilon()
                elif (params[temp].type == char):
                    self.r = Regexp.Char(params[temp].r.c)
                elif (params[temp].type == star): 
                    self.r = Regexp.Star(params[temp].r.iReg)
                elif (params[temp].type == alt):
                    self.r = Regexp.Alt(params[temp].r.fReg, params[temp].r.sReg)
                elif  (params[temp].type == concat):
                    self.r = Regexp.Concat(params[temp].r.fReg, params[temp].r.sReg)
            elif (params[0] == params[1]):
                self.type = params[0].type
                temp = 0
                if (params[temp].type == empty):
                    self.r = Regexp.Empty()
                elif (params[temp].type == epsilon):
                    self.r = Regexp.Epsilon()
                elif (params[temp].type == char):
                    self.r = Regexp.Char(params[temp].r.c)
                elif (params[temp].type == star): 
                    self.r = Regexp.Star(params[temp].r.iReg)
                elif (params[temp].type == alt):
                    self.r = Regexp.Alt(params[temp].r.fReg, params[temp].r.sReg)
                elif  (params[temp].type == concat):
                    self.r = Regexp.Concat(params[temp].r.fReg, params[temp].r.sReg)
            else:
                self.r = Regexp.Alt(params[0], params[1])

        elif (expr == concat):
            if (params[0].type == empty or params[1].type == empty):
                self.type = empty  
                self.r = Regexp.Empty() 
            elif (params[0].type == epsilon or params[1].type == epsilon):
                if (params[0].type == epsilon):
                    temp = 1
                else:
                    temp = 0
                self.type = params[temp].type
                if (params[temp].type == empty):
                    self.r = Regexp.Empty()
                elif (params[temp].type == epsilon):
                    self.r = Regexp.Epsilon()
                elif (params[temp].type == char):
                    self.r = Regexp.Char(params[temp].r.c)
                elif (params[temp].type == star): 
                    self.r = Regexp.Star(params[temp].r.iReg)
                elif (params[temp].type == alt):
                    self.r = Regexp.Alt(params[temp].r.fReg, params[temp].r.sReg)
                elif  (params[temp].type == concat):
                    self.r = Regexp.Concat(params[temp].r.fReg, params[temp].r.sReg)          
            else:
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
        if (nullable(reg.r.fReg)):
            return Regexp(alt, Regexp(concat, derivativeChar(c, reg.r.fReg),reg.r.sReg),  derivativeChar(c, reg.r.sReg))
        else:
            return Regexp(concat, derivativeChar(c, reg.r.fReg),reg.r.sReg)

def derivativeString(s, r):
    if (s == ''):
        return r
    else:
        return derivativeString(s[1:], derivativeChar(s[0], r))           

def nullable(r):
    if (r.type == empty):
        return False
    elif (r.type == epsilon):
        return True
    elif (r.type == char):
        return False
    elif (r.type == concat):
        return (nullable(r.r.fReg)) and (nullable(r.r.sReg))
    elif (r.type == alt):
        return (nullable(r.r.fReg)) or (nullable(r.r.sReg)) 
    elif (r.type == star):
        return True   


def match(r, s):
    return nullable(derivativeString(s, r))

# (00)*|(01)
test1 = Regexp(alt, Regexp(star, Regexp(concat, Regexp(char, '0'), 
            Regexp(char, '0') )) , Regexp(concat, Regexp(char, '0'), Regexp(char, '1'))) 

# a(b|c)*
test2 = Regexp(concat, Regexp(char, 'a'), 
            Regexp(star, Regexp(alt,
            Regexp(char, 'b'), Regexp(char, 'c'))))

# 1(01*0)*1
test3 = Regexp(concat, Regexp(char, '1'), 
            Regexp(concat, Regexp(star, Regexp(concat, Regexp(char, '0'), 
            Regexp(concat, Regexp(star, (Regexp(char, '1'))), 
            Regexp(char, '0')))), Regexp(char, '1')))

# (0|1|2|3|4|5)*
test4 = Regexp(star,
            Regexp(alt, Regexp(char, '0'), 
            Regexp(alt, Regexp(char, '1'), 
            Regexp(alt, Regexp(char, '2'), 
            Regexp(alt, Regexp(char, '3'), 
            Regexp(alt, Regexp(char, '4'), 
            Regexp(char, '5')))))))

# (0|1)*(a|b)
test5 = Regexp(concat, 
            Regexp(star, Regexp(alt, 
            Regexp(char, '0'), Regexp(char, '1'))),
            Regexp(alt, 
            Regexp(char, 'a'), Regexp(char, 'b'))) 

while (1):
    print("--------------")
    x = input()
    time_now = time.time()
    print("String: ", x)
    print("Answer: ", match(test4, x))
    print("Matched time: ", round(time.time() - time_now, 4))
