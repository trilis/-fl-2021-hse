#include <vector>
#include <string>
#include <iostream>

extern int id_;

enum Messege_type {
    STR_LITERAL_,
    NUM_LITERAL_D_,
    NUM_LITERAL_B_,
    MAIN_,
    SKIP_,
    PROGRAMM,
    FUNC_MAIN,
    FUNC,
    PARAMS,
    SEQ,
    BEQ,
    BODY,
    OPERATOR,
    IF_,
    IF_ELSE,
    WHILE_,
    CALLER,
    T1,
    T2,
    T3,
    T4,
    T5,
    T6,
    T7,
    T8,
    IDENTIFIER_,
    COMMENT_,
    EQUEL_,
    OR_,
    AND_,
    NOT_,
    IS_EQUEL_,
    IS_NOT_EQUEL_,
    LT_,
    GT_,
    LT_EQ_,
    GT_EQ_,
    PLUS_,
    MINUS_,
    TIMES_,
    DIV_,
    EXP_, 
    EXPRESSION,
    FUNC_NAME        
};

struct Node {
    std::string mess;
    int num;
    int id;
    Messege_type tp;
    std::vector<Node*> children;
    Node(Messege_type a) : tp(a) {id = id_++; }
    Node(Messege_type a, std::vector<Node*> ch) : tp(a), children(ch) { id = id_++; }
    Node(Messege_type a, std::string str) : tp(a), mess(str) { id = id_++; }
    Node(Messege_type a, int num_)  : tp(a), num(num_) { id = id_++; }
};

struct Graph { 
    Node* root;
    Graph(Node* r) : root(r) {}
};

void print_graph(Node* root);

void print_graph_recursively(Node* root);