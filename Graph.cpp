
#include "Graph.hpp"

int id_ = 0;

void print_msg(Node* i) {
    switch (i->tp) {
        case 0: {
            std::cout << "String Literal: " << i->mess;
            break;
        }
        case 1: {
            std::cout << "Num Literal: " << i->num;
            break;
        }
        case 2: {
            std::cout << "Binary num Literal: " << i->num;
            break;
        }
        case 3: {
            std::cout << "Function name: main";
            break;
        }
        case 4: {
            std::cout << "Skip";
            break;
        }
        case 5: {
            std::cout << "Programm";
            break;
        }
        case 6: {
            std::cout << "Main function";
            break;
        }
        case 7: {
            std::cout << "Function";
            break;
        }
        case 8: {
            std::cout << "Parametrs";
            break;
        }
        case 9: {
            std::cout << "Sequence";
            break;
        }
        case 10: {
            std::cout << "Sequence2";
            break;
        }
        case 11: {
            std::cout << "Body";
            break;
        }
        case 12: {
            std::cout << "Operator";
            break;
        }
        case 13: {
            std::cout << "If";
            break;
        }
        case 14: {
            std::cout << "If_else";
            break;
        }
        case 15: {
            std::cout << "While";
            break;
        }
        case 16: {
            std::cout << "Function call";
            break;
        }
        case 17: {
            std::cout << "T1";
            break;
        }
        case 18: {
            std::cout << "T2";
            break;
        }
        case 19: {
            std::cout << "T3";
            break;
        }
        case 20: {
            std::cout << "T4";
            break;
        }
        case 21: {
            std::cout << "T5";
            break;
        }case 22: {
            std::cout << "T6";
            break;
        }
        case 23: {
            std::cout << "T7";
            break;
        }
        case 24: {
            std::cout << "T8";
            break;
        }
        case 25: {
            std::cout << "Identifier: " << i->mess;
            break;
        }
        case 26: {
            int k = i->mess.size();
            i->mess[k-1] = ' ';
            std::cout << "Comment: " << i->mess;
            break;
        }
        case 27: {
            std::cout << "=";
            break;
        }
        case 28: {
            std::cout << "||";
            break;
        }
        case 29: {
            std::cout << "&&";
            break;
        }
        case 30: {
            std::cout << "!";
            break;
        }
        case 31: {
            std::cout << "==";
            break;
        }
        case 32: {
            std::cout << "!=";
            break;
        }
        case 33: {
            std::cout << "<";
            break;
        }
        case 34: {
            std::cout << ">";
            break;
        }
        case 35: {
            std::cout << "<=";
            break;
        }
        case 36: {
            std::cout << ">=";
            break;
        }
        case 37: {
            std::cout << "+";
            break;
        }
        case 38: {
            std::cout << "-";
            break;
        }
        case 39: {
            std::cout << "*";
            break;
        }
        case 40: {
            std::cout << "/";
            break;
        }
        case 41: {
            std::cout << "^";
            break;
        }
        case 42: {
            std::cout << "Expression";
            break;
        }
        case 43: {
            std::cout << "Function name: " << i->mess;
            break;
        }


    }
}


void print_graph(Node* root) {
    std::cout << "v:"<< root->id << " (";
    print_msg(root);
    std::cout << ')';
    if (!root->children.empty()) {
        std::cout << "  ------->";
    } 
    for (auto i : root->children) {
        std::cout << "  v:"<< i->id << " (";
        print_msg(i);
        std::cout  << "),";
    }
    std::cout << '\n';
    for (auto i : root->children) {
        print_graph(i);
    }
}

void print_graph_recursively(Node* root) {
    std::cout << "Node(type:";
    print_msg(root);
    if (root->children.empty()) {
        std::cout << ")";
        return;
    }
    std::cout << ", children:";
    for (auto i : root->children) {
        print_graph_recursively(i);
    }
    std::cout << ")";
}