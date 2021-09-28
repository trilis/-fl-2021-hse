
from solution_improved import match
from solution_improved import Regexp
#from solution import match
#from solution import Regexp
import time

empty = 1           
epsilon = 2
char = 3
star = 4           
alt = 5
concat = 6

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
    line = input()
    time_now = time.time()
    print("String: ", line)
    print("Answer: ", match(test4, line))
    print("Matched time: ", round(time.time() - time_now, 4)) 
