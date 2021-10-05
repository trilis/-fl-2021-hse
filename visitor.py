from antlr4 import *
from arithmeticLexer import arithmeticLexer
from arithmeticVisitor import arithmeticVisitor
from arithmeticParser import arithmeticParser
import sys
from pprint import pprint


import codecs
import sys

class EvalVisitor(arithmeticVisitor):
    def visitOpExpr(self, ctx):

        left = self.visit(ctx.left)
        right = self.visit(ctx.right)
        op = ctx.op.text;

        opchar1=op[0]
        if opchar1 == '*':
           val = left * right 
        elif opchar1 == '/':
           val = left / right 
        elif opchar1 == '+':
           val = left + right 
        elif opchar1 == '-':
           val = left - right
        else:
           raise ValueError("Unknown operator " + op) 
        print("visitOpExpr",opchar1,left,right,val)
        return val 

    def visitStart(self, ctx):
        print("visitStart",ctx.getText())
        return self.visit(ctx.expr())

    def visitAtomExpr(self, ctx):
        print("visitAtomExpr",int(ctx.getText()))
        return int(ctx.getText())

    def visitParenExpr(self, ctx):
        print("visitParenExpr",ctx.getText())
        return self.visit(ctx.expr())

def main():
    #lexer = arithmeticLexer(StdinStream())
    expression = "(( 4 - 10 ) * ( 3 + 4 )) / (( 2 - 5 ) * ( 3 + 4 ))"
    lexer = arithmeticLexer(InputStream(expression))
    stream = CommonTokenStream(lexer)
    parser = arithmeticParser(stream)
    tree = parser.start()
    answer = EvalVisitor().visit(tree) 
    print(answer)

if __name__ == '__main__':
    main()