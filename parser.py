class char:
    def __init__(self, c):
        self.ch = c


class star:
    def __init__(self, rgxp):
        self.regexp = rgxp


class alt:
    def __init__(self, rgxp1, rgxp2):
        self.regex1 = rgxp1
        self.regex2 = rgxp2


class concat:
    def __init__(self, rgxp1, rgxp2):
        self.regex1 = rgxp1
        self.regex2 = rgxp2


class empty:
    pass


class epsilon:
    pass


def derivative(c, r):
    if type(r) is empty or type(r) is epsilon:
        return empty()
    elif type(r) is char:
        if c == r.ch:
            return epsilon()
        else:
            return empty()
    elif type(r) is alt:
        if type(r.regex1) is empty:
            return derivative(c, r.regex2)
        elif type(r.regex2) is empty:
            return derivative(c, r.regex1)
        elif type(r.regex2) is epsilon:
            if (nullable(r.regex1)):
                return derivative(c, r.regex1)
        elif type(r.regex1) is epsilon:
            if (nullable(r.regex2)):
                return derivative(c, r.regex2)
        elif r.regex1 == r.regex2:
            return derivative(c, r.regex1)
        else:
            return alt(derivative(c, r.regex1), derivative(c, r.regex2))
    elif type(r) is star:
        if type(r.regexp) is empty:
            return empty()
        elif type(r.regexp) is epsilon:
            return epsilon()
        elif type(r.regexp) is star:
            return derivative(c, r.regexp)
        else:  # I LOVE PYTHON, но работает он мильён лет
            return concat(derivative(c, r.regexp), star(r.regexp))
    elif type(r) is concat:
        if type(r.regex1) is empty:
            return empty()
        elif type(r.regex2) is empty:
            return empty()
        elif type(r.regex1) is epsilon:
            return derivative(c, r.regex2)
        elif type(r.regex2) is epsilon:
            return derivative(c, r.regex1)
        else:
            return alt(concat(derivative(c, r.regex1), r.regex2), concat(nullable(r.regex1), derivative(c, r.regex2)))
    else:
        pass


def intersect(r1, r2):
    if type(r1) is empty or type(r2) is empty:
        return empty()
    elif type(r1) is epsilon and type(r2) is epsilon:
        return epsilon()
    else:
        pass


def union(r1, r2):
    if type(r1) is epsilon or type(r2) is epsilon:
        return epsilon()
    elif type(r1) is empty and type(r2) is empty:
        return empty()
    else:
        pass


def nullable(r):
    if type(r) is empty or type(r) is char:
        return empty()
    elif type(r) is epsilon or type(r) is star:
        return epsilon()
    elif type(r) is concat:
        return intersect(nullable(r.regex1), nullable(r.regex2))
    elif type(r) is alt:
        return union(nullable(r.regex1), nullable(r.regex2))
    else:
        pass


def derivative2(str, r):
    if str == '':
        return r
    else:  # остальное/откусанный
        return derivative2(str[1:], derivative(str[0], r))


def match(r, s):
    return type(nullable(derivative2(s, r))) is epsilon


def main():
    inlst = [x for x in input().split()]
    for str in inlst:
        print('--------------- ' + str + ' ---------------')
        print(match(concat(char('a'), char('b')), str), '<---- ab')  # ab
        print(match(alt(char('a'), char('b')), str), '<---- a|b')  # a|b
        print(match(star(char('a')), str), '<---- a*')  # a*
        print(match(concat(star(alt(concat(char('a'), char('b')), concat(char('b'), char('a')))), char('c')), str),
              '<---- (ab|ba)*c')  # (ab|ba)*c
        print(match(concat(concat(char('a'), char('a')), star(alt(alt(star(char('h')), concat(char('a'), char('s'))),
                                                                  alt(concat(char('a'), concat(char('s'), char('d'))),
                                                                      concat(concat(char('a'), char('s')),
                                                                             concat(char('d'), char('f'))))))), str),
              '<---- aa(h*|as|asd|asdf)*')  # aa(h*|as|asd|asdf)* Здесь на длинной регулярке работает очень долго (я успел проголодаться)


if __name__ == "__main__":
    main()

# ab a abbaababbac aahhhhhhhhhhhhhhhasasasdasdasdasdasdasdf
